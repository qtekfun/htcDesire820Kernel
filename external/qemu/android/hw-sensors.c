/* Copyright (C) 2009 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

#include <math.h>
#include "android/hw-sensors.h"
#include "android/utils/debug.h"
#include "android/utils/misc.h"
#include "android/utils/system.h"
#include "android/hw-qemud.h"
#include "android/globals.h"
#include "hw/hw.h"
#include "qemu-char.h"
#include "qemu-timer.h"
#include "android/sensors-port.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)  VERBOSE_PRINT(sensors,__VA_ARGS__)
#define  V(...)  VERBOSE_PRINT(init,__VA_ARGS__)

#define  T_ACTIVE  0

#if T_ACTIVE
#define  T(...)  VERBOSE_PRINT(sensors,__VA_ARGS__)
#else
#define  T(...)   ((void)0)
#endif



static const struct {
    const char*  name;
    int          id;
} _sSensors[MAX_SENSORS] = {
#define SENSOR_(x,y)  { y, ANDROID_SENSOR_##x },
  SENSORS_LIST
#undef SENSOR_
};


static int
_sensorIdFromName( const char*  name )
{
    int  nn;
    for (nn = 0; nn < MAX_SENSORS; nn++)
        if (!strcmp(_sSensors[nn].name,name))
            return _sSensors[nn].id;
    return -1;
}

static const char*
_sensorNameFromId( int  id )
{
    int  nn;
    for (nn = 0; nn < MAX_SENSORS; nn++)
        if (id == _sSensors[nn].id)
            return _sSensors[nn].name;
    return NULL;
}

typedef struct {
    float a, b, c;
} SensorValues;

typedef struct {
    float   x, y, z;
} Acceleration;


typedef struct {
    float  x, y, z;
} MagneticField;


typedef struct {
    float  azimuth;
    float  pitch;
    float  roll;
} Orientation;


typedef struct {
    float  celsius;
} Temperature;


typedef struct {
    float  value;
} Proximity;

typedef struct {
    char       enabled;
    union {
        SensorValues   value;
        Acceleration   acceleration;
        MagneticField  magnetic;
        Orientation    orientation;
        Temperature    temperature;
        Proximity      proximity;
    } u;
} Sensor;

#define  HEADER_SIZE  4
#define  BUFFER_SIZE  512

typedef struct HwSensorClient   HwSensorClient;

typedef struct {
    QemudService*       service;
    Sensor              sensors[MAX_SENSORS];
    HwSensorClient*     clients;
    AndroidSensorsPort* sensors_port;
} HwSensors;

struct HwSensorClient {
    HwSensorClient*  next;
    HwSensors*       sensors;
    QemudClient*     client;
    QEMUTimer*       timer;
    uint32_t         enabledMask;
    int32_t          delay_ms;
};

static void
_hwSensorClient_free( HwSensorClient*  cl )
{
    
    if (cl->sensors) {
        HwSensorClient**  pnode = &cl->sensors->clients;
        for (;;) {
            HwSensorClient*  node = *pnode;
            if (node == NULL)
                break;
            if (node == cl) {
                *pnode = cl->next;
                break;
            }
            pnode = &node->next;
        }
        cl->next    = NULL;
        cl->sensors = NULL;
    }

    
    if (cl->client) {
        qemud_client_close(cl->client);
        cl->client = NULL;
    }
    
    if (cl->timer) {
        qemu_del_timer(cl->timer);
        qemu_free_timer(cl->timer);
        cl->timer = NULL;
    }
    AFREE(cl);
}

static void  _hwSensorClient_tick(void*  opaque);


static HwSensorClient*
_hwSensorClient_new( HwSensors*  sensors )
{
    HwSensorClient*  cl;

    ANEW0(cl);

    cl->sensors     = sensors;
    cl->enabledMask = 0;
    cl->delay_ms    = 800;
    cl->timer       = qemu_new_timer_ns(vm_clock, _hwSensorClient_tick, cl);

    cl->next         = sensors->clients;
    sensors->clients = cl;

    return cl;
}


static void  _hwSensorClient_receive( HwSensorClient*  cl,
                                      uint8_t*         query,
                                      int              querylen );


static void
_hwSensorClient_recv( void*  opaque, uint8_t*  msg, int  msglen,
                      QemudClient*  client )
{
    HwSensorClient*  cl = opaque;

    _hwSensorClient_receive(cl, msg, msglen);
}

static void
_hwSensorClient_close( void*  opaque )
{
    HwSensorClient*  cl = opaque;

    
    cl->client = NULL;
    _hwSensorClient_free(cl);
}

static void
_hwSensorClient_send( HwSensorClient*  cl, const uint8_t*  msg, int  msglen )
{
    D("%s: '%s'", __FUNCTION__, quote_bytes((const void*)msg, msglen));
    qemud_client_send(cl->client, msg, msglen);
}

static int
_hwSensorClient_enabled( HwSensorClient*  cl, int  sensorId )
{
    return (cl->enabledMask & (1 << sensorId)) != 0;
}

static void
_hwSensorClient_tick( void*  opaque )
{
    HwSensorClient*  cl = opaque;
    HwSensors*       hw  = cl->sensors;
    int64_t          delay = cl->delay_ms;
    int64_t          now_ns;
    uint32_t         mask  = cl->enabledMask;
    Sensor*          sensor;
    char             buffer[128];

    if (_hwSensorClient_enabled(cl, ANDROID_SENSOR_ACCELERATION)) {
        sensor = &hw->sensors[ANDROID_SENSOR_ACCELERATION];
        snprintf(buffer, sizeof buffer, "acceleration:%g:%g:%g",
                 sensor->u.acceleration.x,
                 sensor->u.acceleration.y,
                 sensor->u.acceleration.z);
        _hwSensorClient_send(cl, (uint8_t*)buffer, strlen(buffer));
    }

    if (_hwSensorClient_enabled(cl, ANDROID_SENSOR_MAGNETIC_FIELD)) {
        sensor = &hw->sensors[ANDROID_SENSOR_MAGNETIC_FIELD];
        
        snprintf(buffer, sizeof buffer, "magnetic:%g:%g:%g",
                 sensor->u.magnetic.x,
                 sensor->u.magnetic.y,
                 sensor->u.magnetic.z);
        _hwSensorClient_send(cl, (uint8_t*)buffer, strlen(buffer));
    }

    if (_hwSensorClient_enabled(cl, ANDROID_SENSOR_ORIENTATION)) {
        sensor = &hw->sensors[ANDROID_SENSOR_ORIENTATION];
        snprintf(buffer, sizeof buffer, "orientation:%g:%g:%g",
                 sensor->u.orientation.azimuth,
                 sensor->u.orientation.pitch,
                 sensor->u.orientation.roll);
        _hwSensorClient_send(cl, (uint8_t*)buffer, strlen(buffer));
    }

    if (_hwSensorClient_enabled(cl, ANDROID_SENSOR_TEMPERATURE)) {
        sensor = &hw->sensors[ANDROID_SENSOR_TEMPERATURE];
        snprintf(buffer, sizeof buffer, "temperature:%g",
                 sensor->u.temperature.celsius);
        _hwSensorClient_send(cl, (uint8_t*)buffer, strlen(buffer));
    }

    if (_hwSensorClient_enabled(cl, ANDROID_SENSOR_PROXIMITY)) {
        sensor = &hw->sensors[ANDROID_SENSOR_PROXIMITY];
        snprintf(buffer, sizeof buffer, "proximity:%g",
                 sensor->u.proximity.value);
        _hwSensorClient_send(cl, (uint8_t*) buffer, strlen(buffer));
    }

    now_ns = qemu_get_clock_ns(vm_clock);

    snprintf(buffer, sizeof buffer, "sync:%" PRId64, now_ns/1000);
    _hwSensorClient_send(cl, (uint8_t*)buffer, strlen(buffer));

    if (mask == 0)
        return;

    if (delay < 20)
        delay = 20;

    delay *= 1000000LL;  
    qemu_mod_timer(cl->timer, now_ns + delay);
}

static void
_hwSensorClient_receive( HwSensorClient*  cl, uint8_t*  msg, int  msglen )
{
    HwSensors*  hw = cl->sensors;

    D("%s: '%.*s'", __FUNCTION__, msglen, msg);

    if (msglen == 12 && !memcmp(msg, "list-sensors", 12)) {
        char  buff[12];
        int   mask = 0;
        int   nn;

        for (nn = 0; nn < MAX_SENSORS; nn++) {
            if (hw->sensors[nn].enabled)
                mask |= (1 << nn);
        }

        snprintf(buff, sizeof buff, "%d", mask);
        _hwSensorClient_send(cl, (const uint8_t*)buff, strlen(buff));
        return;
    }

    if (msglen == 4 && !memcmp(msg, "wake", 4)) {
        _hwSensorClient_send(cl, (const uint8_t*)"wake", 4);
        return;
    }

    if (msglen > 10 && !memcmp(msg, "set-delay:", 10)) {
        cl->delay_ms = atoi((const char*)msg+10);
        if (cl->enabledMask != 0)
            _hwSensorClient_tick(cl);

        return;
    }

    if (msglen > 4 && !memcmp(msg, "set:", 4)) {
        char*  q;
        int    id, enabled, oldEnabledMask = cl->enabledMask;
        msg += 4;
        q    = strchr((char*)msg, ':');
        if (q == NULL) {  
            D("%s: ignore bad 'set' command", __FUNCTION__);
            return;
        }
        *q++ = 0;

        id = _sensorIdFromName((const char*)msg);
        if (id < 0 || id >= MAX_SENSORS) {
            D("%s: ignore unknown sensor name '%s'", __FUNCTION__, msg);
            return;
        }

        if (!hw->sensors[id].enabled) {
            D("%s: trying to set disabled %s sensor", __FUNCTION__, msg);
            return;
        }
        enabled = (q[0] == '1');

        if (enabled)
            cl->enabledMask |= (1 << id);
        else
            cl->enabledMask &= ~(1 << id);

        if (cl->enabledMask != oldEnabledMask) {
            D("%s: %s %s sensor", __FUNCTION__,
                (cl->enabledMask & (1 << id))  ? "enabling" : "disabling",  msg);
        }

        
        if (hw->sensors_port != NULL) {
            if (enabled) {
                sensors_port_enable_sensor(hw->sensors_port, (const char*)msg);
            } else {
                sensors_port_disable_sensor(hw->sensors_port, (const char*)msg);
            }
        }

        _hwSensorClient_tick(cl);
        return;
    }

    D("%s: ignoring unknown query", __FUNCTION__);
}

static void
_hwSensorClient_save( QEMUFile*  f, QemudClient*  client, void*  opaque  )
{
    HwSensorClient* sc = opaque;

    qemu_put_be32(f, sc->delay_ms);
    qemu_put_be32(f, sc->enabledMask);
    qemu_put_timer(f, sc->timer);
}

static int
_hwSensorClient_load( QEMUFile*  f, QemudClient*  client, void*  opaque  )
{
    HwSensorClient* sc = opaque;

    sc->delay_ms = qemu_get_be32(f);
    sc->enabledMask = qemu_get_be32(f);
    qemu_get_timer(f, sc->timer);

    return 0;
}

static QemudClient*
_hwSensors_connect( void*  opaque,
                    QemudService*  service,
                    int  channel,
                    const char* client_param )
{
    HwSensors*       sensors = opaque;
    HwSensorClient*  cl      = _hwSensorClient_new(sensors);
    QemudClient*     client  = qemud_client_new(service, channel, client_param, cl,
                                                _hwSensorClient_recv,
                                                _hwSensorClient_close,
                                                _hwSensorClient_save,
                                                _hwSensorClient_load );
    qemud_client_set_framing(client, 1);
    cl->client = client;

    return client;
}

static void
_hwSensors_setSensorValue( HwSensors*  h, int sensor_id, float a, float b, float c )
{
    Sensor* s = &h->sensors[sensor_id];

    s->u.value.a = a;
    s->u.value.b = b;
    s->u.value.c = c;
}

static void
_hwSensors_save( QEMUFile*  f, QemudService*  sv, void*  opaque)
{
    HwSensors* h = opaque;

    
    qemu_put_be32(f, MAX_SENSORS);
    AndroidSensor i;
    for (i = 0 ; i < MAX_SENSORS; i++) {
        Sensor* s = &h->sensors[i];
        qemu_put_be32(f, s->enabled);

        switch (i) {
        case ANDROID_SENSOR_ACCELERATION:
            qemu_put_float(f, s->u.acceleration.x);
            qemu_put_float(f, s->u.acceleration.y);
            qemu_put_float(f, s->u.acceleration.z);
            break;
        case ANDROID_SENSOR_MAGNETIC_FIELD:
            qemu_put_float(f, s->u.magnetic.x);
            qemu_put_float(f, s->u.magnetic.y);
            qemu_put_float(f, s->u.magnetic.z);
            break;
        case ANDROID_SENSOR_ORIENTATION:
            qemu_put_float(f, s->u.orientation.azimuth);
            qemu_put_float(f, s->u.orientation.pitch);
            qemu_put_float(f, s->u.orientation.roll);
            break;
        case ANDROID_SENSOR_TEMPERATURE:
            qemu_put_float(f, s->u.temperature.celsius);
            break;
        case ANDROID_SENSOR_PROXIMITY:
            qemu_put_float(f, s->u.proximity.value);
            break;
        case MAX_SENSORS:
            break;
        }
    }
}


static int
_hwSensors_load( QEMUFile*  f, QemudService*  s, void*  opaque)
{
    HwSensors* h = opaque;

    
    int32_t num_sensors = qemu_get_be32(f);
    if (num_sensors > MAX_SENSORS) {
        D("%s: cannot load: snapshot requires %d sensors, %d available\n",
          __FUNCTION__, num_sensors, MAX_SENSORS);
        return -EIO;
    }

    
    AndroidSensor i;
    for (i = 0 ; i < num_sensors; i++) {
        Sensor* s = &h->sensors[i];
        s->enabled = qemu_get_be32(f);

        switch (i) {
        case ANDROID_SENSOR_ACCELERATION:
            s->u.acceleration.x = qemu_get_float(f);
            s->u.acceleration.y = qemu_get_float(f);
            s->u.acceleration.z = qemu_get_float(f);
            break;
        case ANDROID_SENSOR_MAGNETIC_FIELD:
            s->u.magnetic.x = qemu_get_float(f);
            s->u.magnetic.y = qemu_get_float(f);
            s->u.magnetic.z = qemu_get_float(f);
            break;
        case ANDROID_SENSOR_ORIENTATION:
            s->u.orientation.azimuth = qemu_get_float(f);
            s->u.orientation.pitch   = qemu_get_float(f);
            s->u.orientation.roll    = qemu_get_float(f);
            break;
        case ANDROID_SENSOR_TEMPERATURE:
            s->u.temperature.celsius = qemu_get_float(f);
            break;
        case ANDROID_SENSOR_PROXIMITY:
            s->u.proximity.value = qemu_get_float(f);
            break;
        case MAX_SENSORS:
            break;
        }
    }

    for ( ; i < MAX_SENSORS; i++ ) {
        h->sensors[i].enabled = 0;
    }

    return 0;
}


static void
_hwSensors_setProximity( HwSensors*  h, float value )
{
    Sensor*  s = &h->sensors[ANDROID_SENSOR_PROXIMITY];
    s->u.proximity.value = value;
}

static void
_hwSensors_setCoarseOrientation( HwSensors*  h, AndroidCoarseOrientation  orient )
{
    const double  g      = 9.81;
    const double  angle  = 20.0;
    const double  cos_angle = cos(angle/M_PI);
    const double  sin_angle = sin(angle/M_PI);

    switch (orient) {
    case ANDROID_COARSE_PORTRAIT:
        _hwSensors_setSensorValue( h, ANDROID_SENSOR_ACCELERATION, 0., g*cos_angle, g*sin_angle );
        break;

    case ANDROID_COARSE_LANDSCAPE:
        _hwSensors_setSensorValue( h, ANDROID_SENSOR_ACCELERATION, g*cos_angle, 0., g*sin_angle );
        break;
    default:
        ;
    }
}


static void
_hwSensors_init( HwSensors*  h )
{
    h->sensors_port = sensors_port_create(h);
    if (h->sensors_port == NULL) {
        V("Realistic sensor emulation is not available, since the remote controller is not accessible:\n %s",
          strerror(errno));
    }

    h->service = qemud_service_register("sensors", 0, h, _hwSensors_connect,
                                        _hwSensors_save, _hwSensors_load);

    if (android_hw->hw_accelerometer) {
        h->sensors[ANDROID_SENSOR_ACCELERATION].enabled = 1;
    }

    if (android_hw->hw_sensors_proximity) {
        h->sensors[ANDROID_SENSOR_PROXIMITY].enabled = 1;
    }

    if (android_hw->hw_sensors_magnetic_field) {
        h->sensors[ANDROID_SENSOR_MAGNETIC_FIELD].enabled = 1;
    }

    if (android_hw->hw_sensors_orientation) {
        h->sensors[ANDROID_SENSOR_ORIENTATION].enabled = 1;
    }

    if (android_hw->hw_sensors_temperature) {
        h->sensors[ANDROID_SENSOR_TEMPERATURE].enabled = 1;
    }


    _hwSensors_setCoarseOrientation(h, ANDROID_COARSE_PORTRAIT);
    _hwSensors_setProximity(h, 1);
}

static HwSensors    _sensorsState[1];

void
android_hw_sensors_init( void )
{
    HwSensors*  hw = _sensorsState;

    if (hw->service == NULL) {
        _hwSensors_init(hw);
        D("%s: sensors qemud service initialized", __FUNCTION__);
    }
}

extern void
android_sensors_set_coarse_orientation( AndroidCoarseOrientation  orient )
{
    android_hw_sensors_init();
    _hwSensors_setCoarseOrientation(_sensorsState, orient);
}

extern const char*
android_sensors_get_name_from_id( int sensor_id )
{
    if (sensor_id < 0 || sensor_id >= MAX_SENSORS)
        return NULL;

    return _sensorNameFromId(sensor_id);
}

extern int
android_sensors_get_id_from_name( char* sensorname )
{
    HwSensors* hw = _sensorsState;

    if (sensorname == NULL)
        return SENSOR_STATUS_UNKNOWN;

    int id = _sensorIdFromName(sensorname);

    if (id < 0 || id >= MAX_SENSORS)
        return SENSOR_STATUS_UNKNOWN;

    if (hw->service != NULL) {
        if (! hw->sensors[id].enabled)
            return SENSOR_STATUS_DISABLED;
    } else
        return SENSOR_STATUS_NO_SERVICE;

    return id;
}

extern int
android_sensors_get( int sensor_id, float* a, float* b, float* c )
{
    HwSensors* hw = _sensorsState;

    *a = 0;
    *b = 0;
    *c = 0;

    if (sensor_id < 0 || sensor_id >= MAX_SENSORS)
        return SENSOR_STATUS_UNKNOWN;

    Sensor* sensor = &hw->sensors[sensor_id];
    if (hw->service != NULL) {
        if (! sensor->enabled)
            return SENSOR_STATUS_DISABLED;
    } else
        return SENSOR_STATUS_NO_SERVICE;

    *a = sensor->u.value.a;
    *b = sensor->u.value.b;
    *c = sensor->u.value.c;

    return SENSOR_STATUS_OK;
}

extern int
android_sensors_set( int sensor_id, float a, float b, float c )
{
    HwSensors* hw = _sensorsState;

    if (sensor_id < 0 || sensor_id >= MAX_SENSORS)
        return SENSOR_STATUS_UNKNOWN;

    if (hw->service != NULL) {
        if (! hw->sensors[sensor_id].enabled)
            return SENSOR_STATUS_DISABLED;
    } else
        return SENSOR_STATUS_NO_SERVICE;

    _hwSensors_setSensorValue(hw, sensor_id, a, b, c);

    return SENSOR_STATUS_OK;
}

extern uint8_t
android_sensors_get_sensor_status( int sensor_id )
{
    HwSensors* hw = _sensorsState;

    if (sensor_id < 0 || sensor_id >= MAX_SENSORS)
        return SENSOR_STATUS_UNKNOWN;

    return hw->sensors[sensor_id].enabled;
}

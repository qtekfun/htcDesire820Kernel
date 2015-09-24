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
#ifndef _android_sensors_h
#define _android_sensors_h

#include "qemu-common.h"

extern void  android_hw_sensors_init( void );

typedef enum{
    SENSOR_STATUS_NO_SERVICE = -3,
    SENSOR_STATUS_DISABLED   = -2,
    SENSOR_STATUS_UNKNOWN    = -1,
    SENSOR_STATUS_OK         = 0,
} SensorStatus;

#define  SENSORS_LIST  \
    SENSOR_(ACCELERATION,"acceleration") \
    SENSOR_(MAGNETIC_FIELD,"magnetic-field") \
    SENSOR_(ORIENTATION,"orientation") \
    SENSOR_(TEMPERATURE,"temperature") \
    SENSOR_(PROXIMITY,"proximity") \

typedef enum {
#define  SENSOR_(x,y)  ANDROID_SENSOR_##x,
    SENSORS_LIST
#undef   SENSOR_
    MAX_SENSORS  
} AndroidSensor;

extern void  android_hw_sensor_enable( AndroidSensor  sensor );

typedef enum {
    ANDROID_COARSE_PORTRAIT,
    ANDROID_COARSE_LANDSCAPE
} AndroidCoarseOrientation;

extern void android_sensors_set_coarse_orientation( AndroidCoarseOrientation  orient );

extern int android_sensors_get( int sensor_id, float* a, float* b, float* c );

extern int android_sensors_set( int sensor_id, float a, float b, float c );

extern int android_sensors_get_id_from_name( char* sensorname );

extern const char* android_sensors_get_name_from_id( int sensor_id );

extern uint8_t android_sensors_get_sensor_status( int sensor_id );

#endif 

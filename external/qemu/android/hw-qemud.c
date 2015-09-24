/* Copyright (C) 2007-2008 The Android Open Source Project
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
#include "android/hw-qemud.h"
#include "android/utils/debug.h"
#include "android/utils/misc.h"
#include "android/utils/system.h"
#include "android/utils/bufprint.h"
#include "android/looper.h"
#include "hw/hw.h"
#include "hw/goldfish_pipe.h"
#include "qemu-char.h"
#include "charpipe.h"
#include "cbuffer.h"
#include "utils/panic.h"

#define  D(...)    VERBOSE_PRINT(qemud,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(qemud)

#define  T_ACTIVE   0

#if T_ACTIVE
#define  T(...)    VERBOSE_PRINT(qemud,__VA_ARGS__)
#else
#define  T(...)    ((void)0)
#endif

#define  MAX_SERIAL_PAYLOAD        4000

#define  MAX_FRAME_PAYLOAD  65535

#define QEMUD_SAVE_VERSION 2

#define min(a, b) (((a) < (b)) ? (a) : (b))


#ifdef TARGET_ARM
#define  SUPPORT_LEGACY_QEMUD  1
#endif
#ifdef TARGET_I386
#define  SUPPORT_LEGACY_QEMUD  0 
#endif
#if SUPPORT_LEGACY_QEMUD
#include "telephony/android_modem.h"
#include "telephony/modem_driver.h"
#endif




typedef struct QemudSink {
    int       used;  
    int       size;  
    uint8_t*  buff;
} QemudSink;

static void
qemud_sink_save(QEMUFile* f, QemudSink* s)
{
    qemu_put_be32(f, s->used);
    qemu_put_be32(f, s->size);
}

static int
qemud_sink_load(QEMUFile* f, QemudSink* s)
{
    s->used = qemu_get_be32(f);
    s->size = qemu_get_be32(f);
    return 0;
}


static void
qemud_sink_reset( QemudSink*  ss, int  size, uint8_t*  buffer )
{
    ss->used = 0;
    ss->size = size;
    ss->buff = buffer;
}

static int
qemud_sink_fill( QemudSink*  ss, const uint8_t* *pmsg, int  *plen)
{
    int  avail = ss->size - ss->used;

    if (avail <= 0)
        return 1;

    if (avail > *plen)
        avail = *plen;

    memcpy(ss->buff + ss->used, *pmsg, avail);
    *pmsg += avail;
    *plen -= avail;
    ss->used += avail;

    return (ss->used == ss->size);
}

static int
qemud_sink_needed( QemudSink*  ss )
{
    return ss->size - ss->used;
}



#define  HEADER_SIZE    6

#define  LENGTH_OFFSET  2
#define  LENGTH_SIZE    4

#define  CHANNEL_OFFSET 0
#define  CHANNEL_SIZE   2

#if SUPPORT_LEGACY_QEMUD
typedef enum {
    QEMUD_VERSION_UNKNOWN,
    QEMUD_VERSION_LEGACY,
    QEMUD_VERSION_NORMAL
} QemudVersion;

#  define  LEGACY_LENGTH_OFFSET   0
#  define  LEGACY_CHANNEL_OFFSET  4
#endif

#define  FRAME_HEADER_SIZE  4

#define  BUFFER_SIZE    MAX_SERIAL_PAYLOAD

typedef void  (*QemudSerialReceive)( void*  opaque, int  channel, uint8_t*  msg, int  msglen);

typedef struct QemudSerial {
    CharDriverState*  cs;  

    
    ABool         need_header;
    int           overflow;
    int           in_size;
    int           in_channel;
#if SUPPORT_LEGACY_QEMUD
    QemudVersion  version;
#endif
    QemudSink     header[1];
    QemudSink     payload[1];
    uint8_t       data0[MAX_SERIAL_PAYLOAD+1];

    
    QemudSerialReceive  recv_func;    
    void*               recv_opaque;  
} QemudSerial;


static void
qemud_serial_save(QEMUFile* f, QemudSerial* s)
{

    
    qemu_put_be32(f, s->need_header);
    qemu_put_be32(f, s->overflow);
    qemu_put_be32(f, s->in_size);
    qemu_put_be32(f, s->in_channel);
#if SUPPORT_LEGACY_QEMUD
    qemu_put_be32(f, s->version);
#endif
    qemud_sink_save(f, s->header);
    qemud_sink_save(f, s->payload);
    qemu_put_be32(f, MAX_SERIAL_PAYLOAD+1);
    qemu_put_buffer(f, s->data0, MAX_SERIAL_PAYLOAD+1);
}

static int
qemud_serial_load(QEMUFile* f, QemudSerial* s)
{
    
    s->need_header = qemu_get_be32(f);
    s->overflow    = qemu_get_be32(f);
    s->in_size     = qemu_get_be32(f);
    s->in_channel  = qemu_get_be32(f);
#if SUPPORT_LEGACY_QEMUD
    s->version = qemu_get_be32(f);
#endif
    qemud_sink_load(f, s->header);
    qemud_sink_load(f, s->payload);

    
    s->header->buff = s->payload->buff = s->data0;

    int len = qemu_get_be32(f);
    if (len - 1 > MAX_SERIAL_PAYLOAD) {
        D("%s: load failed: size of saved payload buffer (%d) exceeds "
          "current maximum (%d)\n",
          __FUNCTION__, len - 1, MAX_SERIAL_PAYLOAD);
        return -EIO;
    }
    int ret;
    if ((ret = qemu_get_buffer(f, s->data0, len)) != len) {
        D("%s: failed to load serial buffer contents (tried reading %d bytes, got %d)\n",
          __FUNCTION__, len, ret);
        return -EIO;
    }

    return 0;
}

static int
qemud_serial_can_read( void*  opaque )
{
    QemudSerial*  s = opaque;

    if (s->overflow > 0) {
        return s->overflow;
    }

    
    if (s->need_header)
        return qemud_sink_needed(s->header);

    
    return qemud_sink_needed(s->payload);
}

static void
qemud_serial_read( void*  opaque, const uint8_t*  from, int  len )
{
    QemudSerial*  s = opaque;

    T("%s: received %3d bytes: '%s'", __FUNCTION__, len, quote_bytes((const void*)from, len));

    while (len > 0) {
        int  avail;

        
        if (s->overflow > 0) {
            avail = s->overflow;
            if (avail > len)
                avail = len;

            from += avail;
            len  -= avail;
            continue;
        }

        
        if (s->need_header) {
            if (!qemud_sink_fill(s->header, (const uint8_t**)&from, &len))
                break;

#if SUPPORT_LEGACY_QEMUD
            if (s->version == QEMUD_VERSION_UNKNOWN) {
                if ( !memcmp(s->data0, "001200", 6) ) {
                    D("%s: legacy qemud detected.", __FUNCTION__);
                    s->version = QEMUD_VERSION_LEGACY;
                    
                    amodem_set_legacy(android_modem);
                } else {
                    D("%s: normal qemud detected.", __FUNCTION__);
                    s->version = QEMUD_VERSION_NORMAL;
                }
            }

            if (s->version == QEMUD_VERSION_LEGACY) {
                s->in_size     = hex2int( s->data0 + LEGACY_LENGTH_OFFSET,  LENGTH_SIZE );
                s->in_channel  = hex2int( s->data0 + LEGACY_CHANNEL_OFFSET, CHANNEL_SIZE );
            } else {
                s->in_size     = hex2int( s->data0 + LENGTH_OFFSET,  LENGTH_SIZE );
                s->in_channel  = hex2int( s->data0 + CHANNEL_OFFSET, CHANNEL_SIZE );
            }
#else
            
            s->in_size     = hex2int( s->data0 + LENGTH_OFFSET,  LENGTH_SIZE );
            s->in_channel  = hex2int( s->data0 + CHANNEL_OFFSET, CHANNEL_SIZE );
#endif
            s->header->used = 0;

            if (s->in_size <= 0 || s->in_channel < 0) {
                D("%s: bad header: '%.*s'", __FUNCTION__, HEADER_SIZE, s->data0);
                continue;
            }

            if (s->in_size > MAX_SERIAL_PAYLOAD) {
                D("%s: ignoring huge serial packet: length=%d channel=%1",
                  __FUNCTION__, s->in_size, s->in_channel);
                s->overflow = s->in_size;
                continue;
            }

            
            s->need_header = 0;
            qemud_sink_reset(s->payload, s->in_size, s->data0);
        }

        
        if (!qemud_sink_fill(s->payload, &from, &len))
            break;

        
        s->payload->buff[s->payload->size] = 0;
        D("%s: channel=%2d len=%3d '%s'", __FUNCTION__,
          s->in_channel, s->payload->size,
          quote_bytes((const void*)s->payload->buff, s->payload->size));

        s->recv_func( s->recv_opaque, s->in_channel, s->payload->buff, s->payload->size );

        
        s->need_header = 1;
    }
}


#if SUPPORT_LEGACY_QEMUD
static void
qemud_serial_send_legacy_probe( QemudSerial*  s )
{
    uint8_t  tab[194];

    memset(tab, 0, sizeof(tab));
    qemu_chr_write(s->cs, (uint8_t*)"000100X", 7);
    qemu_chr_write(s->cs, (uint8_t*)"000b00connect:gsm", 17);
    qemu_chr_write(s->cs, (uint8_t*)"000b00connect:gps", 17);
    qemu_chr_write(s->cs, (uint8_t*)"000f00connect:control", 21);
    qemu_chr_write(s->cs, (uint8_t*)"00c210", 6);
    qemu_chr_write(s->cs, tab, sizeof(tab));
}
#endif 

static void
qemud_serial_init( QemudSerial*        s,
                   CharDriverState*    cs,
                   QemudSerialReceive  recv_func,
                   void*               recv_opaque )
{
    s->cs           = cs;
    s->recv_func    = recv_func;
    s->recv_opaque  = recv_opaque;
    s->need_header  = 1;
    s->overflow     = 0;

    qemud_sink_reset( s->header, HEADER_SIZE, s->data0 );
    s->in_size      = 0;
    s->in_channel   = -1;

#if SUPPORT_LEGACY_QEMUD
    s->version = QEMUD_VERSION_UNKNOWN;
    qemud_serial_send_legacy_probe(s);
#endif

    qemu_chr_add_handlers( cs,
                           qemud_serial_can_read,
                           qemud_serial_read,
                           NULL,
                           s );
}

static void
qemud_serial_send( QemudSerial*    s,
                   int             channel,
                   ABool           framing,
                   const uint8_t*  msg,
                   int             msglen )
{
    uint8_t   header[HEADER_SIZE];
    uint8_t   frame[FRAME_HEADER_SIZE];
    int       avail, len = msglen;

    if (msglen <= 0 || channel < 0)
        return;

    D("%s: channel=%2d len=%3d '%s'",
      __FUNCTION__, channel, msglen,
      quote_bytes((const void*)msg, msglen));

    if (framing) {
        len += FRAME_HEADER_SIZE;
    }

    
    while (len > 0)
    {
        avail = len;
        if (avail > MAX_SERIAL_PAYLOAD)
            avail = MAX_SERIAL_PAYLOAD;

        
#if SUPPORT_LEGACY_QEMUD
        if (s->version == QEMUD_VERSION_LEGACY) {
            int2hex(header + LEGACY_LENGTH_OFFSET,  LENGTH_SIZE,  avail);
            int2hex(header + LEGACY_CHANNEL_OFFSET, CHANNEL_SIZE, channel);
        } else {
            int2hex(header + LENGTH_OFFSET,  LENGTH_SIZE,  avail);
            int2hex(header + CHANNEL_OFFSET, CHANNEL_SIZE, channel);
        }
#else
        int2hex(header + LENGTH_OFFSET,  LENGTH_SIZE,  avail);
        int2hex(header + CHANNEL_OFFSET, CHANNEL_SIZE, channel);
#endif
        T("%s: '%.*s'", __FUNCTION__, HEADER_SIZE, header);
        qemu_chr_write(s->cs, header, HEADER_SIZE);

        
        if (framing) {
            int2hex(frame, FRAME_HEADER_SIZE, msglen);
            T("%s: '%.*s'", __FUNCTION__, FRAME_HEADER_SIZE, frame);
            qemu_chr_write(s->cs, frame, FRAME_HEADER_SIZE);
            avail  -= FRAME_HEADER_SIZE;
            len    -= FRAME_HEADER_SIZE;
            framing = 0;
        }

        
        T("%s: '%.*s'", __FUNCTION__, avail, msg);
        qemu_chr_write(s->cs, msg, avail);
        msg += avail;
        len -= avail;
    }
}


typedef struct QemudPipeMessage QemudPipeMessage;
struct QemudPipeMessage {
    
    uint8_t*            message;
    
    size_t              size;
    size_t              offset;
    
    QemudPipeMessage*   next;
};



typedef enum QemudProtocol {
    
    QEMUD_PROTOCOL_PIPE,
    
    QEMUD_PROTOCOL_SERIAL
} QemudProtocol;

typedef struct QemudPipe {
    
    void*           hwpipe;
    
    void*           looper;
    
    QemudService*   service;
    
    QemudClient*    client;
} QemudPipe;

struct QemudClient {
    
    QemudProtocol     protocol;

    
    char*             param;
    void*             clie_opaque;
    QemudClientRecv   clie_recv;
    QemudClientClose  clie_close;
    QemudClientSave   clie_save;
    QemudClientLoad   clie_load;
    QemudService*     service;
    QemudClient*      next_serv; 
    QemudClient*      next;
    QemudClient**     pref;

    
    int               framing;
    ABool             need_header;
    ABool             closing;
    QemudSink         header[1];
    uint8_t           header0[FRAME_HEADER_SIZE];
    QemudSink         payload[1];

    
    union {
        
        struct {
            int                 channel;
            QemudSerial*        serial;
        } Serial;
        
        struct {
            QemudPipe*          qemud_pipe;
            QemudPipeMessage*   messages;
        } Pipe;
    } ProtocolSelector;
};

static ABool
_is_pipe_client(QemudClient* client)
{
    return (client-> protocol == QEMUD_PROTOCOL_PIPE) ? true : false;
}

static void  qemud_service_remove_client( QemudService*  service,
                                          QemudClient*   client );

static void
qemud_client_remove( QemudClient*  c )
{
    c->pref[0] = c->next;
    if (c->next)
        c->next->pref = c->pref;

    c->next = NULL;
    c->pref = &c->next;
}

static void
qemud_client_prepend( QemudClient*  c, QemudClient** plist )
{
    c->next = *plist;
    c->pref = plist;
    *plist  = c;
    if (c->next)
        c->next->pref = &c->next;
}

static void
qemud_client_recv( void*  opaque, uint8_t*  msg, int  msglen )
{
    QemudClient*  c = opaque;

    
    if (!c->framing) {
        if (c->clie_recv)
            c->clie_recv( c->clie_opaque, msg, msglen, c );
        return;
    }

    

#if 1
    if (msglen > FRAME_HEADER_SIZE   &&
        c->need_header == 1          &&
        qemud_sink_needed(c->header) == 0)
    {
        int  len = hex2int( msg, FRAME_HEADER_SIZE );

        if (len >= 0 && msglen == len + FRAME_HEADER_SIZE) {
            if (c->clie_recv)
                c->clie_recv( c->clie_opaque,
                              msg+FRAME_HEADER_SIZE,
                              msglen-FRAME_HEADER_SIZE, c );
            return;
        }
    }
#endif

    while (msglen > 0) {
        uint8_t *data;

        
        if (c->need_header) {
            int       frame_size;
            uint8_t*  data;

            if (!qemud_sink_fill(c->header, (const uint8_t**)&msg, &msglen))
                break;

            frame_size = hex2int(c->header0, 4);
            if (frame_size == 0) {
                D("%s: ignoring empty frame", __FUNCTION__);
                continue;
            }
            if (frame_size < 0) {
                D("%s: ignoring corrupted frame header '.*s'",
                  __FUNCTION__, FRAME_HEADER_SIZE, c->header0 );
                continue;
            }

            AARRAY_NEW(data, frame_size+1);  
            qemud_sink_reset(c->payload, frame_size, data);
            c->need_header = 0;
            c->header->used = 0;
        }

        
        if (!qemud_sink_fill(c->payload, (const uint8_t**)&msg, &msglen))
            break;

        c->payload->buff[c->payload->size] = 0;
        c->need_header = 1;
        data = c->payload->buff;

        if (c->clie_recv)
            c->clie_recv( c->clie_opaque, c->payload->buff, c->payload->size, c );

        AFREE(data);
    }
}

static void
_qemud_pipe_send(QemudClient*  client, const uint8_t*  msg, int  msglen);

static void
_qemud_client_free(QemudClient* c)
{
    if ( c != NULL) {
        if (_is_pipe_client(c)) {
            
            QemudPipeMessage** msg_list = &c->ProtocolSelector.Pipe.messages;
            while (*msg_list != NULL) {
                QemudPipeMessage* to_free = *msg_list;
                *msg_list = to_free->next;
                free(to_free);
            }
        }
        if (c->param != NULL) {
            free(c->param);
        }
        AFREE(c);
    }
}

static void
qemud_client_disconnect( void*  opaque, int guest_close )
{
    QemudClient*  c = opaque;

    if (c->closing) {  
        return;
    }

    if (_is_pipe_client(c) && !guest_close) {
        char  tmp[128], *p=tmp, *end=p+sizeof(tmp);
        p = bufprint(tmp, end, "disconnect:00");
        _qemud_pipe_send(c, (uint8_t*)tmp, p-tmp);
        return;
    }

    c->closing = 1;

    
    qemud_client_remove(c);

    if (_is_pipe_client(c)) {
        c->ProtocolSelector.Pipe.qemud_pipe->client = NULL;
    } else if (c->ProtocolSelector.Serial.channel > 0) {
        
        char  tmp[128], *p=tmp, *end=p+sizeof(tmp);
        p = bufprint(tmp, end, "disconnect:%02x",
                     c->ProtocolSelector.Serial.channel);
        qemud_serial_send(c->ProtocolSelector.Serial.serial, 0, 0, (uint8_t*)tmp, p-tmp);
    }

    
    if (c->clie_close) {
        c->clie_close(c->clie_opaque);
        c->clie_close = NULL;
    }
    c->clie_recv = NULL;

    
    if (c->service) {
        qemud_service_remove_client(c->service, c);
        c->service = NULL;
    }

    _qemud_client_free(c);
}

static QemudClient*
qemud_client_alloc( int               channel_id,
                    const char*       client_param,
                    void*             clie_opaque,
                    QemudClientRecv   clie_recv,
                    QemudClientClose  clie_close,
                    QemudClientSave   clie_save,
                    QemudClientLoad   clie_load,
                    QemudSerial*      serial,
                    QemudClient**     pclients )
{
    QemudClient*  c;

    ANEW0(c);

    if (channel_id < 0) {
        
        c->protocol = QEMUD_PROTOCOL_PIPE;
        c->ProtocolSelector.Pipe.messages   = NULL;
        c->ProtocolSelector.Pipe.qemud_pipe = NULL;
    } else {
        
        c->protocol = QEMUD_PROTOCOL_SERIAL;
        c->ProtocolSelector.Serial.serial   = serial;
        c->ProtocolSelector.Serial.channel  = channel_id;
    }
    c->param       = client_param ? ASTRDUP(client_param) : NULL;
    c->clie_opaque = clie_opaque;
    c->clie_recv   = clie_recv;
    c->clie_close  = clie_close;
    c->clie_save   = clie_save;
    c->clie_load   = clie_load;
    c->service     = NULL;
    c->next_serv   = NULL;
    c->next        = NULL;
    c->framing     = 0;
    c->need_header = 1;
    qemud_sink_reset(c->header, FRAME_HEADER_SIZE, c->header0);

    qemud_client_prepend(c, pclients);

    return c;
}

static void  qemud_service_save_name( QEMUFile* f, QemudService* s );
static char* qemud_service_load_name( QEMUFile* f );
static QemudService* qemud_service_find(  QemudService*  service_list,
                                          const char*    service_name );
static QemudClient*  qemud_service_connect_client(  QemudService  *sv,
                                                    int           channel_id,
                                                    const char* client_param);

static void
qemud_serial_client_save(QEMUFile* f, QemudClient* c)
{
    
    qemud_service_save_name(f, c->service);
    qemu_put_string(f, c->param);
    qemu_put_be32(f, c->ProtocolSelector.Serial.channel);

    
    if (c->clie_save)
        c->clie_save(f, c, c->clie_opaque);

    
    qemu_put_be32(f, c->framing);
    if (c->framing) {
        qemu_put_be32(f, c->need_header);
        
        qemu_put_be32(f, FRAME_HEADER_SIZE);
        qemu_put_buffer(f, c->header0, FRAME_HEADER_SIZE);
        
        qemud_sink_save(f, c->payload);
        qemu_put_buffer(f, c->payload->buff, c->payload->size);
    }
}

static int
qemud_serial_client_load(QEMUFile* f, QemudService* current_services, int version )
{
    char *service_name = qemud_service_load_name(f);
    if (service_name == NULL)
        return -EIO;
    char* param = qemu_get_string(f);
    
    QemudService *sv = qemud_service_find(current_services, service_name);
    if (sv == NULL) {
        D("%s: load failed: unknown service \"%s\"\n",
          __FUNCTION__, service_name);
        return -EIO;
    }

    int channel = qemu_get_be32(f);

    if (channel == 0) {
        D("%s: illegal snapshot: client for control channel must no be saved\n",
          __FUNCTION__);
        return -EIO;
    }

    
    QemudClient* c = qemud_service_connect_client(sv, channel, param);
    if(c == NULL)
        return -EIO;

    
    int ret;
    if (c->clie_load)
        if ((ret = c->clie_load(f, c, c->clie_opaque)))
            return ret;  

    
    c->framing = qemu_get_be32(f);
    if (c->framing) {

        
        c->need_header = qemu_get_be32(f);
        int header_size = qemu_get_be32(f);
        if (header_size > FRAME_HEADER_SIZE) {
            D("%s: load failed: payload buffer requires %d bytes, %d available\n",
              __FUNCTION__, header_size, FRAME_HEADER_SIZE);
            return -EIO;
        }
        int ret;
        if ((ret = qemu_get_buffer(f, c->header0, header_size)) != header_size) {
            D("%s: frame header buffer load failed: expected %d bytes, got %d\n",
              __FUNCTION__, header_size, ret);
            return -EIO;
        }

        
        if ((ret = qemud_sink_load(f, c->payload)))
            return ret;

        
        if (c->payload->buff) {
            AFREE(c->payload->buff);
        }
        AARRAY_NEW(c->payload->buff, c->payload->size+1);  
        if ((ret = qemu_get_buffer(f, c->payload->buff, c->payload->size)) != c->payload->size) {
            D("%s: frame payload buffer load failed: expected %d bytes, got %d\n",
              __FUNCTION__, c->payload->size, ret);
            AFREE(c->payload->buff);
            return -EIO;
        }
    }

    return 0;
}



struct QemudService {
    const char*          name;
    int                  max_clients;
    int                  num_clients;
    QemudClient*         clients;
    QemudServiceConnect  serv_connect;
    QemudServiceSave     serv_save;
    QemudServiceLoad     serv_load;
    void*                serv_opaque;
    QemudService*        next;
};

static QemudService*
qemud_service_new( const char*          name,
                   int                  max_clients,
                   void*                serv_opaque,
                   QemudServiceConnect  serv_connect,
                   QemudServiceSave     serv_save,
                   QemudServiceLoad     serv_load,
                   QemudService**       pservices )
{
    QemudService*  s;

    ANEW0(s);
    s->name        = ASTRDUP(name);
    s->max_clients = max_clients;
    s->num_clients = 0;
    s->clients     = NULL;

    s->serv_opaque  = serv_opaque;
    s->serv_connect = serv_connect;
    s->serv_save = serv_save;
    s->serv_load = serv_load;

    s->next    = *pservices;
    *pservices = s;

    return s;
}

static void
qemud_service_add_client( QemudService*  s, QemudClient*  c )
{
    c->service      = s;
    c->next_serv    = s->clients;
    s->clients      = c;
    s->num_clients += 1;
}

static void
qemud_service_remove_client( QemudService*  s, QemudClient*  c )
{
    QemudClient**  pnode = &s->clients;
    QemudClient*   node;

    
    for (;;) {
        node = *pnode;
        if (node == NULL) {
            D("%s: could not find client for service '%s'",
              __FUNCTION__, s->name);
            return;
        }
        if (node == c)
            break;
        pnode = &node->next_serv;
    }

    *pnode          = node->next_serv;
    s->num_clients -= 1;
}

static QemudClient*
qemud_service_connect_client(QemudService *sv,
                             int channel_id,
                             const char* client_param)
{
    QemudClient* client =
        sv->serv_connect( sv->serv_opaque, sv, channel_id, client_param );
    if (client == NULL) {
        D("%s: registration failed for '%s' service",
          __FUNCTION__, sv->name);
        return NULL;
    }
    D("%s: registered client channel %d for '%s' service",
      __FUNCTION__, channel_id, sv->name);
    return client;
}

static QemudService*
qemud_service_find( QemudService*  service_list, const char*  service_name)
{
    QemudService*  sv = NULL;
    for (sv = service_list; sv != NULL; sv = sv->next) {
        if (!strcmp(sv->name, service_name)) {
            break;
        }
    }
    return sv;
}

static void
qemud_service_save_name(QEMUFile* f, QemudService* s)
{
    int len = strlen(s->name) + 1;  
    qemu_put_be32(f, len);
    qemu_put_buffer(f, (const uint8_t *) s->name, len);
}

static char*
qemud_service_load_name( QEMUFile*  f )
{
    int ret;
    int name_len = qemu_get_be32(f);
    char *service_name = android_alloc(name_len);
    if ((ret = qemu_get_buffer(f, (uint8_t*)service_name, name_len) != name_len)) {
        D("%s: service name load failed: expected %d bytes, got %d\n",
          __FUNCTION__, name_len, ret);
        AFREE(service_name);
        return NULL;
    }
    if (service_name[name_len - 1] != '\0') {
        char last = service_name[name_len - 1];
        service_name[name_len - 1] = '\0';  
        D("%s: service name load failed: expecting NULL-terminated string, but "
          "last char is '%c' (buffer contents: '%s%c')\n",
          __FUNCTION__, name_len, last, service_name, last);
        AFREE(service_name);
        return NULL;
    }

    return service_name;
}

static void
qemud_service_save(QEMUFile* f, QemudService* s)
{
    qemud_service_save_name(f, s);
    qemu_put_be32(f, s->max_clients);
    qemu_put_be32(f, s->num_clients);

    if (s->serv_save)
        s->serv_save(f, s, s->serv_opaque);
}

static int
qemud_service_load(  QEMUFile*  f, QemudService*  current_services  )
{
    char* service_name = qemud_service_load_name(f);
    if (service_name == NULL)
        return -EIO;

    
    QemudService *sv = qemud_service_find(current_services, service_name);
    if (sv == NULL) {
        D("%s: loading failed: service \"%s\" not available\n",
          __FUNCTION__, service_name);
        return -EIO;
    }

    
    sv->max_clients = qemu_get_be32(f);
    sv->num_clients = qemu_get_be32(f);

    
    int ret;
    if (sv->serv_load)
        if ((ret = sv->serv_load(f, sv, sv->serv_opaque)))
            return ret;  

    return 0;
}



typedef struct QemudMultiplexer  QemudMultiplexer;

struct QemudMultiplexer {
    QemudSerial    serial[1];
    QemudClient*   clients;
    QemudService*  services;
};

static void
qemud_multiplexer_serial_recv( void*     opaque,
                               int       channel,
                               uint8_t*  msg,
                               int       msglen )
{
    QemudMultiplexer*  m = opaque;
    QemudClient*       c = m->clients;

    for ( ; c != NULL; c = c->next ) {
        if (!_is_pipe_client(c) && c->ProtocolSelector.Serial.channel == channel) {
            qemud_client_recv(c, msg, msglen);
            return;
        }
    }

    D("%s: ignoring %d bytes for unknown channel %d",
      __FUNCTION__, msglen, channel);
}

static int
qemud_multiplexer_connect( QemudMultiplexer*  m,
                           const char*        service_name,
                           int                channel_id )
{
    
    QemudService*  sv = qemud_service_find(m->services, service_name);
    if (sv == NULL) {
        D("%s: no registered '%s' service", __FUNCTION__, service_name);
        return -1;
    }

    
    if (sv->max_clients > 0 && sv->num_clients >= sv->max_clients) {
        D("%s: registration failed for '%s' service: too many clients (%d)",
          __FUNCTION__, service_name, sv->num_clients);
        return -2;
    }

    
    if (qemud_service_connect_client(sv, channel_id, NULL) == NULL)
        return -1;

    return 0;
}

static void
qemud_multiplexer_disconnect( QemudMultiplexer*  m,
                              int                channel )
{
    QemudClient*  c;

    
    for (c = m->clients; c; c = c->next) {
        if (!_is_pipe_client(c) && c->ProtocolSelector.Serial.channel == channel) {
            D("%s: disconnecting client %d",
              __FUNCTION__, channel);
            c->ProtocolSelector.Serial.channel = -1; 
            qemud_client_disconnect(c, 0);
            return;
        }
    }
    D("%s: disconnecting unknown channel %d",
      __FUNCTION__, channel);
}

static void
qemud_multiplexer_disconnect_noncontrol( QemudMultiplexer*  m )
{
    QemudClient* c;
    QemudClient* next = m->clients;

    while (next) {
        c = next;
        next = c->next;  

        if (!_is_pipe_client(c) && c->ProtocolSelector.Serial.channel > 0) {
            
            D("%s: disconnecting client %d",
              __FUNCTION__, c->ProtocolSelector.Serial.channel);
            D("%s: disconnecting client %d\n",
              __FUNCTION__, c->ProtocolSelector.Serial.channel);
            c->ProtocolSelector.Serial.channel = -1; 
            qemud_client_disconnect(c, 0);
        }
    }
}

static void
qemud_multiplexer_control_recv( void*         opaque,
                                uint8_t*      msg,
                                int           msglen,
                                QemudClient*  client )
{
    QemudMultiplexer*  mult   = opaque;
    uint8_t*           msgend = msg + msglen;
    char               tmp[64], *p=tmp, *end=p+sizeof(tmp);

    if (msglen > 8 && !memcmp(msg, "connect:", 8))
    {
        const char*    service_name = (const char*)msg + 8;
        int            channel, ret;
        char*          q;

        q = strchr(service_name, ':');
        if (q == NULL || q+3 != (char*)msgend) {
            D("%s: malformed connect message: '%.*s' (offset=%d)",
              __FUNCTION__, msglen, (const char*)msg, q ? q-(char*)msg : -1);
            return;
        }
        *q++ = 0;  
        channel = hex2int((uint8_t*)q, 2);
        if (channel <= 0) {
            D("%s: malformed channel id '%.*s",
              __FUNCTION__, 2, q);
            return;
        }

        ret = qemud_multiplexer_connect(mult, service_name, channel);
        if (ret < 0) {
            if (ret == -1) {
                
                p = bufprint(tmp, end, "ko:connect:%02x:unknown service", channel);
            } else {
                p = bufprint(tmp, end, "ko:connect:%02x:service busy", channel);
            }
        }
        else {
            p = bufprint(tmp, end, "ok:connect:%02x", channel);
        }
        qemud_serial_send(mult->serial, 0, 0, (uint8_t*)tmp, p-tmp);
        return;
    }

    if (msglen == 13 && !memcmp(msg, "disconnect:", 11)) {
        int  channel_id = hex2int(msg+11, 2);
        if (channel_id <= 0) {
            D("%s: malformed disconnect channel id: '%.*s'",
              __FUNCTION__, 2, msg+11);
            return;
        }
        qemud_multiplexer_disconnect(mult, channel_id);
        return;
    }

#if SUPPORT_LEGACY_QEMUD
    if (msglen > 11 && !memcmp(msg, "ok:connect:", 11)) {
        const char*  service_name = (const char*)msg + 11;
        char*        q            = strchr(service_name, ':');
        int          channel;

        if (q == NULL || q+3 != (char*)msgend) {
            D("%s: malformed legacy connect message: '%.*s' (offset=%d)",
              __FUNCTION__, msglen, (const char*)msg, q ? q-(char*)msg : -1);
            return;
        }
        *q++ = 0;  
        channel = hex2int((uint8_t*)q, 2);
        if (channel <= 0) {
            D("%s: malformed legacy channel id '%.*s",
              __FUNCTION__, 2, q);
            return;
        }

        switch (mult->serial->version) {
        case QEMUD_VERSION_UNKNOWN:
            mult->serial->version = QEMUD_VERSION_LEGACY;
            D("%s: legacy qemud daemon detected.", __FUNCTION__);
            break;

        case QEMUD_VERSION_LEGACY:
            
            break;

        default:
            D("%s: weird, ignoring legacy qemud control message: '%.*s'",
              __FUNCTION__, msglen, msg);
            return;
        }

        
        if (!strcmp(service_name,"control"))
            service_name = "hw-control";

        qemud_multiplexer_connect(mult, service_name, channel);
        return;
    }

    
    if (mult->serial->version == QEMUD_VERSION_LEGACY)
        return;
#endif 

    
    p = bufprint(tmp, end, "ko:unknown command");
    qemud_serial_send(mult->serial, 0, 0, (uint8_t*)tmp, p-tmp);
}

static void
qemud_multiplexer_init( QemudMultiplexer*  mult,
                        CharDriverState*   serial_cs )
{
    QemudClient*  control;

    
    qemud_serial_init( mult->serial,
                       serial_cs,
                       qemud_multiplexer_serial_recv,
                       mult );

    
    control = qemud_client_alloc( 0,
                                  NULL,
                                  mult,
                                  qemud_multiplexer_control_recv,
                                  NULL, NULL, NULL,
                                  mult->serial,
                                  &mult->clients );
}

static QemudMultiplexer  _multiplexer[1];


QemudClient*
qemud_client_new( QemudService*     service,
                  int               channelId,
                  const char*       client_param,
                  void*             clie_opaque,
                  QemudClientRecv   clie_recv,
                  QemudClientClose  clie_close,
                  QemudClientSave   clie_save,
                  QemudClientLoad   clie_load )
{
    QemudMultiplexer*  m = _multiplexer;
    QemudClient*       c = qemud_client_alloc( channelId,
                                               client_param,
                                               clie_opaque,
                                               clie_recv,
                                               clie_close,
                                               clie_save,
                                               clie_load,
                                               m->serial,
                                               &m->clients );

    qemud_service_add_client(service, c);
    return c;
}

static void
_qemud_pipe_cache_buffer(QemudClient* client, const uint8_t*  msg, int  msglen)
{
    QemudPipeMessage* buf;
    QemudPipeMessage** ins_at = &client->ProtocolSelector.Pipe.messages;

    
    buf = (QemudPipeMessage*)malloc(msglen + sizeof(QemudPipeMessage));
    if (buf != NULL) {
        
        buf->message = (uint8_t*)buf + sizeof(QemudPipeMessage);
        buf->size = msglen;
        memcpy(buf->message, msg, msglen);
        buf->offset = 0;
        buf->next = NULL;
        while (*ins_at != NULL) {
            ins_at = &(*ins_at)->next;
        }
        *ins_at = buf;
        
        goldfish_pipe_wake(client->ProtocolSelector.Pipe.qemud_pipe->hwpipe,
                           PIPE_WAKE_READ);
    }
}

static void
_qemud_pipe_send(QemudClient*  client, const uint8_t*  msg, int  msglen)
{
    uint8_t   frame[FRAME_HEADER_SIZE];
    int       avail, len = msglen;
    int framing = client->framing;

    if (msglen <= 0)
        return;

    D("%s: len=%3d '%s'",
      __FUNCTION__, msglen, quote_bytes((const void*)msg, msglen));

    if (framing) {
        len += FRAME_HEADER_SIZE;
    }

    
    while (len > 0)
    {
        avail = len;
        if (avail > MAX_SERIAL_PAYLOAD)
            avail = MAX_SERIAL_PAYLOAD;

        
        if (framing) {
            int2hex(frame, FRAME_HEADER_SIZE, msglen);
            T("%s: '%.*s'", __FUNCTION__, FRAME_HEADER_SIZE, frame);
            _qemud_pipe_cache_buffer(client, frame, FRAME_HEADER_SIZE);
            avail  -= FRAME_HEADER_SIZE;
            len    -= FRAME_HEADER_SIZE;
            framing = 0;
        }

        
        T("%s: '%.*s'", __FUNCTION__, avail, msg);
        _qemud_pipe_cache_buffer(client, msg, avail);
        msg += avail;
        len -= avail;
    }
}

void
qemud_client_send ( QemudClient*  client, const uint8_t*  msg, int  msglen )
{
    if (_is_pipe_client(client)) {
        _qemud_pipe_send(client, msg, msglen);
    } else {
        qemud_serial_send(client->ProtocolSelector.Serial.serial,
                          client->ProtocolSelector.Serial.channel,
                          client->framing != 0, msg, msglen);
    }
}

void
qemud_client_set_framing( QemudClient*  client, int  framing )
{
    
    if (client->framing) {
        if (!client->need_header) {
            AFREE(client->payload->buff);
            client->need_header = 1;
        }
    }
    client->framing = !!framing;
}

void
qemud_client_close( QemudClient*  client )
{
    qemud_client_disconnect(client, 0);
}



static void
qemud_client_save_count(QEMUFile* f, QemudClient* c)
{
    unsigned int client_count = 0;
    for( ; c; c = c->next)   
        if (!_is_pipe_client(c) && c->ProtocolSelector.Serial.channel > 0)
            client_count++;

    qemu_put_be32(f, client_count);
}

static void
qemud_service_save_count(QEMUFile* f, QemudService* s)
{
    unsigned int service_count = 0;
    for( ; s; s = s->next )  
        service_count++;

    qemu_put_be32(f, service_count);
}

static void
qemud_save(QEMUFile* f, void* opaque)
{
    QemudMultiplexer *m = opaque;

    qemud_serial_save(f, m->serial);

    
    qemud_service_save_count(f, m->services);
    QemudService *s;
    for (s = m->services; s; s = s->next)
        qemud_service_save(f, s);

    
    qemud_client_save_count(f, m->clients);
    QemudClient *c;
    for (c = m->clients; c; c = c->next) {
        
        if (!_is_pipe_client(c) && c->ProtocolSelector.Serial.channel > 0) {
            qemud_serial_client_save(f, c);
        }
    }

}


static int
qemud_load_services( QEMUFile*  f, QemudService*  current_services )
{
    int i, ret;
    int service_count = qemu_get_be32(f);
    for (i = 0; i < service_count; i++) {
        if ((ret = qemud_service_load(f, current_services)))
            return ret;
    }

    return 0;
}

static int
qemud_load_clients(QEMUFile* f, QemudMultiplexer* m, int version )
{
    
    qemud_multiplexer_disconnect_noncontrol(m);

    
    int client_count = qemu_get_be32(f);
    int i, ret;
    for (i = 0; i < client_count; i++) {
        if ((ret = qemud_serial_client_load(f, m->services, version))) {
            return ret;
        }
    }

    return 0;
}

static int
qemud_load(QEMUFile *f, void* opaque, int version)
{
    QemudMultiplexer *m = opaque;

    int ret;

    if ((ret = qemud_serial_load(f, m->serial)))
        return ret;
    if ((ret = qemud_load_services(f, m->services)))
        return ret;
    if ((ret = qemud_load_clients(f, m, version)))
        return ret;

    return 0;
}


static void
_save_pipe_message(QEMUFile* f, QemudPipeMessage* msg)
{
    qemu_put_be32(f, msg->size);
    qemu_put_be32(f, msg->offset);
    qemu_put_buffer(f, msg->message, msg->size);
}

static QemudPipeMessage*
_load_pipe_message(QEMUFile* f)
{
    QemudPipeMessage* ret = NULL;
    QemudPipeMessage** next = &ret;

    uint32_t size = qemu_get_be32(f);
    while (size != 0) {
        QemudPipeMessage* wrk;
        ANEW0(wrk);
        *next = wrk;
        wrk->size = size;
        wrk->offset = qemu_get_be32(f);
        wrk->message = malloc(wrk->size);
        if (wrk->message == NULL) {
            APANIC("Unable to allocate buffer for pipe's pending message.");
        }
        qemu_get_buffer(f, wrk->message, wrk->size);
        next = &wrk->next;
        *next = NULL;
        size = qemu_get_be32(f);
    }

    return ret;
}

static void*
_qemudPipe_init(void* hwpipe, void* _looper, const char* args)
{
    QemudMultiplexer *m = _multiplexer;
    QemudService* sv = m->services;
    QemudClient* client;
    QemudPipe* pipe = NULL;
    char service_name[512];
    const char* client_args;
    size_t srv_name_len;

    if (args == NULL) {
        D("%s: Missing address!", __FUNCTION__);
        return NULL;
    }

    client_args = strchr(args, ':');
    if (client_args != NULL) {
        srv_name_len = min(client_args - args, sizeof(service_name) - 1);
        client_args++;  
        if (*client_args == '\0') {
            
            client_args = NULL;
        }
    } else {
        srv_name_len = min(strlen(args), sizeof(service_name) - 1);
    }
    memcpy(service_name, args, srv_name_len);
    service_name[srv_name_len] = '\0';

    
    while (sv != NULL && strcmp(sv->name, service_name)) {
        sv = sv->next;
    }
    if (sv == NULL) {
        D("%s: Service '%s' has not been registered!", __FUNCTION__, service_name);
        return NULL;
    }

    client = qemud_service_connect_client(sv, -1, client_args);
    if (client != NULL) {
        ANEW0(pipe);
        pipe->hwpipe = hwpipe;
        pipe->looper = _looper;
        pipe->service = sv;
        pipe->client = client;
        client->ProtocolSelector.Pipe.qemud_pipe = pipe;
    }

    return pipe;
}

static void
_qemudPipe_closeFromGuest( void* opaque )
{
    QemudPipe* pipe = opaque;
    QemudClient*  client = pipe->client;
    D("%s", __FUNCTION__);
    if (client != NULL) {
        qemud_client_disconnect(client, 1);
    } else {
        D("%s: Unexpected NULL client", __FUNCTION__);
    }
}

static int
_qemudPipe_sendBuffers(void* opaque,
                       const GoldfishPipeBuffer* buffers,
                       int numBuffers)
{
    QemudPipe* pipe = opaque;
    QemudClient*  client = pipe->client;
    size_t transferred = 0;

    if (client == NULL) {
        D("%s: Unexpected NULL client", __FUNCTION__);
        return -1;
    }

    if (numBuffers == 1) {
        
        D("%s: %s", __FUNCTION__, quote_bytes((char*)buffers->data, buffers->size));
        qemud_client_recv(client, buffers->data, buffers->size);
        transferred = buffers->size;
    } else {
        uint8_t* msg, *wrk;
        int n;
        for (n = 0; n < numBuffers; n++) {
            transferred += buffers[n].size;
        }
        msg = malloc(transferred);
        wrk = msg;
        for (n = 0; n < numBuffers; n++) {
            memcpy(wrk, buffers[n].data, buffers[n].size);
            wrk += buffers[n].size;
        }
        D("%s: %s", __FUNCTION__, quote_bytes((char*)msg, transferred));
        qemud_client_recv(client, msg, transferred);
        free(msg);
    }

    return transferred;
}

static int
_qemudPipe_recvBuffers(void* opaque, GoldfishPipeBuffer* buffers, int numBuffers)
{
    QemudPipe* pipe = opaque;
    QemudClient*  client = pipe->client;
    QemudPipeMessage** msg_list;
    GoldfishPipeBuffer* buff = buffers;
    GoldfishPipeBuffer* endbuff = buffers + numBuffers;
    size_t sent_bytes = 0;
    size_t off_in_buff = 0;

    if (client == NULL) {
        D("%s: Unexpected NULL client", __FUNCTION__);
        return -1;
    }

    msg_list = &client->ProtocolSelector.Pipe.messages;
    if (*msg_list == NULL) {
        return PIPE_ERROR_AGAIN;
    }

    while (buff != endbuff && *msg_list != NULL) {
        QemudPipeMessage* msg = *msg_list;
        
        size_t to_copy = min(msg->size - msg->offset, buff->size - off_in_buff);
        memcpy(buff->data + off_in_buff, msg->message + msg->offset, to_copy);
        
        off_in_buff += to_copy;
        msg->offset += to_copy;
        sent_bytes += to_copy;
        if (msg->size == msg->offset) {
            
            *msg_list = msg->next;
            free(msg);
        }
        if (off_in_buff == buff->size) {
            
            buff++;
            off_in_buff = 0;
        }
    }

    D("%s: -> %u (of %u)", __FUNCTION__, sent_bytes, buffers->size);

    return sent_bytes;
}

static unsigned
_qemudPipe_poll(void* opaque)
{
    QemudPipe* pipe = opaque;
    QemudClient*  client = pipe->client;
    unsigned ret = 0;

    if (client != NULL) {
        ret |= PIPE_POLL_OUT;
        if (client->ProtocolSelector.Pipe.messages != NULL) {
            ret |= PIPE_POLL_IN;
        }
    } else {
        D("%s: Unexpected NULL client", __FUNCTION__);
    }

    return ret;
}

static void
_qemudPipe_wakeOn(void* opaque, int flags)
{
    D("%s: -> %X", __FUNCTION__, flags);
}

static void
_qemudPipe_save(void* opaque, QEMUFile* f )
{
    QemudPipe* qemud_pipe = (QemudPipe*)opaque;
    QemudClient* c = qemud_pipe->client;
    QemudPipeMessage* msg = c->ProtocolSelector.Pipe.messages;

    
    qemud_service_save_name(f, c->service);
    qemu_put_string(f, c->param);

    
    while (msg != NULL) {
        _save_pipe_message(f, msg);
        msg = msg->next;
    }
    
    qemu_put_be32(f, 0);

    
    if (c->clie_save)
        c->clie_save(f, c, c->clie_opaque);

    
    qemu_put_be32(f, c->framing);
    if (c->framing) {
        qemu_put_be32(f, c->need_header);
        
        qemu_put_be32(f, FRAME_HEADER_SIZE);
        qemu_put_buffer(f, c->header0, FRAME_HEADER_SIZE);
        
        qemud_sink_save(f, c->payload);
        qemu_put_buffer(f, c->payload->buff, c->payload->size);
    }
}

static void*
_qemudPipe_load(void* hwpipe, void* pipeOpaque, const char* args, QEMUFile* f)
{
    QemudPipe* qemud_pipe = NULL;
    char* param;
    char *service_name = qemud_service_load_name(f);
    if (service_name == NULL)
        return NULL;
    
    QemudService *sv = qemud_service_find(_multiplexer->services, service_name);
    if (sv == NULL) {
        D("%s: load failed: unknown service \"%s\"\n",
          __FUNCTION__, service_name);
        return NULL;
    }

    
    param = qemu_get_string(f);

    
    QemudClient* c = qemud_service_connect_client(sv, -1, param);
    if(c == NULL)
        return NULL;

    
    c->ProtocolSelector.Pipe.messages = _load_pipe_message(f);

    
    if (c->clie_load && c->clie_load(f, c, c->clie_opaque)) {
        
        return NULL;
    }

    
    c->framing = qemu_get_be32(f);
    if (c->framing) {

        
        c->need_header = qemu_get_be32(f);
        int header_size = qemu_get_be32(f);
        if (header_size > FRAME_HEADER_SIZE) {
            D("%s: load failed: payload buffer requires %d bytes, %d available\n",
              __FUNCTION__, header_size, FRAME_HEADER_SIZE);
            return NULL;
        }
        int ret;
        if ((ret = qemu_get_buffer(f, c->header0, header_size)) != header_size) {
            D("%s: frame header buffer load failed: expected %d bytes, got %d\n",
              __FUNCTION__, header_size, ret);
            return NULL;
        }

        
        if ((ret = qemud_sink_load(f, c->payload)))
            return NULL;

        
        if (c->payload->buff) {
            AFREE(c->payload->buff);
        }
        AARRAY_NEW(c->payload->buff, c->payload->size+1);  
        if ((ret = qemu_get_buffer(f, c->payload->buff, c->payload->size)) != c->payload->size) {
            D("%s: frame payload buffer load failed: expected %d bytes, got %d\n",
              __FUNCTION__, c->payload->size, ret);
            AFREE(c->payload->buff);
            return NULL;
        }
    }

    
    ANEW0(qemud_pipe);
    qemud_pipe->hwpipe = hwpipe;
    qemud_pipe->looper = pipeOpaque;
    qemud_pipe->service = sv;
    qemud_pipe->client = c;
    c->ProtocolSelector.Pipe.qemud_pipe = qemud_pipe;

    return qemud_pipe;
}

static const GoldfishPipeFuncs _qemudPipe_funcs = {
    _qemudPipe_init,
    _qemudPipe_closeFromGuest,
    _qemudPipe_sendBuffers,
    _qemudPipe_recvBuffers,
    _qemudPipe_poll,
    _qemudPipe_wakeOn,
    _qemudPipe_save,
    _qemudPipe_load,
};

static void
_android_qemud_pipe_init(void)
{
    static ABool _qemud_pipe_initialized = false;

    if (!_qemud_pipe_initialized) {
        goldfish_pipe_add_type( "qemud", looper_newCore(), &_qemudPipe_funcs );
        _qemud_pipe_initialized = true;
    }
}

static CharDriverState*  android_qemud_cs;

static void
_android_qemud_serial_init(void)
{
    CharDriverState*    cs;

    if (android_qemud_cs != NULL)
        return;

    if (qemu_chr_open_charpipe( &android_qemud_cs, &cs ) < 0) {
        derror( "%s: can't create charpipe to serial port",
                __FUNCTION__ );
        exit(1);
    }

    qemud_multiplexer_init(_multiplexer, cs);

    register_savevm( "qemud", 0, QEMUD_SAVE_VERSION,
                      qemud_save, qemud_load, _multiplexer);
}

extern void
android_qemud_init( void )
{
    D("%s", __FUNCTION__);
    _android_qemud_serial_init();
    _android_qemud_pipe_init();
}

CharDriverState*  android_qemud_get_cs( void )
{
    if (android_qemud_cs == NULL)
        android_qemud_init();

    return android_qemud_cs;
}

QemudService*
qemud_service_register( const char*          service_name,
                        int                  max_clients,
                        void*                serv_opaque,
                        QemudServiceConnect  serv_connect,
                        QemudServiceSave     serv_save,
                        QemudServiceLoad     serv_load )
{
    QemudService*      sv;
    QemudMultiplexer*  m  = _multiplexer;

    android_qemud_init();

    sv = qemud_service_new(service_name,
                           max_clients,
                           serv_opaque,
                           serv_connect,
                           serv_save,
                           serv_load,
                           &m->services);
    D("Registered QEMUD service %s", service_name);
    return sv;
}

extern void
qemud_service_broadcast( QemudService*  sv,
                         const uint8_t*  msg,
                         int             msglen )
{
    QemudClient*  c;

    for (c = sv->clients; c; c = c->next_serv)
        qemud_client_send(c, msg, msglen);
}




typedef struct {
    QemudService*     service;
    CharDriverState*  cs;
} QemudCharService;

static void
_qemud_char_client_recv( void*  opaque, uint8_t*  msg, int  msglen,
                         QemudClient*  client )
{
    CharDriverState*  cs = opaque;
    qemu_chr_write(cs, msg, msglen);
}

static void
_qemud_char_client_close( void*  opaque )

{
    QemudClient* client = opaque;

    if (!_is_pipe_client(client)) {
        derror("unexpected qemud char. channel close");
    }
}


static int
_qemud_char_service_can_read( void*  opaque )
{
    return 8192;  
}

static void
_qemud_char_service_read( void*  opaque, const uint8_t*  from, int  len )
{
    QemudService*  sv = opaque;
    qemud_service_broadcast( sv, from, len );
}

static QemudClient*
_qemud_char_service_connect(void*          opaque,
                            QemudService*  sv,
                            int            channel,
                            const char*    client_param )
{
    CharDriverState*   cs = opaque;
    QemudClient*       c  = qemud_client_new( sv, channel, client_param,
                                              cs,
                                              _qemud_char_client_recv,
                                              _qemud_char_client_close,
                                              NULL, NULL );

    
    qemu_chr_add_handlers( cs,
                           _qemud_char_service_can_read,
                           _qemud_char_service_read,
                           NULL,
                           sv );

    return c;
}

int
android_qemud_get_channel( const char*  name, CharDriverState* *pcs )
{
    CharDriverState*   cs;

    if (qemu_chr_open_charpipe(&cs, pcs) < 0) {
        derror("can't open charpipe for '%s' qemud service", name);
        exit(2);
    }
    qemud_service_register(name, 1, cs, _qemud_char_service_connect, NULL, NULL);
    return 0;
}

int
android_qemud_set_channel( const char*  name, CharDriverState*  peer_cs )
{
    CharDriverState*  char_buffer = qemu_chr_open_buffer(peer_cs);

    if (char_buffer == NULL)
        return -1;

    qemud_service_register(name, 1, char_buffer, _qemud_char_service_connect,
                           NULL, NULL);
    return 0;
}

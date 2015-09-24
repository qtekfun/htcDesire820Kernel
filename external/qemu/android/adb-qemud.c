/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "qemu-common.h"
#include "android/globals.h"  
#include "android/hw-qemud.h"
#include "android/utils/misc.h"
#include "android/utils/system.h"
#include "android/utils/debug.h"
#include "android/adb-server.h"
#include "android/adb-qemud.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(adbclient,__VA_ARGS__)
#define  DD(...)   VERBOSE_PRINT(adb,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(adbclient)
#define  DD_ACTIVE VERBOSE_CHECK(adb)
#define  QB(b, s)  quote_bytes((const char*)b, (s < 32) ? s : 32)

#define SERVICE_NAME        "adb"
#define DEBUG_SERVICE_NAME  "adb-debug"
#define ADB_MAX_MSG_LEN     8
typedef enum AdbClientState {
    
    ADBC_STATE_WAIT_ON_HOST,
    ADBC_STATE_HOST_CONNECTED,
    
    ADBC_STATE_CONNECTED,
    
    ADBC_STATE_HOST_DISCONNECTED,
    
    ADBC_STATE_GUEST_DISCONNECTED,
} AdbClientState;

typedef struct AdbClient AdbClient;
struct AdbClient {
    
    void*           opaque;
    
    QemudClient*    qemud_client;
    
    AdbClientState  state;
    
    char            msg_buffer[ADB_MAX_MSG_LEN];
    
    int             msg_cur;
};

typedef struct AdbDbgClient AdbDbgClient;
struct AdbDbgClient {
    
    QemudClient*    qemud_client;
};


static void
_adb_on_host_connected(void* opaque, void* connection)
{
    AdbClient* const adb_client = (AdbClient*)opaque;

    if (adb_client->state == ADBC_STATE_WAIT_ON_HOST) {
        D("ADB client %p(o=%p) is connected to the host %p",
          adb_client, adb_client->opaque, connection);

        
         adb_client->state = ADBC_STATE_HOST_CONNECTED;

        qemud_client_send(adb_client->qemud_client, (const uint8_t*)"ok", 2);
    } else {
        D("Unexpected ADB host connection while state is %d", adb_client->state);
    }
}

static void
_adb_on_host_disconnect(void* opaque, void* connection)
{
    AdbClient* const adb_client = (AdbClient*)opaque;

    D("ADB client %p(o=%p) is disconnected from the host %p",
      adb_client, adb_client->opaque, connection);
    adb_client->state = ADBC_STATE_HOST_DISCONNECTED;
}

static void
_adb_on_host_data(void* opaque, void* connection, const void* buff, int size)
{
    AdbClient* const adb_client = (AdbClient*)opaque;
    D("ADB client %p(o=%p) received from the host %p %d bytes in %s",
      adb_client, adb_client->opaque, connection, size, QB(buff, size));

    if (adb_client->state == ADBC_STATE_CONNECTED) {
        
        qemud_client_send(adb_client->qemud_client, (const uint8_t*)buff, size);
    } else {
        D("Unexpected data from ADB host %p while client %p(o=%p) is in state %d",
          connection, adb_client, adb_client->opaque, adb_client->state);
    }
}

static AdbGuestRoutines _adb_client_routines = {
    
    _adb_on_host_connected,
    
    _adb_on_host_disconnect,
    
    _adb_on_host_data,
};


static AdbClient*
_adb_client_new(void)
{
    AdbClient* adb_client;

    ANEW0(adb_client);

    return adb_client;
}

static void
_adb_client_free(AdbClient* adb_client)
{
    if (adb_client != NULL) {
        free(adb_client);
    }
}

static void
_adb_client_recv(void* opaque, uint8_t* msg, int msglen, QemudClient* client)
{
    AdbClient* const adb_client = (AdbClient*)opaque;

    D("ADB client %p(o=%p) received from guest %d bytes in %s",
      adb_client, adb_client->opaque, msglen, QB(msg, msglen));

    if (adb_client->state == ADBC_STATE_CONNECTED) {
        
        adb_server_on_guest_message(adb_client->opaque, msg, msglen);
        return;
    }


    
    if ((msglen + adb_client->msg_cur) > sizeof(adb_client->msg_buffer)) {
        D("Unexpected message in ADB client.");
        adb_client->msg_cur = 0;
        return;
    }
    
    memcpy(adb_client->msg_buffer + adb_client->msg_cur, msg, msglen);
    adb_client->msg_cur += msglen;

    
    switch (adb_client->state) {
        case ADBC_STATE_WAIT_ON_HOST:
            
            if (adb_client->msg_cur == 6 &&
                !memcmp(adb_client->msg_buffer, "accept", 6)) {
                adb_client->msg_cur = 0;
                
                adb_client->opaque =
                    adb_server_register_guest(adb_client, &_adb_client_routines);
                if (adb_client->opaque == NULL) {
                    D("Unable to register ADB guest with the ADB server.");
                    
                    qemud_client_send(adb_client->qemud_client,
                                      (const uint8_t*)"ko", 2);
                }
            } else {
                D("Unexpected guest request while waiting on ADB host to connect.");
            }
            break;

        case ADBC_STATE_HOST_CONNECTED:
            
            if (adb_client->msg_cur &&
                !memcmp(adb_client->msg_buffer, "start", 5)) {
                adb_client->msg_cur = 0;
                adb_client->state = ADBC_STATE_CONNECTED;
                adb_server_complete_connection(adb_client->opaque);
            } else {
                D("Unexpected request while waiting on connection to start.");
            }
            break;

        default:
            D("Unexpected ADB guest request '%s' while client state is %d.",
              QB(msg, msglen), adb_client->state);
            break;
    }
}

static void
_adb_client_close(void* opaque)
{
    AdbClient* const adb_client = (AdbClient*)opaque;

    D("ADB client %p(o=%p) is disconnected from the guest.",
      adb_client, adb_client->opaque);
    adb_client->state = ADBC_STATE_GUEST_DISCONNECTED;
    if (adb_client->opaque != NULL) {
        
        adb_server_on_guest_closed(adb_client->opaque);
    }
    _adb_client_free(adb_client);
}

static QemudClient*
_adb_service_connect(void*          opaque,
                     QemudService*  serv,
                     int            channel,
                     const char*    client_param)
{
    
    AdbClient* const adb_client = _adb_client_new();

    D("Connecting ADB guest: '%s'", client_param ? client_param : "<null>");
    adb_client->qemud_client =
        qemud_client_new(serv, channel, client_param, adb_client,
                         _adb_client_recv, _adb_client_close, NULL, NULL);
    if (adb_client->qemud_client == NULL) {
        D("Unable to create QEMUD client for ADB guest.");
        _adb_client_free(adb_client);
        return NULL;
    }

    return adb_client->qemud_client;
}


static AdbDbgClient*
_adb_dbg_client_new(void)
{
    AdbDbgClient* adb_dbg_client;

    ANEW0(adb_dbg_client);

    return adb_dbg_client;
}

static void
_adb_dbg_client_free(AdbDbgClient* adb_dbg_client)
{
    if (adb_dbg_client != NULL) {
        free(adb_dbg_client);
    }
}

static void
_adb_dbg_client_recv(void* opaque, uint8_t* msg, int msglen, QemudClient* client)
{
    if (DD_ACTIVE) {
        fprintf(stderr, "ADB: %s", (const char*)msg);
    }
}

static void
_adb_dbg_client_close(void* opaque)
{
    AdbDbgClient* const adb_dbg_client = (AdbDbgClient*)opaque;

    DD("ADB debugging client %p is disconnected from the guest.", adb_dbg_client);
    _adb_dbg_client_free(adb_dbg_client);
}

static QemudClient*
_adb_debug_service_connect(void*          opaque,
                           QemudService*  serv,
                           int            channel,
                           const char*    client_param)
{
    
    AdbDbgClient* const adb_dbg_client = _adb_dbg_client_new();

    DD("Connecting ADB debugging guest: '%s'",
       client_param ? client_param : "<null>");
    adb_dbg_client->qemud_client =
        qemud_client_new(serv, channel, client_param, adb_dbg_client,
                         _adb_dbg_client_recv, _adb_dbg_client_close, NULL, NULL);
    if (adb_dbg_client->qemud_client == NULL) {
        DD("Unable to create QEMUD client for ADB debugging guest.");
        _adb_dbg_client_free(adb_dbg_client);
        return NULL;
    }

    return adb_dbg_client->qemud_client;
}


void
android_adb_service_init(void)
{
static int _inited = 0;

    if (!adb_server_is_initialized()) {
        return;
    }

    if (!_inited) {
        
        QemudService*  serv = qemud_service_register(SERVICE_NAME, 0, NULL,
                                                     _adb_service_connect,
                                                     NULL, NULL);
        if (serv == NULL) {
            derror("%s: Could not register '%s' service",
                   __FUNCTION__, SERVICE_NAME);
            return;
        }
        D("%s: Registered '%s' qemud service", __FUNCTION__, SERVICE_NAME);

        
        serv = qemud_service_register(DEBUG_SERVICE_NAME, 0, NULL,
                                      _adb_debug_service_connect, NULL, NULL);
        if (serv != NULL) {
            DD("Registered '%s' qemud service", DEBUG_SERVICE_NAME);
        } else {
            dwarning("%s: Could not register '%s' service",
                   __FUNCTION__, DEBUG_SERVICE_NAME);
        }
    }
}

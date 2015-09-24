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
#include "sockets.h"
#include "iolooper.h"
#include "android/async-utils.h"
#include "android/utils/debug.h"
#include "android/utils/list.h"
#include "android/utils/misc.h"
#include "android/adb-server.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(adbserver,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(adbserver)
#define  QB(b, s)  quote_bytes((const char*)b, (s < 32) ? s : 32)

typedef struct AdbServer    AdbServer;
typedef struct AdbHost      AdbHost;
typedef struct AdbGuest     AdbGuest;

struct AdbGuest {
    ACList              list_entry;
    
    void*               opaque;
    
    AdbServer*          adb_srv;
    
    AdbHost*            adb_host;
    
    AdbGuestRoutines*   callbacks;
    int                 is_connected;
};

struct AdbHost {
    ACList      list_entry;
    
    AdbServer*  adb_srv;
    
    int         host_so;
    
    LoopIo      io[1];
    
    AdbGuest*   adb_guest;
    
    uint8_t*    pending_data;
    
    int         pending_data_size;
    
    uint8_t*    pending_send_buffer;
    
    int         pending_send_data_size;
    
    int         pending_send_buffer_size;
};

struct AdbServer {
    
    SockAddress socket_address;
    
    Looper*     looper;
    
    LoopIo      io[1];
    
    int         port;
    
    int         so;
    
    ACList      adb_hosts;
    
    ACList      adb_guests;
    
    ACList      pending_hosts;
    
    ACList      pending_guests;
};

static AdbServer    _adb_server;
static int          _adb_server_initialized = 0;


static AdbHost*
_adb_host_new(AdbServer* adb_srv)
{
    AdbHost* adb_host;

    ANEW0(adb_host);
    alist_init(&adb_host->list_entry);
    adb_host->adb_srv = adb_srv;
    adb_host->host_so = -1;

    return adb_host;
}

static void
_adb_host_free(AdbHost* adb_host)
{
    if (adb_host != NULL) {
        
        assert(alist_is_empty(&adb_host->list_entry));

        
        if (adb_host->host_so >= 0) {
            loopIo_done(adb_host->io);
            socket_close(adb_host->host_so);
        }

        
        if (adb_host->pending_data != NULL) {
            free(adb_host->pending_data);
        }
        if (adb_host->pending_send_buffer != NULL) {
            free(adb_host->pending_send_buffer);
        }

        AFREE(adb_host);
    }
}

static void
_adb_host_append_message(AdbHost* adb_host, const void* msg, int msglen)
{
    printf("Append %d bytes to ADB host buffer.\n", msglen);

    
    if (adb_host->pending_send_buffer == NULL) {
        adb_host->pending_send_buffer = (uint8_t*)malloc(msglen);
        adb_host->pending_send_buffer_size = msglen;
    } else if ((adb_host->pending_send_data_size + msglen) >
               adb_host->pending_send_buffer_size) {
        adb_host->pending_send_buffer =
            (uint8_t*)realloc(adb_host->pending_send_buffer,
                              adb_host->pending_send_data_size + msglen);
        adb_host->pending_send_buffer_size =
            adb_host->pending_send_data_size + msglen;
    }

    if (adb_host->pending_send_buffer == NULL) {
        D("Unable to allocate %d bytes for pending ADB host data.",
          adb_host->pending_send_data_size + msglen);
        adb_host->pending_send_buffer_size = adb_host->pending_send_data_size = 0;
        loopIo_dontWantWrite(adb_host->io);
        return;
    }

    memcpy(adb_host->pending_send_buffer + adb_host->pending_send_data_size,
           msg, msglen);
    loopIo_wantWrite(adb_host->io);
}

static void
_adb_connect(AdbHost* adb_host, AdbGuest* adb_guest)
{
    D("Connecting ADB host %p(so=%d) with ADB guest %p(o=%p)",
      adb_host, adb_host->host_so, adb_guest, adb_guest->opaque);

    adb_guest->adb_host = adb_host;
    adb_host->adb_guest = adb_guest;
    adb_guest->callbacks->on_connected(adb_guest->opaque, adb_guest);
}

static void
_on_adb_host_disconnected(AdbHost* adb_host)
{
    AdbGuest* const adb_guest = adb_host->adb_guest;

    
    if (adb_guest != NULL) {
        D("Disconnecting ADB host %p(so=%d) from ADB guest %p(o=%p)",
          adb_host, adb_host->host_so, adb_guest, adb_guest->opaque);
        adb_host->adb_guest = NULL;
        adb_guest->callbacks->on_disconnect(adb_guest->opaque, adb_guest);
        adb_guest->adb_host = NULL;
    } else {
        D("Disconnecting ADB host %p(so=%d)", adb_host, adb_host->host_so);
    }

    
    alist_remove(&adb_host->list_entry);
    _adb_host_free(adb_host);

    
    if (adb_guest != NULL) {
        alist_remove(&adb_guest->list_entry);
    }
}

static void
_on_adb_host_read(AdbHost* adb_host)
{
    char buff[4096];

    
    const int size = socket_recv(adb_host->host_so, buff, sizeof(buff));
    if (size < 0) {
        D("Error while reading from ADB host %p(so=%d). Error: %s",
          adb_host, adb_host->host_so, strerror(errno));
    } else if (size == 0) {
        
        _on_adb_host_disconnected(adb_host);
    } else {
        D("%s %d bytes received from ADB host %p(so=%d): %s",
           adb_host->adb_guest ? "Transfer" : "Pend", size, adb_host,
           adb_host->host_so, QB(buff, size));

        AdbGuest* const adb_guest = adb_host->adb_guest;
        if (adb_guest != NULL && adb_guest->is_connected) {
            
            adb_guest->callbacks->on_read(adb_guest->opaque, adb_guest, buff, size);
        } else {
            
            if (adb_host->pending_data == NULL) {
                adb_host->pending_data = malloc(size);
            } else {
                adb_host->pending_data = realloc(adb_host->pending_data,
                                                 adb_host->pending_data_size + size);
            }
            if (adb_host->pending_data != NULL) {
                memcpy(adb_host->pending_data + adb_host->pending_data_size,
                       buff, size);
                adb_host->pending_data_size += size;
            } else {
                D("Unable to (re)allocate %d bytes for pending ADB host data",
                  adb_host->pending_data_size + size);
            }
        }
    }
}

static void
_on_adb_host_write(AdbHost* adb_host)
{
    while (adb_host->pending_send_data_size && adb_host->pending_send_buffer != NULL) {
        const int sent = socket_send(adb_host->host_so,
                                     adb_host->pending_send_buffer,
                                     adb_host->pending_send_data_size);
        if (sent < 0) {
            if (errno == EWOULDBLOCK) {
                
                return;
            } else {
                D("Unable to send pending data to the ADB host: %s",
                   strerror(errno));
                free(adb_host->pending_send_buffer);
                adb_host->pending_send_buffer = NULL;
                adb_host->pending_send_buffer_size = 0;
                adb_host->pending_send_data_size = 0;
                break;
            }
        } else if (sent == 0) {
            
            free(adb_host->pending_send_buffer);
            adb_host->pending_send_buffer = NULL;
            adb_host->pending_send_buffer_size = 0;
            adb_host->pending_send_data_size = 0;
            _on_adb_host_disconnected(adb_host);
            break;
        } else if (sent == adb_host->pending_send_data_size) {
            free(adb_host->pending_send_buffer);
            adb_host->pending_send_buffer = NULL;
            adb_host->pending_send_buffer_size = 0;
            adb_host->pending_send_data_size = 0;
        } else {
            adb_host->pending_send_data_size -= sent;
            memmove(adb_host->pending_send_buffer,
                    adb_host->pending_send_buffer + sent,
                    adb_host->pending_send_data_size);
            return;
        }
    }

    loopIo_dontWantWrite(adb_host->io);
}

static void
_on_adb_host_io(void* opaque, int fd, unsigned events)
{
    AdbHost* const adb_host = (AdbHost*)opaque;
    assert(fd == adb_host->host_so);

    
    if ((events & LOOP_IO_READ) != 0) {
        _on_adb_host_read(adb_host);
    }
    if ((events & LOOP_IO_WRITE) != 0) {
        _on_adb_host_write(adb_host);
    }
}


static AdbGuest*
_adb_guest_new(AdbServer* adb_srv)
{
    AdbGuest* adb_guest;

    ANEW0(adb_guest);
    alist_init(&adb_guest->list_entry);
    adb_guest->adb_srv = adb_srv;

    return adb_guest;
}

static void
_adb_guest_free(AdbGuest* adb_guest)
{
    if (adb_guest != NULL) {
        
        assert(alist_is_empty(&adb_guest->list_entry));
        AFREE(adb_guest);
    }
}


static void
_on_server_socket_io(void* opaque, int fd, unsigned events)
{
    AdbHost* adb_host;
    AdbGuest* adb_guest;
    AdbServer* adb_srv = (AdbServer*)opaque;
    assert(adb_srv->so == fd);

    
    if ((events & LOOP_IO_READ) == 0) {
        D("Unexpected write I/O on ADB server socket");
        return;
    }

    
    adb_host = _adb_host_new(adb_srv);

    
    adb_host->host_so = socket_accept(fd, &adb_srv->socket_address);
    if (adb_host->host_so < 0) {
        D("Unable to accept ADB connection: %s", strerror(errno));
        _adb_host_free(adb_host);
        return;
    }

    
    loopIo_init(adb_host->io, adb_srv->looper, adb_host->host_so,
                _on_adb_host_io, adb_host);

    
    adb_guest = (AdbGuest*)alist_remove_head(&adb_srv->pending_guests);
    if (adb_guest != NULL) {
        
        alist_insert_tail(&adb_srv->adb_guests, &adb_guest->list_entry);
        alist_insert_tail(&adb_srv->adb_hosts, &adb_host->list_entry);
        _adb_connect(adb_host, adb_guest);
    } else {
        
        D("Pend ADB host %p(so=%d)", adb_host, adb_host->host_so);
        alist_insert_tail(&adb_srv->pending_hosts, &adb_host->list_entry);
    }

    
    loopIo_wantRead(adb_host->io);
}

int
adb_server_init(int port)
{
    if (!_adb_server_initialized) {
        
        memset(&_adb_server, 0, sizeof(_adb_server));
        alist_init(&_adb_server.adb_hosts);
        alist_init(&_adb_server.adb_guests);
        alist_init(&_adb_server.pending_hosts);
        alist_init(&_adb_server.pending_guests);
        _adb_server.port = port;

        
        _adb_server.looper = looper_newCore();
        if (_adb_server.looper == NULL) {
            E("Unable to create I/O looper for ADB server");
            return -1;
        }

        
        sock_address_init_inet(&_adb_server.socket_address,
                               SOCK_ADDRESS_INET_LOOPBACK, port);
        _adb_server.so = socket_loopback_server(port, SOCKET_STREAM);
        if (_adb_server.so < 0) {
            E("Unable to create ADB server socket: %s", strerror(errno));
            return -1;
        }

        
        socket_set_nonblock(_adb_server.so);
        loopIo_init(_adb_server.io, _adb_server.looper, _adb_server.so,
                    _on_server_socket_io, &_adb_server);
        loopIo_wantRead(_adb_server.io);

        D("ADB server has been initialized for port %d. Socket: %d",
          port, _adb_server.so);

        _adb_server_initialized = 1;
    }

    return 0;
}

int
adb_server_is_initialized(void)
{
    return _adb_server_initialized;
}

void*
adb_server_register_guest(void* opaque, AdbGuestRoutines* callbacks)
{
    if (_adb_server_initialized) {
        AdbHost* adb_host;

        
        AdbGuest* const adb_guest = _adb_guest_new(&_adb_server);
        adb_guest->opaque = opaque;
        adb_guest->callbacks = callbacks;

        
        adb_host = (AdbHost*)alist_remove_head(&_adb_server.pending_hosts);
        if (adb_host != NULL) {
            
            alist_insert_tail(&_adb_server.adb_guests, &adb_guest->list_entry);
            alist_insert_tail(&_adb_server.adb_hosts, &adb_host->list_entry);
            _adb_connect(adb_host, adb_guest);
        } else {
            
            D("Pend ADB guest %p(o=%p)", adb_guest, adb_guest->opaque);
            alist_insert_tail(&_adb_server.pending_guests, &adb_guest->list_entry);
        }

        return adb_guest;
    } else {
        D("%s is called on an uninitialized ADB server.", __FUNCTION__);
        return NULL;
    }
}

void
adb_server_complete_connection(void* opaque)
{
    AdbGuest* const adb_guest = (AdbGuest*)opaque;
    AdbHost* const adb_host = adb_guest->adb_host;

    
    adb_guest->is_connected = 1;

    
    if (adb_host->pending_data != NULL && adb_host->pending_data_size != 0) {
        
        D("Pushing %d bytes of the pending ADB host data.",
          adb_host->pending_data_size);
        adb_guest->callbacks->on_read(adb_guest->opaque, adb_guest,
                                      adb_host->pending_data,
                                      adb_host->pending_data_size);
        free(adb_host->pending_data);
        adb_host->pending_data = NULL;
        adb_host->pending_data_size = 0;
    }
}

void
adb_server_on_guest_message(void* opaque, const uint8_t* msg, int msglen)
{
    AdbGuest* const adb_guest = (AdbGuest*)opaque;
    AdbHost* const adb_host = adb_guest->adb_host;

    if (adb_host != NULL) {
        D("Sending %d bytes to the ADB host: %s", msglen, QB(msg, msglen));

        
        if (adb_host->pending_send_buffer == NULL) {
            const int sent = socket_send(adb_host->host_so, msg, msglen);
            if (sent < 0) {
                if (errno == EWOULDBLOCK) {
                } else {
                    D("Unable to send data to ADB host: %s", strerror(errno));
                }
            } else if (sent == 0) {
                
                _on_adb_host_disconnected(adb_host);
            } else if (sent < msglen) {
                
                _adb_host_append_message(adb_host, msg + sent, msglen - sent);
            }
        } else {
            _adb_host_append_message(adb_host, msg, msglen);
        }
    } else {
        D("ADB host is disconneted and can't accept %d bytes in %s",
          msglen, QB(msg, msglen));
    }
}

void
adb_server_on_guest_closed(void* opaque)
{
    AdbGuest* const adb_guest = (AdbGuest*)opaque;
    AdbHost* const adb_host = adb_guest->adb_host;

    
    if (!alist_is_empty(&adb_guest->list_entry)) {
        alist_remove(&adb_guest->list_entry);
    }

    
    if (adb_host != NULL) {
        if (!alist_is_empty(&adb_host->list_entry)) {
            alist_remove(&adb_host->list_entry);
        }
        _adb_host_free(adb_host);
    }
    _adb_guest_free(adb_guest);
}

/*
 * Copyright (C) 2012 The Android Open Source Project
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


#include "android/utils/debug.h"
#include "android/async-socket-connector.h"
#include "android/async-socket.h"
#include "utils/panic.h"
#include "iolooper.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(asyncsocket,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(asyncsocket)

#define TRACE_ON    0

#if TRACE_ON
#define  T(...)    VERBOSE_PRINT(asyncsocket,__VA_ARGS__)
#else
#define  T(...)
#endif


static const char* _async_socket_string(AsyncSocket* as);

static Looper* _async_socket_get_looper(AsyncSocket* as);

static AsyncIOAction _async_socket_io_timed_out(AsyncSocket* as,
                                                AsyncSocketIO* asio);


struct AsyncSocketIO {
    
    AsyncSocketIO*      next;
    
    AsyncSocket*        as;
    
    LoopTimer           timer[1];
    
    void*               io_opaque;
    
    uint8_t*            buffer;
    
    uint32_t            to_transfer;
    
    uint32_t            transferred;
    
    on_as_io_cb         on_io;
    
    int                 is_io_read;
    
    AsyncIOState        state;
    
    int                 ref_count;
    
    Duration            deadline;
};


static AsyncSocketIO* _asio_recycled    = NULL;
static int _recycled_asio_count         = 0;
static const int _max_recycled_asio_num = 32;

static void _on_async_socket_io_timed_out(void* opaque);

static AsyncSocketIO*
_async_socket_rw_new(AsyncSocket* as,
                     int is_io_read,
                     void* buffer,
                     uint32_t len,
                     on_as_io_cb io_cb,
                     void* io_opaque,
                     Duration deadline)
{
    
    AsyncSocketIO* asio = _asio_recycled;
    if (asio != NULL) {
        
        _asio_recycled = asio->next;
        _recycled_asio_count--;
    } else {
        
        ANEW0(asio);
    }

    asio->next          = NULL;
    asio->as            = as;
    asio->is_io_read    = is_io_read;
    asio->buffer        = (uint8_t*)buffer;
    asio->to_transfer   = len;
    asio->transferred   = 0;
    asio->on_io         = io_cb;
    asio->io_opaque     = io_opaque;
    asio->state         = ASIO_STATE_QUEUED;
    asio->ref_count     = 1;
    asio->deadline      = deadline;
    loopTimer_init(asio->timer, _async_socket_get_looper(as),
                   _on_async_socket_io_timed_out, asio);
    loopTimer_startAbsolute(asio->timer, deadline);

    
    async_socket_reference(as);

    T("ASocket %s: %s I/O descriptor %p is created for %d bytes of data",
      _async_socket_string(as), is_io_read ? "READ" : "WRITE", asio, len);

    return asio;
}

static void
_async_socket_io_free(AsyncSocketIO* asio)
{
    AsyncSocket* const as = asio->as;

    T("ASocket %s: %s I/O descriptor %p is destroyed.",
      _async_socket_string(as), asio->is_io_read ? "READ" : "WRITE", asio);

    loopTimer_done(asio->timer);

    
    if (_recycled_asio_count < _max_recycled_asio_num) {
        asio->next = _asio_recycled;
        _asio_recycled = asio;
        _recycled_asio_count++;
    } else {
        AFREE(asio);
    }

    
    async_socket_release(as);
}

static void
_async_socket_io_finished(AsyncSocketIO* asio)
{
    
    asio->on_io(asio->io_opaque, asio, ASIO_STATE_FINISHED);
}

int
async_socket_io_reference(AsyncSocketIO* asio)
{
    assert(asio->ref_count > 0);
    asio->ref_count++;
    return asio->ref_count;
}

int
async_socket_io_release(AsyncSocketIO* asio)
{
    assert(asio->ref_count > 0);
    asio->ref_count--;
    if (asio->ref_count == 0) {
        _async_socket_io_finished(asio);
        
        _async_socket_io_free(asio);
        return 0;
    }
    return asio->ref_count;
}

static AsyncSocketIO*
_async_socket_reader_new(AsyncSocket* as,
                         void* buffer,
                         uint32_t len,
                         on_as_io_cb io_cb,
                         void* reader_opaque,
                         Duration deadline)
{
    AsyncSocketIO* const asio = _async_socket_rw_new(as, 1, buffer, len, io_cb,
                                                     reader_opaque, deadline);
    return asio;
}

static AsyncSocketIO*
_async_socket_writer_new(AsyncSocket* as,
                         const void* buffer,
                         uint32_t len,
                         on_as_io_cb io_cb,
                         void* writer_opaque,
                         Duration deadline)
{
    AsyncSocketIO* const asio = _async_socket_rw_new(as, 0, (void*)buffer, len,
                                                     io_cb, writer_opaque,
                                                     deadline);
    return asio;
}

static void
_on_async_socket_io_timed_out(void* opaque)
{
    AsyncSocketIO* const asio = (AsyncSocketIO*)opaque;
    AsyncSocket* const as = asio->as;

    D("ASocket %s: %s I/O with deadline %lld has timed out at %lld",
      _async_socket_string(as), asio->is_io_read ? "READ" : "WRITE",
      asio->deadline, async_socket_deadline(as, 0));

    
    async_socket_io_reference(asio);
    _async_socket_io_timed_out(asio->as, asio);
    async_socket_io_release(asio);
}


AsyncSocket*
async_socket_io_get_socket(const AsyncSocketIO* asio)
{
    async_socket_reference(asio->as);
    return asio->as;
}

void
async_socket_io_cancel_time_out(AsyncSocketIO* asio)
{
    loopTimer_stop(asio->timer);
}

void*
async_socket_io_get_io_opaque(const AsyncSocketIO* asio)
{
    return asio->io_opaque;
}

void*
async_socket_io_get_client_opaque(const AsyncSocketIO* asio)
{
    return async_socket_get_client_opaque(asio->as);
}

void*
async_socket_io_get_buffer_info(const AsyncSocketIO* asio,
                                uint32_t* transferred,
                                uint32_t* to_transfer)
{
    if (transferred != NULL) {
        *transferred = asio->transferred;
    }
    if (to_transfer != NULL) {
        *to_transfer = asio->to_transfer;
    }
    return asio->buffer;
}

void*
async_socket_io_get_buffer(const AsyncSocketIO* asio)
{
    return asio->buffer;
}

uint32_t
async_socket_io_get_transferred(const AsyncSocketIO* asio)
{
    return asio->transferred;
}

uint32_t
async_socket_io_get_to_transfer(const AsyncSocketIO* asio)
{
    return asio->to_transfer;
}

int
async_socket_io_is_read(const AsyncSocketIO* asio)
{
    return asio->is_io_read;
}


struct AsyncSocket {
    
    SockAddress         address;
    
    on_as_connection_cb on_connection;
    
    void*               client_opaque;
    
    Looper*             looper;
    
    LoopIo              io[1];
    
    LoopTimer           reconnect_timer[1];
    
    AsyncSocketIO*      readers_head;
    
    AsyncSocketIO*      readers_tail;
    
    AsyncSocketIO*      writers_head;
    
    AsyncSocketIO*      writers_tail;
    
    int                 fd;
    
    int                 reconnect_to;
    
    int                 ref_count;
    
    int                 owns_looper;
};

static const char*
_async_socket_string(AsyncSocket* as)
{
    return sock_address_to_string(&as->address);
}

static Looper*
_async_socket_get_looper(AsyncSocket* as)
{
    return as->looper;
}

static AsyncSocketIO*
_async_socket_pull_first_io(AsyncSocket* as,
                            AsyncSocketIO** list_head,
                            AsyncSocketIO** list_tail)
{
    AsyncSocketIO* const ret = *list_head;
    if (ret != NULL) {
        *list_head = ret->next;
        ret->next = NULL;
        if (*list_head == NULL) {
            *list_tail = NULL;
        }
    }
    return ret;
}

static AsyncSocketIO*
_async_socket_pull_first_reader(AsyncSocket* as)
{
    return _async_socket_pull_first_io(as, &as->readers_head, &as->readers_tail);
}

static AsyncSocketIO*
_async_socket_pull_first_writer(AsyncSocket* as)
{
    return _async_socket_pull_first_io(as, &as->writers_head, &as->writers_tail);
}

static int
_async_socket_remove_io(AsyncSocket* as,
                        AsyncSocketIO** list_head,
                        AsyncSocketIO** list_tail,
                        AsyncSocketIO* io)
{
    AsyncSocketIO* prev = NULL;

    while (*list_head != NULL && io != *list_head) {
        prev = *list_head;
        list_head = &((*list_head)->next);
    }
    if (*list_head == NULL) {
        D("%s: I/O %p is not found in the list for socket '%s'",
          __FUNCTION__, io, _async_socket_string(as));
        return 0;
    }

    *list_head = io->next;
    if (prev != NULL) {
        prev->next = io->next;
    }
    if (*list_tail == io) {
        *list_tail = prev;
    }

    
    async_socket_io_release(io);

    return 1;
}

static void
_async_socket_advance_io(AsyncSocket* as,
                         AsyncSocketIO** list_head,
                         AsyncSocketIO** list_tail)
{
    AsyncSocketIO* first_io = *list_head;
    if (first_io != NULL) {
        *list_head = first_io->next;
        first_io->next = NULL;
    }
    if (*list_head == NULL) {
        *list_tail = NULL;
    }
    if (first_io != NULL) {
        
        async_socket_io_release(first_io);
    }
}

static void
_async_socket_advance_reader(AsyncSocket* as)
{
    _async_socket_advance_io(as, &as->readers_head, &as->readers_tail);
}

static void
_async_socket_advance_writer(AsyncSocket* as)
{
    _async_socket_advance_io(as, &as->writers_head, &as->writers_tail);
}

static AsyncIOAction
_async_socket_complete_io(AsyncSocket* as, AsyncSocketIO* asio)
{
    T("ASocket %s: %s I/O %p is completed.",
      _async_socket_string(as), asio->is_io_read ? "READ" : "WRITE", asio);

    
    async_socket_io_cancel_time_out(asio);

    return asio->on_io(asio->io_opaque, asio, ASIO_STATE_SUCCEEDED);
}

static AsyncIOAction
_async_socket_io_timed_out(AsyncSocket* as, AsyncSocketIO* asio)
{
    T("ASocket %s: %s I/O %p with deadline %lld has timed out at %lld",
      _async_socket_string(as), asio->is_io_read ? "READ" : "WRITE", asio,
      asio->deadline, async_socket_deadline(as, 0));

    
    const AsyncIOAction action = asio->on_io(asio->io_opaque, asio,
                                             ASIO_STATE_TIMED_OUT);

    
    if (action != ASIO_ACTION_RETRY) {
        if (asio->is_io_read) {
            _async_socket_remove_io(as, &as->readers_head, &as->readers_tail, asio);
        } else {
            _async_socket_remove_io(as, &as->writers_head, &as->writers_tail, asio);
        }
    }

    return action;
}

static AsyncIOAction
_async_socket_cancel_io(AsyncSocket* as, AsyncSocketIO* asio)
{
    T("ASocket %s: %s I/O %p is cancelled.",
      _async_socket_string(as), asio->is_io_read ? "READ" : "WRITE", asio);

    
    async_socket_io_cancel_time_out(asio);

    return asio->on_io(asio->io_opaque, asio, ASIO_STATE_CANCELLED);
}

static AsyncIOAction
_async_socket_io_failure(AsyncSocket* as, AsyncSocketIO* asio, int failure)
{
    T("ASocket %s: %s I/O %p has failed: %d -> %s",
      _async_socket_string(as), asio->is_io_read ? "READ" : "WRITE", asio,
      failure, strerror(failure));

    
    async_socket_io_cancel_time_out(asio);

    errno = failure;
    return asio->on_io(asio->io_opaque, asio, ASIO_STATE_FAILED);
}

static void
_async_socket_cancel_readers(AsyncSocket* as)
{
    while (as->readers_head != NULL) {
        AsyncSocketIO* const to_cancel = _async_socket_pull_first_reader(as);
        _async_socket_cancel_io(as, to_cancel);
        async_socket_io_release(to_cancel);
    }
}

static void
_async_socket_cancel_writers(AsyncSocket* as)
{
    while (as->writers_head != NULL) {
        AsyncSocketIO* const to_cancel = _async_socket_pull_first_writer(as);
        _async_socket_cancel_io(as, to_cancel);
        async_socket_io_release(to_cancel);
    }
}

static void
_async_socket_cancel_all_io(AsyncSocket* as)
{
    
    loopTimer_stop(as->reconnect_timer);

    
    loopIo_dontWantWrite(as->io);
    loopIo_dontWantRead(as->io);

    
    _async_socket_cancel_readers(as);
    _async_socket_cancel_writers(as);
}

static void
_async_socket_close_socket(AsyncSocket* as)
{
    if (as->fd >= 0) {
        T("ASocket %s: Socket handle %d is closed.",
          _async_socket_string(as), as->fd);
        loopIo_done(as->io);
        socket_close(as->fd);
        as->fd = -1;
    }
}

static void
_async_socket_free(AsyncSocket* as)
{
    if (as != NULL) {
        T("ASocket %s: Socket descriptor is destroyed.", _async_socket_string(as));

        
        _async_socket_close_socket(as);

        
        if (as->looper != NULL) {
            loopTimer_done(as->reconnect_timer);
            if (as->owns_looper) {
                looper_free(as->looper);
            }
        }
        sock_address_done(&as->address);
        AFREE(as);
    }
}

static void
_async_socket_reconnect(AsyncSocket* as, int to)
{
    T("ASocket %s: reconnecting in %dms...", _async_socket_string(as), to);

    _async_socket_cancel_all_io(as);

    
    loopTimer_startRelative(as->reconnect_timer, to);
}


static void
_on_async_socket_disconnected(AsyncSocket* as)
{
    
    const int save_errno = errno;
    AsyncIOAction action = ASIO_ACTION_ABORT;

    D("ASocket %s: Disconnected.", _async_socket_string(as));

    
    _async_socket_cancel_all_io(as);

    
    _async_socket_close_socket(as);

    
    errno = save_errno;
    action = as->on_connection(as->client_opaque, as, ASIO_STATE_FAILED);

    if (action == ASIO_ACTION_RETRY) {
        
        _async_socket_reconnect(as, as->reconnect_to);
    }
}

static AsyncIOAction
_on_async_socket_failure(AsyncSocket* as, AsyncSocketIO* asio)
{
    D("ASocket %s: %s I/O failure: %d -> %s",
      _async_socket_string(as), asio->is_io_read ? "READ" : "WRITE",
      errno, strerror(errno));

    
    return _async_socket_io_failure(as, asio, errno);
}

static int
_on_async_socket_recv(AsyncSocket* as)
{
    AsyncIOAction action;

    
    AsyncSocketIO* const asr = as->readers_head;
    if (asr == NULL) {
        D("ASocket %s: No reader is available.", _async_socket_string(as));
        loopIo_dontWantRead(as->io);
        return 0;
    }

    
    async_socket_io_reference(asr);

    
    if (asr->state == ASIO_STATE_QUEUED) {
        asr->state = ASIO_STATE_STARTED;
    } else {
        asr->state = ASIO_STATE_CONTINUES;
    }
    action = asr->on_io(asr->io_opaque, asr, asr->state);
    if (action == ASIO_ACTION_ABORT) {
        D("ASocket %s: Read is aborted by the client.", _async_socket_string(as));
        
        _async_socket_advance_reader(as);
        if (as->readers_head != NULL) {
            loopIo_wantRead(as->io);
        } else {
            loopIo_dontWantRead(as->io);
        }
        async_socket_io_release(asr);
        return 0;
    }

    
    int res = socket_recv(as->fd, asr->buffer + asr->transferred,
                          asr->to_transfer - asr->transferred);
    while (res < 0 && errno == EINTR) {
        res = socket_recv(as->fd, asr->buffer + asr->transferred,
                          asr->to_transfer - asr->transferred);
    }

    if (res == 0) {
        
        errno = ECONNRESET;
        _on_async_socket_disconnected(as);
        async_socket_io_release(asr);
        return -1;
    }

    if (res < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            
            loopIo_wantRead(as->io);
            async_socket_io_release(asr);
            return 0;
        }

        
        action = _on_async_socket_failure(as, asr);
        if (action != ASIO_ACTION_RETRY) {
            D("ASocket %s: Read is aborted on failure.", _async_socket_string(as));
            
            _async_socket_advance_reader(as);
            if (as->readers_head != NULL) {
                loopIo_wantRead(as->io);
            } else {
                loopIo_dontWantRead(as->io);
            }
        }
        async_socket_io_release(asr);
        return -1;
    }

    
    asr->transferred += res;
    if (asr->transferred == asr->to_transfer) {
        
        _async_socket_advance_reader(as);

        
        _async_socket_complete_io(as, asr);
    }

    if (as->readers_head != NULL) {
        loopIo_wantRead(as->io);
    } else {
        loopIo_dontWantRead(as->io);
    }

    async_socket_io_release(asr);

    return 0;
}

static int
_on_async_socket_send(AsyncSocket* as)
{
    AsyncIOAction action;

    
    AsyncSocketIO* const asw = as->writers_head;
    if (asw == NULL) {
        D("ASocket %s: No writer is available.", _async_socket_string(as));
        loopIo_dontWantWrite(as->io);
        return 0;
    }

    
    async_socket_io_reference(asw);

    
    if (asw->state == ASIO_STATE_QUEUED) {
        asw->state = ASIO_STATE_STARTED;
    } else {
        asw->state = ASIO_STATE_CONTINUES;
    }
    action = asw->on_io(asw->io_opaque, asw, asw->state);
    if (action == ASIO_ACTION_ABORT) {
        D("ASocket %s: Write is aborted by the client.", _async_socket_string(as));
        
        _async_socket_advance_writer(as);
        if (as->writers_head != NULL) {
            loopIo_wantWrite(as->io);
        } else {
            loopIo_dontWantWrite(as->io);
        }
        async_socket_io_release(asw);
        return 0;
    }

    
    int res = socket_send(as->fd, asw->buffer + asw->transferred,
                          asw->to_transfer - asw->transferred);
    while (res < 0 && errno == EINTR) {
        res = socket_send(as->fd, asw->buffer + asw->transferred,
                          asw->to_transfer - asw->transferred);
    }

    if (res == 0) {
        
        errno = ECONNRESET;
        _on_async_socket_disconnected(as);
        async_socket_io_release(asw);
        return -1;
    }

    if (res < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            
            loopIo_wantWrite(as->io);
            async_socket_io_release(asw);
            return 0;
        }

        
        action = _on_async_socket_failure(as, asw);
        if (action != ASIO_ACTION_RETRY) {
            D("ASocket %s: Write is aborted on failure.", _async_socket_string(as));
            
            _async_socket_advance_writer(as);
            if (as->writers_head != NULL) {
                loopIo_wantWrite(as->io);
            } else {
                loopIo_dontWantWrite(as->io);
            }
        }
        async_socket_io_release(asw);
        return -1;
    }

    
    asw->transferred += res;
    if (asw->transferred == asw->to_transfer) {
        
        _async_socket_advance_writer(as);

        
        _async_socket_complete_io(as, asw);
    }

    if (as->writers_head != NULL) {
        loopIo_wantWrite(as->io);
    } else {
        loopIo_dontWantWrite(as->io);
    }

    async_socket_io_release(asw);

    return 0;
}

static void
_on_async_socket_io(void* opaque, int fd, unsigned events)
{
    AsyncSocket* const as = (AsyncSocket*)opaque;

    
    async_socket_reference(as);

    if ((events & LOOP_IO_READ) != 0) {
        if (_on_async_socket_recv(as) != 0) {
            async_socket_release(as);
            return;
        }
    }

    if ((events & LOOP_IO_WRITE) != 0) {
        if (_on_async_socket_send(as) != 0) {
            async_socket_release(as);
            return;
        }
    }

    async_socket_release(as);
}

static AsyncIOAction
_on_connector_events(void* opaque,
                     AsyncSocketConnector* connector,
                     AsyncIOState event)
{
    AsyncIOAction action;
    AsyncSocket* const as = (AsyncSocket*)opaque;

    
    async_socket_reference(as);

    if (event == ASIO_STATE_SUCCEEDED) {
        
        as->fd = async_socket_connector_pull_fd(connector);
        loopIo_init(as->io, as->looper, as->fd, _on_async_socket_io, as);
    }

    
    action = as->on_connection(as->client_opaque, as, event);
    if (event == ASIO_STATE_SUCCEEDED && action != ASIO_ACTION_DONE) {
        D("ASocket %s: Connection is discarded by the client.",
          _async_socket_string(as));
        _async_socket_close_socket(as);
    }

    if (action != ASIO_ACTION_RETRY) {
        async_socket_connector_release(connector);
    }

    async_socket_release(as);

    return action;
}

void
_on_async_socket_reconnect(void* opaque)
{
    AsyncSocket* as = (AsyncSocket*)opaque;

    
    async_socket_reference(as);
    async_socket_connect(as, as->reconnect_to);
    async_socket_release(as);
}



AsyncSocket*
async_socket_new(int port,
                 int reconnect_to,
                 on_as_connection_cb client_cb,
                 void* client_opaque,
                 Looper* looper)
{
    AsyncSocket* as;

    if (client_cb == NULL) {
        E("Invalid client_cb parameter");
        return NULL;
    }

    ANEW0(as);

    as->fd = -1;
    as->client_opaque = client_opaque;
    as->on_connection = client_cb;
    as->readers_head = as->readers_tail = NULL;
    as->reconnect_to = reconnect_to;
    as->ref_count = 1;
    sock_address_init_inet(&as->address, SOCK_ADDRESS_INET_LOOPBACK, port);
    if (looper == NULL) {
        as->looper = looper_newCore();
        if (as->looper == NULL) {
            E("Unable to create I/O looper for async socket '%s'",
              _async_socket_string(as));
            client_cb(client_opaque, as, ASIO_STATE_FAILED);
            _async_socket_free(as);
            return NULL;
        }
        as->owns_looper = 1;
    } else {
        as->looper = looper;
        as->owns_looper = 0;
    }

    loopTimer_init(as->reconnect_timer, as->looper, _on_async_socket_reconnect, as);

    T("ASocket %s: Descriptor is created.", _async_socket_string(as));

    return as;
}

int
async_socket_reference(AsyncSocket* as)
{
    assert(as->ref_count > 0);
    as->ref_count++;
    return as->ref_count;
}

int
async_socket_release(AsyncSocket* as)
{
    assert(as->ref_count > 0);
    as->ref_count--;
    if (as->ref_count == 0) {
        
        _async_socket_cancel_all_io(as);
        _async_socket_free(as);
        return 0;
    }
    return as->ref_count;
}

void
async_socket_connect(AsyncSocket* as, int retry_to)
{
    T("ASocket %s: Handling connection request for %dms...",
      _async_socket_string(as), retry_to);

    AsyncSocketConnector* const connector =
        async_socket_connector_new(&as->address, retry_to, _on_connector_events,
                                   as, as->looper);
    if (connector != NULL) {
        async_socket_connector_connect(connector);
    } else {
        as->on_connection(as->client_opaque, as, ASIO_STATE_FAILED);
    }
}

void
async_socket_disconnect(AsyncSocket* as)
{
    T("ASocket %s: Handling disconnection request...", _async_socket_string(as));

    if (as != NULL) {
        _async_socket_cancel_all_io(as);
        _async_socket_close_socket(as);
    }
}

void
async_socket_reconnect(AsyncSocket* as, int retry_to)
{
    T("ASocket %s: Handling reconnection request for %dms...",
      _async_socket_string(as), retry_to);

    _async_socket_cancel_all_io(as);
    _async_socket_close_socket(as);
    _async_socket_reconnect(as, retry_to);
}

void
async_socket_read_abs(AsyncSocket* as,
                      void* buffer, uint32_t len,
                      on_as_io_cb reader_cb,
                      void* reader_opaque,
                      Duration deadline)
{
    T("ASocket %s: Handling read for %d bytes with deadline %lld...",
      _async_socket_string(as), len, deadline);

    AsyncSocketIO* const asr =
        _async_socket_reader_new(as, buffer, len, reader_cb, reader_opaque,
                                 deadline);
    if (async_socket_is_connected(as)) {
        if (as->readers_head == NULL) {
            as->readers_head = as->readers_tail = asr;
        } else {
            as->readers_tail->next = asr;
            as->readers_tail = asr;
        }
        loopIo_wantRead(as->io);
    } else {
        D("ASocket %s: Read on a disconnected socket.", _async_socket_string(as));
        errno = ECONNRESET;
        reader_cb(reader_opaque, asr, ASIO_STATE_FAILED);
        async_socket_io_release(asr);
    }
}

void
async_socket_read_rel(AsyncSocket* as,
                      void* buffer, uint32_t len,
                      on_as_io_cb reader_cb,
                      void* reader_opaque,
                      int to)
{
    const Duration dl = (to >= 0) ? looper_now(_async_socket_get_looper(as)) + to :
                                    DURATION_INFINITE;
    async_socket_read_abs(as, buffer, len, reader_cb, reader_opaque, dl);
}

void
async_socket_write_abs(AsyncSocket* as,
                       const void* buffer, uint32_t len,
                       on_as_io_cb writer_cb,
                       void* writer_opaque,
                       Duration deadline)
{
    T("ASocket %s: Handling write for %d bytes with deadline %lld...",
      _async_socket_string(as), len, deadline);

    AsyncSocketIO* const asw =
        _async_socket_writer_new(as, buffer, len, writer_cb, writer_opaque,
                                 deadline);
    if (async_socket_is_connected(as)) {
        if (as->writers_head == NULL) {
            as->writers_head = as->writers_tail = asw;
        } else {
            as->writers_tail->next = asw;
            as->writers_tail = asw;
        }
        loopIo_wantWrite(as->io);
    } else {
        D("ASocket %s: Write on a disconnected socket.", _async_socket_string(as));
        errno = ECONNRESET;
        writer_cb(writer_opaque, asw, ASIO_STATE_FAILED);
        async_socket_io_release(asw);
    }
}

void
async_socket_write_rel(AsyncSocket* as,
                       const void* buffer, uint32_t len,
                       on_as_io_cb writer_cb,
                       void* writer_opaque,
                       int to)
{
    const Duration dl = (to >= 0) ? looper_now(_async_socket_get_looper(as)) + to :
                                    DURATION_INFINITE;
    async_socket_write_abs(as, buffer, len, writer_cb, writer_opaque, dl);
}

void*
async_socket_get_client_opaque(const AsyncSocket* as)
{
    return as->client_opaque;
}

Duration
async_socket_deadline(AsyncSocket* as, int rel)
{
    return (rel >= 0) ? looper_now(_async_socket_get_looper(as)) + rel :
                        DURATION_INFINITE;
}

int
async_socket_get_port(const AsyncSocket* as)
{
    return sock_address_get_port(&as->address);
}

int
async_socket_is_connected(const AsyncSocket* as)
{
    return as->fd >= 0;
}

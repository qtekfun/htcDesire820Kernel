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
#include "android/sdk-controller-socket.h"
#include "utils/panic.h"
#include "iolooper.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(sdkctlsocket,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(sdkctlsocket)

#define TRACE_ON    0

#if TRACE_ON
#define  T(...)    VERBOSE_PRINT(sdkctlsocket,__VA_ARGS__)
#else
#define  T(...)
#endif

typedef struct SDKCtlRecycled SDKCtlRecycled;
struct SDKCtlRecycled {
    union {
        
        SDKCtlRecycled* next;
        
        uint32_t        size;
    };
};


#define SDKCTL_PACKET_MESSAGE           1
#define SDKCTL_PACKET_QUERY             2
#define SDKCTL_PACKET_QUERY_RESPONSE    3


#define SDKCTL_MSG_PORT_CONNECTED       -1
#define SDKCTL_MSG_PORT_DISCONNECTED    -2
#define SDKCTL_MSG_PORT_ENABLED         -3
#define SDKCTL_MSG_PORT_DISABLED        -4


#define SDKCTL_QUERY_HANDSHAKE          -1


static const int _sdkctl_packet_sig = 0x53444B43;

typedef struct SDKCtlPacketHeader {
    
    int     signature;
    int     size;
    int     type;
} SDKCtlPacketHeader;

typedef struct SDKCtlPacket {
    SDKCtlRecycled          recycling;

    
    SDKCtlSocket*           sdkctl;
    
    int                     ref_count;

    SDKCtlPacketHeader      header;
} SDKCtlPacket;


struct SDKCtlDirectPacket {
    SDKCtlRecycled          recycling;

    
    SDKCtlSocket*           sdkctl;
    
    SDKCtlPacketHeader*     packet;
    
    on_sdkctl_direct_cb     on_sent;
    
    void*                   on_sent_opaque;
    
    int                     ref_count;
};


typedef struct SDKCtlQueryHeader {
    
    SDKCtlPacketHeader  packet;
    int                 query_id;
    
    int                 query_type;
} SDKCtlQueryHeader;

struct SDKCtlQuery {
    SDKCtlRecycled          recycling;

    
    SDKCtlQuery*            next;
    
    LoopTimer               timer[1];
    Duration                deadline;
    
    SDKCtlSocket*           sdkctl;
    
    on_sdkctl_query_cb      query_cb;
    
    void*                   query_opaque;
    
    void**                  response_buffer;
    uint32_t*               response_size;
    void*                   internal_resp_buffer;
    uint32_t                internal_resp_size;
    
    int                     ref_count;

    SDKCtlQueryHeader       header;
};

typedef struct SDKCtlQueryReplyHeader {
    
    SDKCtlPacketHeader  packet;

    
    int                 query_id;
} SDKCtlQueryReplyHeader;


typedef struct SDKCtlMessageHeader {
    
    SDKCtlPacketHeader  packet;
    
    int                 msg_type;
} SDKCtlMessageHeader;

struct SDKCtlMessage {
    
    SDKCtlPacket  packet;
    
    int           msg_type;
};


typedef enum SDKCtlSocketState {
    
    SDKCTL_SOCKET_DISCONNECTED,
    
    SDKCTL_SOCKET_CONNECTING,
    
    SDKCTL_SOCKET_CONNECTED
} SDKCtlSocketState;

typedef enum SDKCtlIODispatcherState {
    
    SDKCTL_IODISP_EXPECT_HEADER,
    
    SDKCTL_IODISP_EXPECT_DATA,
    
    SDKCTL_IODISP_EXPECT_QUERY_REPLY_HEADER,
    
    SDKCTL_IODISP_EXPECT_QUERY_REPLY_DATA,
} SDKCtlIODispatcherState;

typedef struct SDKCtlIODispatcher {
    
    SDKCtlSocket*               sdkctl;
    
    SDKCtlIODispatcherState     state;
    
    union {
        
        SDKCtlPacketHeader      packet_header;
        
        SDKCtlQueryHeader       query_header;
        
        SDKCtlMessageHeader     message_header;
        
        SDKCtlQueryReplyHeader  query_reply_header;
    };
    
    SDKCtlPacket*               packet;
    
    SDKCtlQuery*                current_query;
} SDKCtlIODispatcher;

struct SDKCtlSocket {
    
    SDKCtlSocketState               state;
    
    SdkCtlPortStatus                port_status;
    
    SDKCtlIODispatcher              io_dispatcher;
    
    AsyncSocket*                    as;
    
    on_sdkctl_socket_connection_cb  on_socket_connection;
    
    on_sdkctl_port_connection_cb    on_port_connection;
    
    on_sdkctl_message_cb            on_message;
    
    void*                           opaque;
    
    char*                           service_name;
    
    Looper*                         looper;
    
    SDKCtlQuery*                    query_head;
    
    SDKCtlQuery*                    query_tail;
    
    int                             next_query_id;
    
    int                             reconnect_to;
    
    int                             ref_count;
    
    SDKCtlRecycled*                 recycler;
    
    uint32_t                        recycler_block_size;
    
    int                             recycler_max;
    
    int                             recycler_count;
};


static void*
_sdkctl_socket_alloc_recycler(SDKCtlSocket* sdkctl, uint32_t size)
{
    SDKCtlRecycled* block = NULL;

    if (sdkctl->recycler != NULL && size <= sdkctl->recycler_block_size) {
        assert(sdkctl->recycler_count > 0);
        
        block = sdkctl->recycler;
        sdkctl->recycler = block->next;
        block->size = sdkctl->recycler_block_size;
        sdkctl->recycler_count--;
    } else if (size <= sdkctl->recycler_block_size) {
        block = malloc(sdkctl->recycler_block_size);
        if (block == NULL) {
            APANIC("SDKCtl %s: Unable to allocate %d bytes block.",
                   sdkctl->service_name, sdkctl->recycler_block_size);
        }
        block->size = sdkctl->recycler_block_size;
    } else {
        
        block = malloc(size);
        if (block == NULL) {
            APANIC("SDKCtl %s: Unable to allocate %d bytes block",
                   sdkctl->service_name, size);
        }
        block->size = size;
    }

    return block;
}

static void
_sdkctl_socket_free_recycler(SDKCtlSocket* sdkctl, void* mem)
{
    SDKCtlRecycled* const block = (SDKCtlRecycled*)mem;

    if (block->size != sdkctl->recycler_block_size ||
        sdkctl->recycler_count == sdkctl->recycler_max) {
        
        free(mem);
    } else {
        
        assert(sdkctl->recycler_count >= 0);
        block->next = sdkctl->recycler;
        sdkctl->recycler = block;
        sdkctl->recycler_count++;
    }
}

static void
_sdkctl_socket_empty_recycler(SDKCtlSocket* sdkctl)
{
    SDKCtlRecycled* block = sdkctl->recycler;
    while (block != NULL) {
        void* const to_free = block;
        block = block->next;
        free(to_free);
    }
    sdkctl->recycler = NULL;
    sdkctl->recycler_count = 0;
}


static void
_sdkctl_socket_add_query(SDKCtlQuery* query)
{
    SDKCtlSocket* const sdkctl = query->sdkctl;
    if (sdkctl->query_head == NULL) {
        assert(sdkctl->query_tail == NULL);
        sdkctl->query_head = sdkctl->query_tail = query;
    } else {
        sdkctl->query_tail->next = query;
        sdkctl->query_tail = query;
    }

    
    sdkctl_query_reference(query);
}

static int
_sdkctl_socket_remove_query(SDKCtlQuery* query)
{
    SDKCtlSocket* const sdkctl = query->sdkctl;
    SDKCtlQuery* prev = NULL;
    SDKCtlQuery* head = sdkctl->query_head;

    
    if (sdkctl->io_dispatcher.current_query == query) {
        
        sdkctl->io_dispatcher.current_query = NULL;
        sdkctl_query_release(query);
        return 1;
    }

    
    while (head != NULL && query != head) {
        prev = head;
        head = head->next;
    }
    if (head == NULL) {
        D("SDKCtl %s: Query %p is not found in the list.",
          sdkctl->service_name, query);
        return 0;
    }

    if (prev == NULL) {
        
        assert(query == sdkctl->query_head);
        sdkctl->query_head = query->next;
    } else {
        
        assert(query != sdkctl->query_head);
        prev->next = query->next;
    }
    if (sdkctl->query_tail == query) {
        
        assert(query->next == NULL);
        sdkctl->query_tail = prev;
    }
    query->next = NULL;

    sdkctl_query_release(query);

    return 1;
}

static SDKCtlQuery*
_sdkctl_socket_remove_query_id(SDKCtlSocket* sdkctl, int query_id)
{
    SDKCtlQuery* query = NULL;
    SDKCtlQuery* prev = NULL;
    SDKCtlQuery* head = sdkctl->query_head;

    
    if (sdkctl->io_dispatcher.current_query != NULL &&
        sdkctl->io_dispatcher.current_query->header.query_id == query_id) {
        
        query = sdkctl->io_dispatcher.current_query;
        sdkctl->io_dispatcher.current_query = NULL;
        return query;
    }

    
    while (head != NULL && head->header.query_id != query_id) {
        prev = head;
        head = head->next;
    }
    if (head == NULL) {
        D("SDKCtl %s: Query ID %d is not found in the list.",
          sdkctl->service_name, query_id);
        return NULL;
    }

    
    query = head;
    if (prev == NULL) {
        
        assert(query == sdkctl->query_head);
        sdkctl->query_head = query->next;
    } else {
        
        assert(query != sdkctl->query_head);
        prev->next = query->next;
    }
    if (sdkctl->query_tail == query) {
        
        assert(query->next == NULL);
        sdkctl->query_tail = prev;
    }
    query->next = NULL;

    return query;
}

static SDKCtlQuery*
_sdkctl_socket_pull_first_query(SDKCtlSocket* sdkctl)
{
    SDKCtlQuery* const query = sdkctl->query_head;

    if (query != NULL) {
        sdkctl->query_head = query->next;
        if (sdkctl->query_head == NULL) {
            sdkctl->query_tail = NULL;
        }
    }
    return query;
}

static int
_sdkctl_socket_next_query_id(SDKCtlSocket* sdkctl)
{
    return ++sdkctl->next_query_id;
}


static SDKCtlPacket*
_sdkctl_packet_new(SDKCtlSocket* sdkctl, uint32_t size, int type)
{
    
    SDKCtlPacket* const packet =
        _sdkctl_socket_alloc_recycler(sdkctl, sizeof(SDKCtlPacket) + size);

    packet->sdkctl              = sdkctl;
    packet->ref_count           = 1;
    packet->header.signature    = _sdkctl_packet_sig;
    packet->header.size         = size;
    packet->header.type         = type;

    
    sdkctl_socket_reference(sdkctl);

    T("SDKCtl %s: Packet %p of type %d is allocated for %d bytes transfer.",
          sdkctl->service_name, packet, type, size);

    return packet;
}

static void
_sdkctl_packet_free(SDKCtlPacket* packet)
{
    SDKCtlSocket* const sdkctl = packet->sdkctl;

    
    _sdkctl_socket_free_recycler(packet->sdkctl, packet);

    T("SDKCtl %s: Packet %p is freed.", sdkctl->service_name, packet);

    
    sdkctl_socket_release(sdkctl);
}

int
_sdkctl_packet_reference(SDKCtlPacket* packet)
{
    assert(packet->ref_count > 0);
    packet->ref_count++;
    return packet->ref_count;
}

int
_sdkctl_packet_release(SDKCtlPacket* packet)
{
    assert(packet->ref_count > 0);
    packet->ref_count--;
    if (packet->ref_count == 0) {
        
        _sdkctl_packet_free(packet);
        return 0;
    }
    return packet->ref_count;
}

static AsyncIOAction
_on_sdkctl_packet_send_io(void* io_opaque,
                          AsyncSocketIO* asio,
                          AsyncIOState status)
{
    SDKCtlPacket* const packet = (SDKCtlPacket*)io_opaque;
    AsyncIOAction action = ASIO_ACTION_DONE;

    
    _sdkctl_packet_reference(packet);

    
    switch (status) {
        case ASIO_STATE_SUCCEEDED:
            
            T("SDKCtl %s: Packet %p transmission has succeeded.",
              packet->sdkctl->service_name, packet);
            break;

        case ASIO_STATE_CANCELLED:
            T("SDKCtl %s: Packet %p is cancelled.",
              packet->sdkctl->service_name, packet);
            break;

        case ASIO_STATE_FAILED:
            T("SDKCtl %s: Packet %p has failed: %d -> %s",
              packet->sdkctl->service_name, packet, errno, strerror(errno));
            break;

        case ASIO_STATE_FINISHED:
            
            _sdkctl_packet_release(packet);
            break;

        default:
            
            break;
    }

    _sdkctl_packet_release(packet);

    return action;
}

static void
_sdkctl_packet_transmit(SDKCtlPacket* packet)
{
    assert(packet->header.signature == _sdkctl_packet_sig);

    
    _sdkctl_packet_reference(packet);

    
    async_socket_write_rel(packet->sdkctl->as, &packet->header, packet->header.size,
                           _on_sdkctl_packet_send_io, packet, -1);

    T("SDKCtl %s: Packet %p size %d is being sent.",
      packet->sdkctl->service_name, packet, packet->header.size);
}


SDKCtlDirectPacket*
sdkctl_direct_packet_new(SDKCtlSocket* sdkctl)
{
    SDKCtlDirectPacket* const packet =
        _sdkctl_socket_alloc_recycler(sdkctl, sizeof(SDKCtlDirectPacket));

    packet->sdkctl      = sdkctl;
    packet->ref_count   = 1;

    
    sdkctl_socket_reference(packet->sdkctl);

    T("SDKCtl %s: Direct packet %p is allocated.", sdkctl->service_name, packet);

    return packet;
}

static void
_sdkctl_direct_packet_free(SDKCtlDirectPacket* packet)
{
    SDKCtlSocket* const sdkctl = packet->sdkctl;

    
    _sdkctl_socket_free_recycler(packet->sdkctl, packet);

    T("SDKCtl %s: Direct packet %p is freed.", sdkctl->service_name, packet);

    
    sdkctl_socket_release(sdkctl);
}

int
sdkctl_direct_packet_reference(SDKCtlDirectPacket* packet)
{
    assert(packet->ref_count > 0);
    packet->ref_count++;
    return packet->ref_count;
}

int
sdkctl_direct_packet_release(SDKCtlDirectPacket* packet)
{
    assert(packet->ref_count > 0);
    packet->ref_count--;
    if (packet->ref_count == 0) {
        
        _sdkctl_direct_packet_free(packet);
        return 0;
    }
    return packet->ref_count;
}

static AsyncIOAction
_on_sdkctl_direct_packet_send_io(void* io_opaque,
                                 AsyncSocketIO* asio,
                                 AsyncIOState status)
{
    SDKCtlDirectPacket* const packet = (SDKCtlDirectPacket*)io_opaque;
    AsyncIOAction action = ASIO_ACTION_DONE;

    
    sdkctl_direct_packet_reference(packet);

    
    switch (status) {
        case ASIO_STATE_SUCCEEDED:
            
            T("SDKCtl %s: Direct packet %p transmission has succeeded.",
              packet->sdkctl->service_name, packet);
            packet->on_sent(packet->on_sent_opaque, packet, status);
            break;

        case ASIO_STATE_CANCELLED:
            T("SDKCtl %s: Direct packet %p is cancelled.",
              packet->sdkctl->service_name, packet);
            packet->on_sent(packet->on_sent_opaque, packet, status);
            break;

        case ASIO_STATE_FAILED:
            T("SDKCtl %s: Direct packet %p has failed: %d -> %s",
              packet->sdkctl->service_name, packet, errno, strerror(errno));
            packet->on_sent(packet->on_sent_opaque, packet, status);
            break;

        case ASIO_STATE_FINISHED:
            
            sdkctl_direct_packet_release(packet);
            break;

        default:
            
            break;
    }

    sdkctl_direct_packet_release(packet);

    return action;
}

void
sdkctl_direct_packet_send(SDKCtlDirectPacket* packet,
                          void* data,
                          on_sdkctl_direct_cb cb,
                          void* cb_opaque)
{
    packet->packet          = (SDKCtlPacketHeader*)data;
    packet->on_sent         = cb;
    packet->on_sent_opaque  = cb_opaque;
    assert(packet->packet->signature == _sdkctl_packet_sig);

    
    sdkctl_direct_packet_reference(packet);

    
    async_socket_write_rel(packet->sdkctl->as, packet->packet, packet->packet->size,
                           _on_sdkctl_direct_packet_send_io, packet, -1);

    T("SDKCtl %s: Direct packet %p size %d is being sent",
      packet->sdkctl->service_name, packet, packet->packet->size);
}


static SDKCtlMessage*
_sdkctl_message_new(SDKCtlSocket* sdkctl, uint32_t msg_size, int msg_type)
{
    SDKCtlMessage* const msg =
        (SDKCtlMessage*)_sdkctl_packet_new(sdkctl,
                                           sizeof(SDKCtlMessageHeader) + msg_size,
                                           SDKCTL_PACKET_MESSAGE);
    msg->msg_type = msg_type;

    return msg;
}

int
sdkctl_message_reference(SDKCtlMessage* msg)
{
    return _sdkctl_packet_reference(&msg->packet);
}

int
sdkctl_message_release(SDKCtlMessage* msg)
{
    return _sdkctl_packet_release(&msg->packet);
}

SDKCtlMessage*
sdkctl_message_send(SDKCtlSocket* sdkctl,
                    int msg_type,
                    const void* data,
                    uint32_t size)
{
    SDKCtlMessage* const msg = _sdkctl_message_new(sdkctl, size, msg_type);
    if (size != 0 && data != NULL) {
        memcpy(msg + 1, data, size);
    }
    _sdkctl_packet_transmit(&msg->packet);

    return msg;
}

int
sdkctl_message_get_header_size(void)
{
    return sizeof(SDKCtlMessageHeader);
}

void
sdkctl_init_message_header(void* msg, int msg_type, int msg_size)
{
    SDKCtlMessageHeader* const msg_header = (SDKCtlMessageHeader*)msg;

    msg_header->packet.signature    = _sdkctl_packet_sig;
    msg_header->packet.size         = sizeof(SDKCtlMessageHeader) + msg_size;
    msg_header->packet.type         = SDKCTL_PACKET_MESSAGE;
    msg_header->msg_type            = msg_type;
}


static void
_sdkctl_query_free(SDKCtlQuery* query)
{
    if (query != NULL) {
        SDKCtlSocket* const sdkctl = query->sdkctl;
        if (query->internal_resp_buffer != NULL &&
            (query->response_buffer == NULL ||
             query->response_buffer == &query->internal_resp_buffer)) {
            free(query->internal_resp_buffer);
        }

        loopTimer_done(query->timer);

        
        _sdkctl_socket_free_recycler(sdkctl, query);

        T("SDKCtl %s: Query %p is freed.", sdkctl->service_name, query);

        
        sdkctl_socket_release(sdkctl);
    }
}

static void
_sdkctl_query_cancel_timeout(SDKCtlQuery* query)
{
    loopTimer_stop(query->timer);

    T("SDKCtl %s: Query %p ID %d deadline %lld is cancelled.",
      query->sdkctl->service_name, query, query->header.query_id, query->deadline);
}


static void
_on_sdkctl_query_completed(SDKCtlQuery* query)
{
    T("SDKCtl %s: Query %p ID %d is completed.",
      query->sdkctl->service_name, query, query->header.query_id);

    
    _sdkctl_query_cancel_timeout(query);
    query->query_cb(query->query_opaque, query, ASIO_STATE_SUCCEEDED);
}

static void
_on_sdkctl_query_cancelled(SDKCtlQuery* query)
{

    
    _sdkctl_query_cancel_timeout(query);
    query->query_cb(query->query_opaque, query, ASIO_STATE_CANCELLED);
}

static void
_on_skdctl_query_timeout(void* opaque)
{
    SDKCtlQuery* const query = (SDKCtlQuery*)opaque;

    D("SDKCtl %s: Query %p ID %d with deadline %lld has timed out at %lld",
      query->sdkctl->service_name, query, query->header.query_id,
      query->deadline, async_socket_deadline(query->sdkctl->as, 0));

    
    sdkctl_query_reference(query);

    const AsyncIOAction action =
        query->query_cb(query->query_opaque, query, ASIO_STATE_TIMED_OUT);

    
    if (action != ASIO_ACTION_RETRY) {
        _sdkctl_socket_remove_query(query);
    }

    sdkctl_query_release(query);
}

static void
_on_sdkctl_query_sent(SDKCtlQuery* query)
{
    T("SDKCtl %s: Sent %d bytes of query %p ID %d of type %d",
      query->sdkctl->service_name, query->header.packet.size, query,
      query->header.query_id, query->header.query_type);

    
    query->query_cb(query->query_opaque, query, ASIO_STATE_CONTINUES);

    loopTimer_startAbsolute(query->timer, query->deadline);
}

static AsyncIOAction
_on_sdkctl_query_send_io(void* io_opaque,
                         AsyncSocketIO* asio,
                         AsyncIOState status)
{
    SDKCtlQuery* const query = (SDKCtlQuery*)io_opaque;
    AsyncIOAction action = ASIO_ACTION_DONE;

    
    sdkctl_query_reference(query);

    
    switch (status) {
        case ASIO_STATE_SUCCEEDED:
            
            _on_sdkctl_query_sent(query);
            break;

        case ASIO_STATE_CANCELLED:
            T("SDKCtl %s: Query %p ID %d is cancelled in transmission.",
              query->sdkctl->service_name, query, query->header.query_id);
            
            _sdkctl_socket_remove_query(query);
            _on_sdkctl_query_cancelled(query);
            break;

        case ASIO_STATE_TIMED_OUT:
            D("SDKCtl %s: Query %p ID %d with deadline %lld has timed out in transmission at %lld",
              query->sdkctl->service_name, query, query->header.query_id,
              query->deadline,  async_socket_deadline(query->sdkctl->as, 0));
            
            action = query->query_cb(query->query_opaque, query, status);
            
            if (action != ASIO_ACTION_RETRY) {
                _sdkctl_socket_remove_query(query);
            }
            break;

        case ASIO_STATE_FAILED:
            T("SDKCtl %s: Query %p ID %d failed in transmission: %d -> %s",
              query->sdkctl->service_name, query, query->header.query_id,
              errno, strerror(errno));
            action = query->query_cb(query->query_opaque, query, status);
            
            if (action != ASIO_ACTION_RETRY) {
                _sdkctl_socket_remove_query(query);
            }
            break;

        case ASIO_STATE_FINISHED:
            
            sdkctl_query_release(query);
            break;

        default:
            
            break;
    }

    sdkctl_query_release(query);

    return action;
}


SDKCtlQuery*
sdkctl_query_new(SDKCtlSocket* sdkctl, int query_type, uint32_t in_data_size)
{
    SDKCtlQuery* const query =
        _sdkctl_socket_alloc_recycler(sdkctl, sizeof(SDKCtlQuery) + in_data_size);
    query->next                     = NULL;
    query->sdkctl                   = sdkctl;
    query->response_buffer          = NULL;
    query->response_size            = NULL;
    query->internal_resp_buffer     = NULL;
    query->internal_resp_size       = 0;
    query->query_cb                 = NULL;
    query->query_opaque             = NULL;
    query->deadline                 = DURATION_INFINITE;
    query->ref_count                = 1;
    query->header.packet.signature  = _sdkctl_packet_sig;
    query->header.packet.size       = sizeof(SDKCtlQueryHeader) + in_data_size;
    query->header.packet.type       = SDKCTL_PACKET_QUERY;
    query->header.query_id          = _sdkctl_socket_next_query_id(sdkctl);
    query->header.query_type        = query_type;

    
    loopTimer_init(query->timer, sdkctl->looper, _on_skdctl_query_timeout, query);

    
    sdkctl_socket_reference(sdkctl);

    T("SDKCtl %s: Query %p ID %d type %d is created for %d bytes of data.",
      query->sdkctl->service_name, query, query->header.query_id,
      query_type, in_data_size);

    return query;
}

SDKCtlQuery*
sdkctl_query_new_ex(SDKCtlSocket* sdkctl,
                    int query_type,
                    uint32_t in_data_size,
                    const void* in_data,
                    void** response_buffer,
                    uint32_t* response_size,
                    on_sdkctl_query_cb query_cb,
                    void* query_opaque)
{
    SDKCtlQuery* const query = sdkctl_query_new(sdkctl, query_type, in_data_size);

    query->response_buffer = response_buffer;
    if (query->response_buffer == NULL) {
        
        query->response_buffer = &query->internal_resp_buffer;
    }
    query->response_size = response_size;
    if (query->response_size == NULL) {
        query->response_size = &query->internal_resp_size;
    }
    query->query_cb = query_cb;
    query->query_opaque = query_opaque;
    
    if (in_data_size != 0 && in_data != NULL) {
        memcpy(query + 1, in_data, in_data_size);
    }

    return query;
}

void
sdkctl_query_send(SDKCtlQuery* query, int to)
{
    SDKCtlSocket* const sdkctl = query->sdkctl;

    
    query->deadline = async_socket_deadline(query->sdkctl->as, to);

    
    _sdkctl_socket_add_query(query);

    
    sdkctl_query_reference(query);

    assert(query->header.packet.signature == _sdkctl_packet_sig);
    
    async_socket_write_abs(sdkctl->as, &query->header, query->header.packet.size,
                           _on_sdkctl_query_send_io, query, query->deadline);

    T("SDKCtl %s: Query %p ID %d type %d is being sent with deadline at %lld",
      query->sdkctl->service_name, query, query->header.query_id,
      query->header.query_type, query->deadline);
}

SDKCtlQuery*
sdkctl_query_build_and_send(SDKCtlSocket* sdkctl,
                            int query_type,
                            uint32_t in_data_size,
                            const void* in_data,
                            void** response_buffer,
                            uint32_t* response_size,
                            on_sdkctl_query_cb query_cb,
                            void* query_opaque,
                            int to)
{
    SDKCtlQuery* const query =
        sdkctl_query_new_ex(sdkctl, query_type, in_data_size, in_data,
                            response_buffer, response_size, query_cb,
                            query_opaque);
    sdkctl_query_send(query, to);
    return query;
}

int
sdkctl_query_reference(SDKCtlQuery* query)
{
    assert(query->ref_count > 0);
    query->ref_count++;
    return query->ref_count;
}

int
sdkctl_query_release(SDKCtlQuery* query)
{
    assert(query->ref_count > 0);
    query->ref_count--;
    if (query->ref_count == 0) {
        
        _sdkctl_query_free(query);
        return 0;
    }
    return query->ref_count;
}

void*
sdkctl_query_get_buffer_in(SDKCtlQuery* query)
{
    
    return query + 1;
}

void*
sdkctl_query_get_buffer_out(SDKCtlQuery* query)
{
    return query->response_buffer != NULL ? *query->response_buffer :
                                            query->internal_resp_buffer;
}


static void
_on_sdkctl_packet_received(SDKCtlSocket* sdkctl, SDKCtlPacket* packet)
{
    T("SDKCtl %s: Received packet size: %d, type: %d",
      sdkctl->service_name, packet->header.size, packet->header.type);

    assert(packet->header.signature == _sdkctl_packet_sig);
    if (packet->header.type == SDKCTL_PACKET_MESSAGE) {
        SDKCtlMessage* const msg = (SDKCtlMessage*)packet;
        
        switch (msg->msg_type) {
            case SDKCTL_MSG_PORT_CONNECTED:
                sdkctl->port_status = SDKCTL_PORT_CONNECTED;
                sdkctl->on_port_connection(sdkctl->opaque, sdkctl,
                                           SDKCTL_PORT_CONNECTED);
                break;

            case SDKCTL_MSG_PORT_DISCONNECTED:
                sdkctl->port_status = SDKCTL_PORT_DISCONNECTED;
                sdkctl->on_port_connection(sdkctl->opaque, sdkctl,
                                           SDKCTL_PORT_DISCONNECTED);
                break;

            case SDKCTL_MSG_PORT_ENABLED:
                sdkctl->port_status = SDKCTL_PORT_ENABLED;
                sdkctl->on_port_connection(sdkctl->opaque, sdkctl,
                                           SDKCTL_PORT_ENABLED);
                break;

            case SDKCTL_MSG_PORT_DISABLED:
                sdkctl->port_status = SDKCTL_PORT_DISABLED;
                sdkctl->on_port_connection(sdkctl->opaque, sdkctl,
                                           SDKCTL_PORT_DISABLED);
                break;

            default:
                sdkctl->on_message(sdkctl->opaque, sdkctl, msg, msg->msg_type, msg + 1,
                                   packet->header.size - sizeof(SDKCtlMessageHeader));
                break;
        }
    } else {
        E("SDKCtl %s: Received unknown packet type %d size %d",
          sdkctl->service_name, packet->header.type, packet->header.size);
    }
}


static AsyncIOAction _on_sdkctl_io_dispatcher_io(void* io_opaque,
                                                 AsyncSocketIO* asio,
                                                 AsyncIOState status);

static void
_sdkctl_io_dispatcher_start(SDKCtlSocket* sdkctl) {
    SDKCtlIODispatcher* const dispatcher = &sdkctl->io_dispatcher;

    dispatcher->state           = SDKCTL_IODISP_EXPECT_HEADER;
    dispatcher->sdkctl          = sdkctl;
    dispatcher->packet          = NULL;
    dispatcher->current_query   = NULL;

    
    async_socket_read_rel(dispatcher->sdkctl->as, &dispatcher->packet_header,
                          sizeof(SDKCtlPacketHeader), _on_sdkctl_io_dispatcher_io,
                          dispatcher, -1);
}

static void
_sdkctl_io_dispatcher_reset(SDKCtlSocket* sdkctl) {
    SDKCtlIODispatcher* const dispatcher = &sdkctl->io_dispatcher;

    
    if (dispatcher->current_query != NULL) {
        SDKCtlQuery* const query = dispatcher->current_query;
        dispatcher->current_query = NULL;
        _on_sdkctl_query_cancelled(query);
        sdkctl_query_release(query);
    }

    
    if (dispatcher->packet != NULL) {
        _sdkctl_packet_release(dispatcher->packet);
        dispatcher->packet = NULL;
    }

    
    dispatcher->state = SDKCTL_IODISP_EXPECT_HEADER;

    T("SDKCtl %s: I/O Dispatcher is reset", sdkctl->service_name);
}


static void
_on_io_dispatcher_io_failure(SDKCtlIODispatcher* dispatcher,
                             AsyncSocketIO* asio)
{
    SDKCtlSocket* const sdkctl = dispatcher->sdkctl;

    D("SDKCtl %s: Dispatcher I/O failure: %d -> %s",
      sdkctl->service_name, errno, strerror(errno));

    sdkctl_socket_disconnect(sdkctl);

    sdkctl->on_socket_connection(sdkctl->opaque, sdkctl, ASIO_STATE_FAILED);
}

static void
_on_io_dispatcher_io_cancelled(SDKCtlIODispatcher* dispatcher,
                               AsyncSocketIO* asio)
{
    T("SDKCtl %s: Dispatcher I/O cancelled.", dispatcher->sdkctl->service_name);

    if (dispatcher->current_query != NULL) {
        SDKCtlQuery* const query = dispatcher->current_query;
        dispatcher->current_query = NULL;
        _on_sdkctl_query_cancelled(query);
        sdkctl_query_release(query);
    }

    
    if (dispatcher->packet != NULL) {
        _sdkctl_packet_release(dispatcher->packet);
        dispatcher->packet = NULL;
    }
}

static AsyncIOAction
_on_io_dispatcher_packet_header(SDKCtlIODispatcher* dispatcher,
                                AsyncSocketIO* asio)
{
    SDKCtlSocket* const sdkctl = dispatcher->sdkctl;

    T("SDKCtl %s: Packet header type %d, size %d is received.",
      dispatcher->sdkctl->service_name, dispatcher->packet_header.type,
      dispatcher->packet_header.size);

    
    if (dispatcher->packet_header.signature != _sdkctl_packet_sig) {
        E("SDKCtl %s: Invalid packet signature %x for packet type %d, size %d",
          sdkctl->service_name, dispatcher->packet_header.signature,
          dispatcher->packet_header.type, dispatcher->packet_header.size);
        errno = EINVAL;
        _on_io_dispatcher_io_failure(dispatcher, asio);
        return ASIO_ACTION_DONE;
    }

     if (dispatcher->packet_header.type == SDKCTL_PACKET_QUERY_RESPONSE) {
        dispatcher->state = SDKCTL_IODISP_EXPECT_QUERY_REPLY_HEADER;
        async_socket_read_rel(sdkctl->as, &dispatcher->query_reply_header.query_id,
                              sizeof(SDKCtlQueryReplyHeader) - sizeof(SDKCtlPacketHeader),
                             _on_sdkctl_io_dispatcher_io, dispatcher, -1);
    } else {
        dispatcher->state = SDKCTL_IODISP_EXPECT_DATA;
        dispatcher->packet =
            _sdkctl_packet_new(sdkctl, dispatcher->packet_header.size,
                               dispatcher->packet_header.type);
        
        async_socket_read_rel(sdkctl->as, dispatcher->packet + 1,
                              dispatcher->packet_header.size - sizeof(SDKCtlPacketHeader),
                              _on_sdkctl_io_dispatcher_io, dispatcher, -1);
    }

    return ASIO_ACTION_DONE;
}

static AsyncIOAction
_on_io_dispatcher_packet(SDKCtlIODispatcher* dispatcher, AsyncSocketIO* asio)
{
    SDKCtlSocket* const sdkctl = dispatcher->sdkctl;
    SDKCtlPacket* const packet = dispatcher->packet;
    dispatcher->packet = NULL;

    T("SDKCtl %s: Packet type %d, size %d is received.",
      dispatcher->sdkctl->service_name, dispatcher->packet_header.type,
      dispatcher->packet_header.size);

    _on_sdkctl_packet_received(sdkctl, packet);
    _sdkctl_packet_release(packet);

    
    dispatcher->state = SDKCTL_IODISP_EXPECT_HEADER;
    async_socket_read_rel(sdkctl->as, &dispatcher->packet_header, sizeof(SDKCtlPacketHeader),
                          _on_sdkctl_io_dispatcher_io, dispatcher, -1);
    return ASIO_ACTION_DONE;
}

static AsyncIOAction
_on_io_dispatcher_query_reply_header(SDKCtlIODispatcher* dispatcher,
                                     AsyncSocketIO* asio)
{
    SDKCtlSocket* const sdkctl = dispatcher->sdkctl;
    SDKCtlQuery* query;

    T("SDKCtl %s: Query reply header is received for query ID %d",
      dispatcher->sdkctl->service_name, dispatcher->query_reply_header.query_id);

    dispatcher->current_query =
        _sdkctl_socket_remove_query_id(sdkctl, dispatcher->query_reply_header.query_id);
    query = dispatcher->current_query;
    const uint32_t query_data_size =
        dispatcher->packet_header.size - sizeof(SDKCtlQueryReplyHeader);
    dispatcher->state = SDKCTL_IODISP_EXPECT_QUERY_REPLY_DATA;

    if (query == NULL) {
        D("%s: Query #%d is not found by dispatcher",
          dispatcher->sdkctl->service_name, dispatcher->query_reply_header.query_id);

        dispatcher->state = SDKCTL_IODISP_EXPECT_QUERY_REPLY_DATA;
        dispatcher->packet =
            _sdkctl_packet_new(sdkctl, dispatcher->packet_header.size,
                               dispatcher->packet_header.type);
        
        memcpy(&dispatcher->packet->header, &dispatcher->query_reply_header,
               sizeof(SDKCtlQueryReplyHeader));
        async_socket_read_rel(sdkctl->as, dispatcher->packet + 1, query_data_size,
                             _on_sdkctl_io_dispatcher_io, dispatcher, -1);
    } else {
        _sdkctl_query_cancel_timeout(query);

        if (*query->response_size < query_data_size) {
            *query->response_buffer = malloc(query_data_size);
            if (*query->response_buffer == NULL) {
                APANIC("%s: Unable to allocate %d bytes for query response",
                       sdkctl->service_name, query_data_size);
            }
        }
        
        *query->response_size = query_data_size;

        
        async_socket_read_rel(sdkctl->as, *query->response_buffer,
                              *query->response_size, _on_sdkctl_io_dispatcher_io,
                              dispatcher, -1);
    }

    return ASIO_ACTION_DONE;
}

static AsyncIOAction
_on_io_dispatcher_query_reply(SDKCtlIODispatcher* dispatcher, AsyncSocketIO* asio)
{
    SDKCtlSocket* const sdkctl = dispatcher->sdkctl;
    SDKCtlQuery* const query = dispatcher->current_query;
    dispatcher->current_query = NULL;

    if (query != NULL) {
        _ANDROID_ASSERT(query->header.query_id == dispatcher->query_reply_header.query_id,
                        "SDKCtl %s: Query ID mismatch in I/O dispatcher",
                        sdkctl->service_name);
        T("SDKCtl %s: Query reply is received for query %p ID %d. Reply size is %d",
          dispatcher->sdkctl->service_name, query, query->header.query_id,
          *query->response_size);

        
        _on_sdkctl_query_completed(query);
        sdkctl_query_release(query);
    } else {
        if (dispatcher->packet != NULL) {
            _sdkctl_packet_release(dispatcher->packet);
            dispatcher->packet = NULL;
        }
    }

    
    dispatcher->state = SDKCTL_IODISP_EXPECT_HEADER;
    async_socket_read_rel(sdkctl->as, &dispatcher->packet_header, sizeof(SDKCtlPacketHeader),
                          _on_sdkctl_io_dispatcher_io, dispatcher, -1);
    return ASIO_ACTION_DONE;
}

static AsyncIOAction
_on_sdkctl_io_dispatcher_io(void* io_opaque,
                            AsyncSocketIO* asio,
                            AsyncIOState status)
{
    AsyncIOAction action = ASIO_ACTION_DONE;
    SDKCtlIODispatcher* const dispatcher = (SDKCtlIODispatcher*)io_opaque;
    SDKCtlSocket* const sdkctl = dispatcher->sdkctl;

    
    sdkctl_socket_reference(sdkctl);

    if (status != ASIO_STATE_SUCCEEDED) {
        
        switch (status) {
            case ASIO_STATE_STARTED:
                async_socket_io_cancel_time_out(asio);
                break;

            case ASIO_STATE_FAILED:
                
                _on_io_dispatcher_io_failure(dispatcher, asio);
                break;

            case ASIO_STATE_TIMED_OUT:
                _ANDROID_ASSERT(0,
                    "SDKCtl %s: We should never receive ASIO_STATE_TIMED_OUT in SDKCtl I/O dispatcher.",
                    sdkctl->service_name);
                break;

            case ASIO_STATE_CANCELLED:
                _on_io_dispatcher_io_cancelled(dispatcher, asio);
                break;

            case ASIO_STATE_FINISHED:
                break;

            default:
                _ANDROID_ASSERT(0, "SDKCtl %s: Unexpected I/O status %d in the dispatcher",
                                sdkctl->service_name, status);
                
                errno = EINVAL;
                _on_io_dispatcher_io_failure(dispatcher, asio);
                action = ASIO_ACTION_ABORT;
                break;
        }

        sdkctl_socket_release(sdkctl);

        return action;
    }

    switch (dispatcher->state) {
        case SDKCTL_IODISP_EXPECT_HEADER:
            
            action = _on_io_dispatcher_packet_header(dispatcher, asio);
            break;

        case SDKCTL_IODISP_EXPECT_QUERY_REPLY_HEADER:
            
            action = _on_io_dispatcher_query_reply_header(dispatcher, asio);
            break;

        case SDKCTL_IODISP_EXPECT_QUERY_REPLY_DATA:
            
            action = _on_io_dispatcher_query_reply(dispatcher, asio);
            break;

        case SDKCTL_IODISP_EXPECT_DATA:
            
            action = _on_io_dispatcher_packet(dispatcher, asio);
            break;

        default:
            _ANDROID_ASSERT(0, "SDKCtl %s: Unexpected I/O dispacher state %d",
                            sdkctl->service_name, dispatcher->state);
            break;
    }

    sdkctl_socket_release(sdkctl);

    return action;
}


static void
_sdkctl_socket_cancel_all_queries(SDKCtlSocket* sdkctl)
{
    SDKCtlIODispatcher* const dispatcher = &sdkctl->io_dispatcher;
    SDKCtlQuery* query;

    
    if (dispatcher->current_query != NULL) {
        SDKCtlQuery* const query = dispatcher->current_query;
        dispatcher->current_query = NULL;
        _on_sdkctl_query_cancelled(query);
        sdkctl_query_release(query);
    }

    
    query = _sdkctl_socket_pull_first_query(sdkctl);
    while (query != NULL) {
        _sdkctl_query_cancel_timeout(query);
        query->query_cb(query->query_opaque, query, ASIO_STATE_CANCELLED);
        sdkctl_query_release(query);
        query = _sdkctl_socket_pull_first_query(sdkctl);
    }
}

static void
_sdkctl_socket_cancel_all_packets(SDKCtlSocket* sdkctl)
{
}

static void
_sdkctl_socket_cancel_all_io(SDKCtlSocket* sdkctl)
{
    
    _sdkctl_socket_cancel_all_queries(sdkctl);
    _sdkctl_socket_cancel_all_packets(sdkctl);
}

static void
_sdkctl_socket_disconnect_socket(SDKCtlSocket* sdkctl)
{
    if (sdkctl->as != NULL) {
        
        async_socket_disconnect(sdkctl->as);

        
        _sdkctl_socket_cancel_all_io(sdkctl);

        
        _sdkctl_io_dispatcher_reset(sdkctl);
    }

    sdkctl->state = SDKCTL_SOCKET_DISCONNECTED;
    sdkctl->port_status = SDKCTL_PORT_DISCONNECTED;
}

static void
_sdkctl_socket_free(SDKCtlSocket* sdkctl)
{
    if (sdkctl != NULL) {
        T("SDKCtl %s: descriptor is destroing.", sdkctl->service_name);

        
        if (sdkctl->as != NULL) {
            async_socket_disconnect(sdkctl->as);
            async_socket_release(sdkctl->as);
        }

        
        if (sdkctl->looper != NULL) {
            looper_free(sdkctl->looper);
        }
        if (sdkctl->service_name != NULL) {
            free(sdkctl->service_name);
        }
        _sdkctl_socket_empty_recycler(sdkctl);

        AFREE(sdkctl);
    }
}


static void _sdkctl_do_handshake(SDKCtlSocket* sdkctl);

static AsyncIOAction
_on_async_socket_connected(SDKCtlSocket* sdkctl)
{
    D("SDKCtl %s: Socket is connected.", sdkctl->service_name);

    
    const AsyncIOAction action =
        sdkctl->on_socket_connection(sdkctl->opaque, sdkctl, ASIO_STATE_SUCCEEDED);

    if (action == ASIO_ACTION_DONE) {
        
        _sdkctl_io_dispatcher_start(sdkctl);

        
        _sdkctl_do_handshake(sdkctl);

        return action;
    } else {
        
        return action;
    }
}

static AsyncIOAction
_on_async_socket_disconnected(SDKCtlSocket* sdkctl)
{
    D("SDKCtl %s: Socket has been disconnected.", sdkctl->service_name);

    _sdkctl_socket_disconnect_socket(sdkctl);

    AsyncIOAction action = sdkctl->on_socket_connection(sdkctl->opaque, sdkctl,
                                                        ASIO_STATE_FAILED);
    if (action == ASIO_ACTION_DONE) {
        
        action = ASIO_ACTION_RETRY;
    }
    if (action == ASIO_ACTION_RETRY) {
        sdkctl->state = SDKCTL_SOCKET_CONNECTING;
    }
    return action;
}

static AsyncIOAction
_on_async_socket_connection(void* client_opaque,
                            AsyncSocket* as,
                            AsyncIOState status)
{
    AsyncIOAction action = ASIO_ACTION_DONE;
    SDKCtlSocket* const sdkctl = (SDKCtlSocket*)client_opaque;

    
    sdkctl_socket_reference(sdkctl);

    switch (status) {
        case ASIO_STATE_SUCCEEDED:
            sdkctl->state = SDKCTL_SOCKET_CONNECTED;
            _on_async_socket_connected(sdkctl);
            break;

        case ASIO_STATE_FAILED:
            if (sdkctl->state == SDKCTL_SOCKET_CONNECTED) {
                
                action = _on_async_socket_disconnected(sdkctl);
            } else {
                action = ASIO_ACTION_RETRY;
            }
            break;

        case ASIO_STATE_RETRYING:
        default:
            action = ASIO_ACTION_RETRY;
            break;
    }

    sdkctl_socket_release(sdkctl);

    return action;
}


SDKCtlSocket*
sdkctl_socket_new(int reconnect_to,
                  const char* service_name,
                  on_sdkctl_socket_connection_cb on_socket_connection,
                  on_sdkctl_port_connection_cb on_port_connection,
                  on_sdkctl_message_cb on_message,
                  void* opaque)
{
    SDKCtlSocket* sdkctl;
    ANEW0(sdkctl);

    sdkctl->state                   = SDKCTL_SOCKET_DISCONNECTED;
    sdkctl->port_status             = SDKCTL_PORT_DISCONNECTED;
    sdkctl->opaque                  = opaque;
    sdkctl->service_name            = ASTRDUP(service_name);
    sdkctl->on_socket_connection    = on_socket_connection;
    sdkctl->on_port_connection      = on_port_connection;
    sdkctl->on_message              = on_message;
    sdkctl->reconnect_to            = reconnect_to;
    sdkctl->as                      = NULL;
    sdkctl->next_query_id           = 0;
    sdkctl->query_head              = sdkctl->query_tail = NULL;
    sdkctl->ref_count               = 1;
    sdkctl->recycler                = NULL;
    sdkctl->recycler_block_size     = 0;
    sdkctl->recycler_max            = 0;
    sdkctl->recycler_count          = 0;

    T("SDKCtl %s: descriptor is created.", sdkctl->service_name);

    sdkctl->looper = looper_newCore();
    if (sdkctl->looper == NULL) {
        E("Unable to create I/O looper for SDKCtl socket '%s'",
          service_name);
        on_socket_connection(opaque, sdkctl, ASIO_STATE_FAILED);
        _sdkctl_socket_free(sdkctl);
        return NULL;
    }

    return sdkctl;
}

int sdkctl_socket_reference(SDKCtlSocket* sdkctl)
{
    assert(sdkctl->ref_count > 0);
    sdkctl->ref_count++;
    return sdkctl->ref_count;
}

int
sdkctl_socket_release(SDKCtlSocket* sdkctl)
{
    assert(sdkctl->ref_count > 0);
    sdkctl->ref_count--;
    if (sdkctl->ref_count == 0) {
        
        _sdkctl_socket_free(sdkctl);
        return 0;
    }
    return sdkctl->ref_count;
}

void
sdkctl_init_recycler(SDKCtlSocket* sdkctl,
                     uint32_t data_size,
                     int max_recycled_num)
{
    if (sdkctl->recycler != NULL) {
        D("SDKCtl %s: Recycler is already initialized. Ignoring recycler init.",
          sdkctl->service_name);
        return;
    }

    
    data_size += sizeof(SDKCtlQuery);

    sdkctl->recycler_block_size = data_size;
    sdkctl->recycler_max        = max_recycled_num;
    sdkctl->recycler_count      = 0;
}

void
sdkctl_socket_connect(SDKCtlSocket* sdkctl, int port, int retry_to)
{
    T("SDKCtl %s: Handling connect request to port %d, retrying in %dms...",
      sdkctl->service_name, port, retry_to);

    sdkctl->state = SDKCTL_SOCKET_CONNECTING;
    sdkctl->as = async_socket_new(port, sdkctl->reconnect_to,
                                  _on_async_socket_connection, sdkctl,
                                  sdkctl->looper);
    if (sdkctl->as == NULL) {
        E("Unable to allocate AsyncSocket for SDKCtl socket '%s'",
           sdkctl->service_name);
        sdkctl->on_socket_connection(sdkctl->opaque, sdkctl, ASIO_STATE_FAILED);
    } else {
        async_socket_connect(sdkctl->as, retry_to);
    }
}

void
sdkctl_socket_reconnect(SDKCtlSocket* sdkctl, int port, int retry_to)
{
    T("SDKCtl %s: Handling reconnection request to port %d, retrying in %dms...",
      sdkctl->service_name, port, retry_to);

    _sdkctl_socket_disconnect_socket(sdkctl);

    if (sdkctl->as == NULL) {
        sdkctl_socket_connect(sdkctl, port, retry_to);
    } else {
        sdkctl->state = SDKCTL_SOCKET_CONNECTING;
        async_socket_reconnect(sdkctl->as, retry_to);
    }
}

void
sdkctl_socket_disconnect(SDKCtlSocket* sdkctl)
{
    T("SDKCtl %s: Handling disconnect request.", sdkctl->service_name);

    _sdkctl_socket_disconnect_socket(sdkctl);
}

int
sdkctl_socket_is_connected(SDKCtlSocket* sdkctl)
{
    return (sdkctl->state == SDKCTL_SOCKET_CONNECTED) ? 1 : 0;
}

int
sdkctl_socket_is_port_ready(SDKCtlSocket* sdkctl)
{
    return (sdkctl->port_status == SDKCTL_PORT_ENABLED) ? 1 : 0;
}

SdkCtlPortStatus
sdkctl_socket_get_port_status(SDKCtlSocket* sdkctl)
{
    return sdkctl->port_status;
}

int
sdkctl_socket_is_handshake_ok(SDKCtlSocket* sdkctl)
{
    switch (sdkctl->port_status) {
        case SDKCTL_HANDSHAKE_DUP:
        case SDKCTL_HANDSHAKE_UNKNOWN_QUERY:
        case SDKCTL_HANDSHAKE_UNKNOWN_RESPONSE:
            return 0;
        default:
        return 1;
    }
}



#define SDKCTL_HANDSHAKE_RESP_CONNECTED         0
#define SDKCTL_HANDSHAKE_RESP_NOPORT            1
#define SDKCTL_HANDSHAKE_RESP_DUP               -1
#define SDKCTL_HANDSHAKE_RESP_QUERY_UNKNOWN     -2

static AsyncIOAction
_on_handshake_io(void* query_opaque,
                 SDKCtlQuery* query,
                 AsyncIOState status)
{
    SDKCtlSocket* const sdkctl = (SDKCtlSocket*)query_opaque;

    if (status == ASIO_STATE_SUCCEEDED) {
        const int* res = (const int*)(*query->response_buffer);
        SdkCtlPortStatus handshake_status;
        switch (*res) {
            case SDKCTL_HANDSHAKE_RESP_CONNECTED:
                D("SDKCtl %s: Handshake succeeded. Port is connected",
                  sdkctl->service_name);
                handshake_status = SDKCTL_HANDSHAKE_CONNECTED;
                break;

            case SDKCTL_HANDSHAKE_RESP_NOPORT:
                D("SDKCtl %s: Handshake succeeded. Port is not connected",
                  sdkctl->service_name);
                handshake_status = SDKCTL_HANDSHAKE_NO_PORT;
                break;

            case SDKCTL_HANDSHAKE_RESP_DUP:
                D("SDKCtl %s: Handshake failed: duplicate connection.",
                  sdkctl->service_name);
                handshake_status = SDKCTL_HANDSHAKE_DUP;
                break;

            case SDKCTL_HANDSHAKE_RESP_QUERY_UNKNOWN:
                D("SDKCtl %s: Handshake failed: unknown query.",
                  sdkctl->service_name);
                handshake_status = SDKCTL_HANDSHAKE_UNKNOWN_QUERY;
                break;

            default:
                E("SDKCtl %s: Unknown handshake response: %d",
                  sdkctl->service_name, *res);
                handshake_status = SDKCTL_HANDSHAKE_UNKNOWN_RESPONSE;
                break;
        }
        sdkctl->port_status = handshake_status;
        sdkctl->on_port_connection(sdkctl->opaque, sdkctl, handshake_status);
    } else {
        
        switch (status) {
            case ASIO_STATE_FAILED:
            case ASIO_STATE_TIMED_OUT:
            case ASIO_STATE_CANCELLED:
              D("SDKCtl %s: Handshake failed: I/O state %d. Error: %d -> %s",
                sdkctl->service_name, status, errno, strerror(errno));
                sdkctl->on_socket_connection(sdkctl->opaque, sdkctl,
                                             ASIO_STATE_FAILED);
                break;

            default:
                break;
        }
    }
    return ASIO_ACTION_DONE;
}

static AsyncIOAction
_on_sdkctl_endianness_io(void* io_opaque,
                         AsyncSocketIO* asio,
                         AsyncIOState status) {
    SDKCtlSocket* const sdkctl = (SDKCtlSocket*)io_opaque;

    if (status == ASIO_STATE_SUCCEEDED) {
        
        D("SDKCtl %s: Sending handshake query...", sdkctl->service_name);
        SDKCtlQuery* query =
            sdkctl_query_build_and_send(sdkctl, SDKCTL_QUERY_HANDSHAKE,
                                        strlen(sdkctl->service_name),
                                        sdkctl->service_name, NULL, NULL,
                                        _on_handshake_io, sdkctl, 3000);
        sdkctl_query_release(query);
        return ASIO_ACTION_DONE;
    } else {
        
        switch (status) {
                case ASIO_STATE_FAILED:
                case ASIO_STATE_TIMED_OUT:
                case ASIO_STATE_CANCELLED:
                  D("SDKCtl %s: endianness failed: I/O state %d. Error: %d -> %s",
                    sdkctl->service_name, status, errno, strerror(errno));
                    sdkctl->on_socket_connection(sdkctl->opaque, sdkctl, ASIO_STATE_FAILED);
                    break;

                default:
                    break;
        }
    }
    return ASIO_ACTION_DONE;
}

static void
_sdkctl_do_handshake(SDKCtlSocket* sdkctl)
{
#ifndef HOST_WORDS_BIGENDIAN
static const char _host_end = 0;
#else
static const char _host_end = 1;
#endif

    D("SDKCtl %s: Sending endianness: %d", sdkctl->service_name, _host_end);

    async_socket_write_rel(sdkctl->as, &_host_end, 1,
                           _on_sdkctl_endianness_io, sdkctl, 3000);
}

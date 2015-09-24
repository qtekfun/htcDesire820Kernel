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

#ifndef ANDROID_SDKCONTROL_SOCKET_H_
#define ANDROID_SDKCONTROL_SOCKET_H_

#include "android/async-socket.h"
#include "android/async-utils.h"


#define SDKCTL_DEFAULT_TCP_PORT     1970

typedef struct SDKCtlSocket SDKCtlSocket;

typedef struct SDKCtlMessage SDKCtlMessage;

typedef struct SDKCtlQuery SDKCtlQuery;

typedef struct SDKCtlDirectPacket SDKCtlDirectPacket;

typedef AsyncIOAction (*on_sdkctl_socket_connection_cb)(void* client_opaque,
                                                        SDKCtlSocket* sdkctl,
                                                        AsyncIOState status);

typedef enum SdkCtlPortStatus {
    
    SDKCTL_PORT_DISCONNECTED,
    
    SDKCTL_PORT_CONNECTED,
    
    SDKCTL_PORT_ENABLED,
    
    SDKCTL_PORT_DISABLED,
    
    SDKCTL_HANDSHAKE_CONNECTED,
    
    SDKCTL_HANDSHAKE_NO_PORT,
    
    SDKCTL_HANDSHAKE_DUP,
    
    SDKCTL_HANDSHAKE_UNKNOWN_QUERY,
    
    SDKCTL_HANDSHAKE_UNKNOWN_RESPONSE,
} SdkCtlPortStatus;

typedef void (*on_sdkctl_port_connection_cb)(void* client_opaque,
                                             SDKCtlSocket* sdkctl,
                                             SdkCtlPortStatus status);

typedef void (*on_sdkctl_message_cb)(void* client_opaque,
                                     SDKCtlSocket* sdkctl,
                                     SDKCtlMessage* message,
                                     int msg_type,
                                     void* msg_data,
                                     int msg_size);

typedef AsyncIOAction (*on_sdkctl_query_cb)(void* query_opaque,
                                            SDKCtlQuery* query,
                                            AsyncIOState status);

typedef AsyncIOAction (*on_sdkctl_direct_cb)(void* opaque,
                                             SDKCtlDirectPacket* packet,
                                             AsyncIOState status);


extern SDKCtlDirectPacket* sdkctl_direct_packet_new(SDKCtlSocket* sdkctl);

extern int sdkctl_direct_packet_reference(SDKCtlDirectPacket* packet);

extern int sdkctl_direct_packet_release(SDKCtlDirectPacket* packet);

extern void sdkctl_direct_packet_send(SDKCtlDirectPacket* packet,
                                      void* data,
                                      on_sdkctl_direct_cb cb,
                                      void* cb_opaque);


extern int sdkctl_message_reference(SDKCtlMessage* msg);

extern int sdkctl_message_release(SDKCtlMessage* msg);

extern SDKCtlMessage* sdkctl_message_send(SDKCtlSocket* sdkctl,
                                          int msg_type,
                                          const void* data,
                                          uint32_t size);

extern int sdkctl_message_get_header_size(void);

extern void sdkctl_init_message_header(void* msg, int msg_type, int msg_size);


extern SDKCtlQuery* sdkctl_query_new(SDKCtlSocket* sdkctl,
                                     int query_type,
                                     uint32_t in_data_size);

extern SDKCtlQuery* sdkctl_query_new_ex(SDKCtlSocket* sdkctl,
                                        int query_type,
                                        uint32_t in_data_size,
                                        const void* in_data,
                                        void** response_buffer,
                                        uint32_t* response_size,
                                        on_sdkctl_query_cb query_cb,
                                        void* query_opaque);

extern void sdkctl_query_send(SDKCtlQuery* query, int to);

extern SDKCtlQuery* sdkctl_query_build_and_send(SDKCtlSocket* sdkctl,
                                                int query_type,
                                                uint32_t in_data_size,
                                                const void* in_data,
                                                void** response_buffer,
                                                uint32_t* response_size,
                                                on_sdkctl_query_cb query_cb,
                                                void* query_opaque,
                                                int to);

extern int sdkctl_query_reference(SDKCtlQuery* query);

extern int sdkctl_query_release(SDKCtlQuery* query);

extern void* sdkctl_query_get_buffer_in(SDKCtlQuery* query);

extern void* sdkctl_query_get_buffer_out(SDKCtlQuery* query);


extern SDKCtlSocket* sdkctl_socket_new(int reconnect_to,
                                       const char* service_name,
                                       on_sdkctl_socket_connection_cb on_socket_connection,
                                       on_sdkctl_port_connection_cb on_port_connection,
                                       on_sdkctl_message_cb on_message,
                                       void* opaque);

extern void sdkctl_init_recycler(SDKCtlSocket* sdkctl,
                                 uint32_t data_size,
                                 int max_recycled_num);

extern int sdkctl_socket_reference(SDKCtlSocket* sdkctl);

extern int sdkctl_socket_release(SDKCtlSocket* sdkctl);

extern void sdkctl_socket_connect(SDKCtlSocket* sdkctl, int port, int retry_to);

extern void sdkctl_socket_reconnect(SDKCtlSocket* sdkctl, int port, int retry_to);

extern void sdkctl_socket_disconnect(SDKCtlSocket* sdkctl);

extern int sdkctl_socket_is_connected(SDKCtlSocket* sdkctl);

extern int sdkctl_socket_is_port_ready(SDKCtlSocket* sdkctl);

extern SdkCtlPortStatus sdkctl_socket_get_port_status(SDKCtlSocket* sdkctl);

extern int sdkctl_socket_is_handshake_ok(SDKCtlSocket* sdkctl);

#endif  

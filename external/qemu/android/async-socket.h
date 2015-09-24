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

#ifndef ANDROID_ASYNC_SOCKET_H_
#define ANDROID_ASYNC_SOCKET_H_

#include "qemu-common.h"
#include "android/async-io-common.h"
#include "android/async-utils.h"


typedef struct AsyncSocket AsyncSocket;

typedef struct AsyncSocketIO AsyncSocketIO;

typedef AsyncIOAction (*on_as_connection_cb)(void* client_opaque,
                                             AsyncSocket* as,
                                             AsyncIOState status);

typedef AsyncIOAction (*on_as_io_cb)(void* io_opaque,
                                     AsyncSocketIO* asio,
                                     AsyncIOState status);


extern int async_socket_io_reference(AsyncSocketIO* asio);

extern int async_socket_io_release(AsyncSocketIO* asio);

extern AsyncSocket* async_socket_io_get_socket(const AsyncSocketIO* asio);

extern void async_socket_io_cancel_time_out(AsyncSocketIO* asio);

extern void* async_socket_io_get_io_opaque(const AsyncSocketIO* asio);

extern void* async_socket_io_get_client_opaque(const AsyncSocketIO* asio);

extern void* async_socket_io_get_buffer_info(const AsyncSocketIO* asio,
                                             uint32_t* transferred,
                                             uint32_t* to_transfer);

extern void* async_socket_io_get_buffer(const AsyncSocketIO* asio);

extern uint32_t async_socket_io_get_transferred(const AsyncSocketIO* asio);

extern uint32_t async_socket_io_get_to_transfer(const AsyncSocketIO* asio);

extern int async_socket_io_is_read(const AsyncSocketIO* asio);


extern AsyncSocket* async_socket_new(int port,
                                     int reconnect_to,
                                     on_as_connection_cb connect_cb,
                                     void* client_opaque,
                                     Looper* looper);

extern int async_socket_reference(AsyncSocket* as);

extern int async_socket_release(AsyncSocket* as);

extern void async_socket_connect(AsyncSocket* as, int retry_to);

extern void async_socket_disconnect(AsyncSocket* as);

extern void async_socket_reconnect(AsyncSocket* as, int retry_to);

extern void async_socket_read_abs(AsyncSocket* as,
                                  void* buffer, uint32_t len,
                                  on_as_io_cb reader_cb,
                                  void* reader_opaque,
                                  Duration deadline);

extern void async_socket_read_rel(AsyncSocket* as,
                                  void* buffer, uint32_t len,
                                  on_as_io_cb reader_cb,
                                  void* reader_opaque,
                                  int to);

extern void async_socket_write_abs(AsyncSocket* as,
                                   const void* buffer, uint32_t len,
                                   on_as_io_cb writer_cb,
                                   void* writer_opaque,
                                   Duration deadline);

extern void async_socket_write_rel(AsyncSocket* as,
                                   const void* buffer, uint32_t len,
                                   on_as_io_cb writer_cb,
                                   void* writer_opaque,
                                   int to);

extern Duration async_socket_deadline(AsyncSocket* as, int rel);

extern void* async_socket_get_client_opaque(const AsyncSocket* as);

extern int async_socket_get_port(const AsyncSocket* as);

extern int async_socket_is_connected(const AsyncSocket* as);

#endif  

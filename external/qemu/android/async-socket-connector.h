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

#ifndef ANDROID_ASYNC_SOCKET_CONNECTOR_H_
#define ANDROID_ASYNC_SOCKET_CONNECTOR_H_

#include "qemu-common.h"
#include "android/async-io-common.h"
#include "android/async-utils.h"


typedef struct AsyncSocketConnector AsyncSocketConnector;

typedef AsyncIOAction (*asc_event_cb)(void* opaque,
                                      AsyncSocketConnector* connector,
                                      AsyncIOState event);

extern AsyncSocketConnector* async_socket_connector_new(const SockAddress* address,
                                                        int retry_to,
                                                        asc_event_cb cb,
                                                        void* cb_opaque,
                                                        Looper* looper);

extern int async_socket_connector_reference(AsyncSocketConnector* connector);

extern int async_socket_connector_release(AsyncSocketConnector* connector);

extern void async_socket_connector_connect(AsyncSocketConnector* connector);

extern int async_socket_connector_pull_fd(AsyncSocketConnector* connector);

#endif  

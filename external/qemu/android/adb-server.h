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

#ifndef ANDROID_ADB_SERVER_H_
#define ANDROID_ADB_SERVER_H_


typedef void (*adbguest_connect)(void* opaque, void* connection);

typedef void (*adbguest_read)(void* opaque,
                              void* connection,
                              const void* buff,
                              int size);

typedef void (*adbguest_disconnect)(void* opaque, void* connection);

typedef struct AdbGuestRoutines AdbGuestRoutines;
struct AdbGuestRoutines {
    
    adbguest_connect     on_connected;
    
    adbguest_disconnect  on_disconnect;
    
    adbguest_read        on_read;
};

extern int adb_server_init(int port);

extern int adb_server_is_initialized(void);

extern void* adb_server_register_guest(void* opaque, AdbGuestRoutines* callbacks);

extern void adb_server_complete_connection(void* opaque);

extern void adb_server_on_guest_message(void* opaque,
                                        const uint8_t* data,
                                        int size);

extern void adb_server_on_guest_closed(void* opaque);

#endif  

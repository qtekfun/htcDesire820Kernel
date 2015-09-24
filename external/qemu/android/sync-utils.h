/*
 * Copyright (C) 2010 The Android Open Source Project
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


#ifndef ANDROID_SYNC_UTILS_H
#define ANDROID_SYNC_UTILS_H

#include "android/android.h"
#include "sockets.h"

typedef struct SyncSocket SyncSocket;

SyncSocket* syncsocket_connect(int fd, SockAddress* sockaddr, int timeout);

SyncSocket* syncsocket_init(int fd);

void syncsocket_close(SyncSocket* ssocket);

void syncsocket_free(SyncSocket* ssocket);

int syncsocket_start_read(SyncSocket* ssocket);

int syncsocket_stop_read(SyncSocket* ssocket);

int syncsocket_start_write(SyncSocket* ssocket);

/*
 * Clears the socket after writing.
 * Note: this routine must be called after all data has been written to the
 * socket.
 * Param:
 *  ssocket - SyncSocket descriptor obtained from syncsocket_connect routine.
 * Return:
 *  0 on success, or -1 on failure.
 */
int syncsocket_stop_write(SyncSocket* ssocket);

ssize_t syncsocket_read_absolute(SyncSocket* ssocket,
                                 void* buf,
                                 size_t size,
                                 int64_t deadline);

ssize_t syncsocket_read(SyncSocket* ssocket, void* buf, size_t size, int timeout);

/*
 * Synchronously writes to the socket.
 * Note: syncsocket_start_write must be called before first call to this routine.
 * Once syncsocket_start_write has been called, multiple syncsocket_write_xxx can
 * be called to write all necessary data to the socket. When all necessary data
 * has been written, syncsocket_stop_write must be called.
 * Param:
 *  ssocket - SyncSocket descriptor obtained from syncsocket_connect routine.
 *  buf - Buffer containing data to write.
 *  size - Number of bytes to write.
 *  deadline - Absoulte deadline time to complete the writing.
 * Return:
 *  Number of bytes written on success,or -1 on failure.
 */
ssize_t syncsocket_write_absolute(SyncSocket* ssocket,
                                  const void* buf,
                                  size_t size,
                                  int64_t deadline);

/*
 * Synchronously writes to the socket.
 * Note: syncsocket_start_write must be called before first call to this routine.
 * Once syncsocket_start_write has been called, multiple syncsocket_write_xxx can
 * be called to write all necessary data to the socket. When all necessary data
 * has been written, syncsocket_stop_write must be called.
 * Param:
 *  ssocket - SyncSocket descriptor obtained from syncsocket_connect routine.
 *  buf - Buffer containing data to write.
 *  size - Number of bytes to write.
 *  timeout - Timeout (in milliseconds) to complete the writing.
 * Return:
 *  Number of bytes written on success, or -1 on failure.
 */
ssize_t syncsocket_write(SyncSocket* ssocket,
                         const void* buf,
                         size_t size,
                         int timeout);

ssize_t syncsocket_read_line_absolute(SyncSocket* ssocket,
                                      char* buffer,
                                      size_t size,
                                      int64_t deadline);

ssize_t syncsocket_read_line(SyncSocket* ssocket,
                             char* buffer,
                             size_t size,
                             int timeout);

int syncsocket_get_socket(SyncSocket* ssocket);

static inline int
syncsocket_result(int status)
{
    if (status == 0) {
        
        
        status = -1;
    } else if (status > 0) {
        
        status = 0;
    }
    return status;
}

#endif  

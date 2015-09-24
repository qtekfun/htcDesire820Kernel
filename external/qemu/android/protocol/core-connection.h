/* Copyright (C) 2010 The Android Open Source Project
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


#ifndef QEMU_ANDROID_CORE_CONNECTION_H
#define QEMU_ANDROID_CORE_CONNECTION_H

#include "android/sync-utils.h"

typedef struct CoreConnection CoreConnection;

#define CORE_BASE_PORT          5554

#define MAX_CORE_PROCS          16

#define CORE_PORT_TIMEOUT_MS    5000

SyncSocket* core_connection_open_socket(SockAddress* sockaddr);

CoreConnection* core_connection_create(SockAddress* console_socket);

void core_connection_free(CoreConnection* desc);

int core_connection_open(CoreConnection* desc);

void core_connection_close(CoreConnection* desc);

/* Synchronously writes to the console. See CORE_PORT_TIMEOUT_MS for the timeout
 * value used to wait for the write operation to complete.
 * Param:
 *  desc Console client descriptor opened with core_connection_open.
 *      buffer Buffer to write.
 *  to_write Number of bytes to write.
 *  written_bytes Upon success, contains number of bytes written. This parameter
 *      is optional, and can be NULL.
 * Return:
 *  0 on success, or -1 on failure.
 */
int core_connection_write(CoreConnection* desc,
                          const void* buffer,
                          size_t to_write,
                          size_t* written_bytes);

int core_connection_read(CoreConnection* desc,
                         void* buffer,
                         size_t to_read,
                         size_t* read_bytes);

int core_connection_switch_stream(CoreConnection* desc,
                                  const char* stream_name,
                                  char** handshake);

CoreConnection* core_connection_create_and_switch(SockAddress* console_socket,
                                                  const char* stream_name,
                                                  char** handshake);

void core_connection_detach(CoreConnection* desc);

int core_connection_get_socket(CoreConnection* desc);

static inline int
core_connection_get_timeout(size_t data_size)
{
    
    
    return 2000 + data_size * 10;
}

#endif  

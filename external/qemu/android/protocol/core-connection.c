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

#include <unistd.h>

#include "sockets.h"
#include "qemu-common.h"
#include "errno.h"
#include "iolooper.h"
#include "android/android.h"
#include "android/utils/debug.h"
#include "android/globals.h"
#include "android/utils/system.h"
#include "android/protocol/core-connection.h"

struct CoreConnection {
    
    SockAddress console_address;

    
    SyncSocket* ssocket;

    char* stream_name;
};

static inline void
_zero_terminate(char* buf, size_t buf_size, size_t eos)
{
    if (eos < buf_size) {
        buf[eos] = '\0';
    } else {
        buf[buf_size - 1] = '\0';
    }
}

static int
_is_reply_ok(const char* reply, int reply_size)
{
    return (reply_size < 2) ? 0 : (reply[0] == 'O' && reply[1] == 'K');
}

static int
_is_reply_ko(const char* reply, int reply_size)
{
    return (reply_size < 2) ? 0 : (reply[0] == 'K' && reply[1] == 'O');
}

SyncSocket*
core_connection_open_socket(SockAddress* sockaddr)
{
    SyncSocket* ssocket;
    int status;
    int64_t deadline;
    char buf[512];

    int fd = socket_create(sock_address_get_family(sockaddr), SOCKET_STREAM);
    if (fd < 0) {
        return NULL;
    }

    socket_set_xreuseaddr(fd);

    
    ssocket = syncsocket_connect(fd, sockaddr, CORE_PORT_TIMEOUT_MS);
    if (ssocket == NULL) {
        derror("syncsocket_connect has failed: %s\n", errno_str);
        socket_close(fd);
        return NULL;
    }

    
    
    status = syncsocket_start_read(ssocket);
    if (status < 0) {
        derror("syncsocket_start_read has failed: %s\n", errno_str);
        syncsocket_free(ssocket);
        return NULL;
    }

    deadline = iolooper_now() + CORE_PORT_TIMEOUT_MS;
    
    status = syncsocket_read_line_absolute(ssocket, buf, sizeof(buf), deadline);
    if (status <= 0) {
        derror("syncsocket_read_line_absolute has failed: %s\n", errno_str);
        syncsocket_free(ssocket);
        return NULL;
    }
    if (status < 15 || memcmp(buf, "Android Console", 15)) {
        _zero_terminate(buf, sizeof(buf), status);
        derror("console has failed the connection: %s\n", buf);
        syncsocket_free(ssocket);
        return NULL;
    }
    
    status = syncsocket_read_line_absolute(ssocket, buf, sizeof(buf), deadline);
    syncsocket_stop_read(ssocket);
    if (status < 2 || !_is_reply_ok(buf, status)) {
        _zero_terminate(buf, sizeof(buf), status);
        derror("unexpected reply from the console: %s\n", buf);
        syncsocket_free(ssocket);
        return NULL;
    }

    return ssocket;
}

CoreConnection*
core_connection_create(SockAddress* console_address)
{
    CoreConnection* desc;
    ANEW0(desc);
    desc->console_address = console_address[0];
    desc->ssocket = NULL;
    desc->stream_name = NULL;

    return desc;
}

void
core_connection_free(CoreConnection* desc)
{
    if (desc == NULL) {
        return;
    }
    if (desc->ssocket != NULL) {
        syncsocket_free(desc->ssocket);
    }
    if (desc->stream_name != NULL) {
        free(desc->stream_name);
    }
    free(desc);
}

int
core_connection_open(CoreConnection* desc)
{
    if (desc == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (desc->ssocket != NULL) {
        return 0;
    }

    desc->ssocket = core_connection_open_socket(&desc->console_address);

    return (desc->ssocket != NULL) ? 0 : -1;
}

void
core_connection_close(CoreConnection* desc)
{
    if (desc == NULL) {
        return;
    }
    if (desc->ssocket != NULL) {
        syncsocket_close(desc->ssocket);
    }
}

int
core_connection_write(CoreConnection* desc,
                      const void* buffer,
                      size_t to_write,
                      size_t* written_bytes)
{
    ssize_t written;

    int status = syncsocket_start_write(desc->ssocket);
    if (status < 0) {
        derror("syncsocket_start_write failed: %s\n", errno_str);
        return status;
    }

    written =
        syncsocket_write(desc->ssocket, buffer, to_write, CORE_PORT_TIMEOUT_MS);
    syncsocket_stop_write(desc->ssocket);
    if (written <= 0) {
        derror("syncsocket_write failed: %s\n", errno_str);
        return -1;
    }
    if (written_bytes != NULL) {
        *written_bytes = written;
    }

    return 0;
}

int
core_connection_read(CoreConnection* desc,
                     void* buffer,
                     size_t to_read,
                     size_t* read_bytes)
{
    ssize_t read_size;

    int status = syncsocket_start_read(desc->ssocket);
    if (status < 0) {
        derror("syncsocket_start_read failed: %s\n", errno_str);
        return status;
    }

    read_size =
        syncsocket_read(desc->ssocket, buffer, to_read, CORE_PORT_TIMEOUT_MS);
    syncsocket_stop_read(desc->ssocket);
    if (read_size <= 0) {
        derror("syncsocket_read failed: %s\n", errno_str);
        return -1;
    }

    if (read_bytes != NULL) {
        *read_bytes = read_size;
    }
    return 0;
}

int
core_connection_switch_stream(CoreConnection* desc,
                              const char* stream_name,
                              char** handshake)
{
    char buf[4096];
    int handshake_len;
    int status;
    int64_t deadline;

    *handshake = NULL;
    if (desc == NULL || desc->stream_name != NULL || stream_name == NULL) {
        errno = EINVAL;
        return -1;
    }

    
    snprintf(buf, sizeof(buf), "qemu %s\r\n", stream_name);
    if (core_connection_write(desc, buf, strlen(buf), NULL)) {
        return -1;
    }

    
    status = syncsocket_start_read(desc->ssocket);
    if (status < 0) {
        return -1;
    }
    deadline = iolooper_now() + CORE_PORT_TIMEOUT_MS;
    handshake_len =
        syncsocket_read_line_absolute(desc->ssocket, buf, sizeof(buf), deadline);
    _zero_terminate(buf, sizeof(buf), handshake_len);
    
    if (handshake_len >= 1) {
        if (buf[handshake_len - 1] == '\r' || buf[handshake_len - 1] == '\n') {
            buf[handshake_len - 1] = '\0';
            if (handshake_len >= 2 && (buf[handshake_len - 2] == '\r' ||
                                       buf[handshake_len - 2] == '\n')) {
                buf[handshake_len - 2] = '\0';
            }
        }
    }
    
    if (_is_reply_ok(buf, handshake_len)) {
        *handshake = strdup(buf + 3);
        desc->stream_name = strdup(stream_name);
        
        status = syncsocket_read_line_absolute(desc->ssocket, buf, sizeof(buf),
                                               deadline);
        syncsocket_stop_read(desc->ssocket);
        if (status < 0) {
            derror("error reading console reply on stream switch: %s\n", errno_str);
            return -1;
        } else if (!_is_reply_ok(buf, status)) {
            _zero_terminate(buf, sizeof(buf), status);
            derror("unexpected console reply when switching streams: %s\n", buf);
            return -1;
        }
        return 0;
    } else if (_is_reply_ko(buf, handshake_len)) {
        derror("console has rejected stream switch: %s\n", buf);
        syncsocket_stop_read(desc->ssocket);
        *handshake = strdup(buf + 3);
        return -1;
    } else {
        
        derror("unexpected console reply when switching streams: %s\n", buf);
        syncsocket_stop_read(desc->ssocket);
        *handshake = strdup(buf);
        return -1;
    }
}

CoreConnection*
core_connection_create_and_switch(SockAddress* console_socket,
                                  const char* stream_name,
                                  char** handshake)
{
    char switch_cmd[256];
    CoreConnection* connection = NULL;

    
    connection = core_connection_create(console_socket);
    if (connection == NULL) {
        return NULL;
    }
    if (core_connection_open(connection)) {
        core_connection_free(connection);
        return NULL;
    }

    
    snprintf(switch_cmd, sizeof(switch_cmd), "%s", stream_name);
    if (core_connection_switch_stream(connection, switch_cmd, handshake)) {
        core_connection_close(connection);
        core_connection_free(connection);
        return NULL;
    }

    return connection;
}

void
core_connection_detach(CoreConnection* desc)
{
    core_connection_write(desc, "\n", 1, NULL);
}

int
core_connection_get_socket(CoreConnection* desc)
{
    return (desc != NULL) ? syncsocket_get_socket(desc->ssocket) : -1;
}

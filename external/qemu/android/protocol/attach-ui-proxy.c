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


#include "android/android.h"
#include "android/globals.h"
#include "android/looper.h"
#include "android/async-utils.h"
#include "android/sync-utils.h"
#include "android/utils/debug.h"
#include "android/protocol/core-commands.h"
#include "android/protocol/core-commands-impl.h"

typedef struct AttachUIProxy {
    
    AsyncReader         async_reader;

    
    LoopIo              io;

    
    Looper*             looper;

    
    int                 sock;
} AttachUIProxy;

static AttachUIProxy    _attachUiProxy;

extern void destroy_attach_ui_client(void);

static void
_attachUiProxy_io_func(void* opaque, int fd, unsigned events)
{
    AttachUIProxy* uicmd = (AttachUIProxy*)opaque;
    AsyncReader reader;
    AsyncStatus status;
    uint8_t read_buf[1];

    if (events & LOOP_IO_WRITE) {
        derror("Unexpected LOOP_IO_WRITE in _attachUiProxy_io_func.\n");
        return;
    }

    
    asyncReader_init(&reader, read_buf, sizeof(read_buf), &uicmd->io);
    status = asyncReader_read(&reader);
    
    if (status != ASYNC_ERROR) {
        derror("Unexpected read status %d in _attachUiProxy_io_func\n", status);
        return;
    }
    
    if (errno != ECONNRESET) {
        derror("Unexpected read error %d (%s) in _attachUiProxy_io_func.\n",
               errno, errno_str);
        return;
    }

    
    destroy_attach_ui_client();
}

int
attachUiProxy_create(int fd)
{
    
    _attachUiProxy.sock = fd;
    _attachUiProxy.looper = looper_newCore();
    loopIo_init(&_attachUiProxy.io, _attachUiProxy.looper, _attachUiProxy.sock,
                _attachUiProxy_io_func, &_attachUiProxy);
    loopIo_wantRead(&_attachUiProxy.io);

    return 0;
}

void
attachUiProxy_destroy(void)
{
    if (_attachUiProxy.looper != NULL) {
        
        loopIo_done(&_attachUiProxy.io);
        looper_free(_attachUiProxy.looper);
        _attachUiProxy.looper = NULL;
    }
    _attachUiProxy.sock = -1;
}

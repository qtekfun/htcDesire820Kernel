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
#include "android/hw-control.h"
#include "android/looper.h"
#include "android/async-utils.h"
#include "android/sync-utils.h"
#include "android/utils/debug.h"
#include "android/protocol/ui-commands.h"
#include "android/protocol/ui-commands-proxy.h"
#include "android/protocol/ui-commands-api.h"

typedef struct UICmdProxy {
    
    LoopIo          io;

    
    Looper*         looper;

    
    SyncSocket*     sync_writer;

    
    int             sock;
} UICmdProxy;

static UICmdProxy    _uiCmdProxy;

extern void destroy_uicmd_client(void);

static int
_uiCmdProxy_get_timeout(size_t data_size)
{
    
    
    return 2000 + data_size * 10;
}

static int
_uiCmdProxy_send_command(uint8_t cmd_type,
                         void* cmd_param,
                         uint32_t cmd_param_size)
{
    UICmdHeader header;
    int status = syncsocket_start_write(_uiCmdProxy.sync_writer);
    if (!status) {
        
        header.cmd_type = cmd_type;
        header.cmd_param_size = cmd_param_size;
        status = syncsocket_write(_uiCmdProxy.sync_writer, &header, sizeof(header),
                                  _uiCmdProxy_get_timeout(sizeof(header)));
        
        if (status > 0 && cmd_param != NULL && cmd_param_size > 0) {
            status = syncsocket_write(_uiCmdProxy.sync_writer, cmd_param,
                                      cmd_param_size,
                                      _uiCmdProxy_get_timeout(cmd_param_size));
        }
        status = syncsocket_result(status);
        syncsocket_stop_write(_uiCmdProxy.sync_writer);
    }
    if (status < 0) {
        derror("Send UI command %d (%u bytes) has failed: %s\n",
               cmd_type, cmd_param_size, errno_str);
    }
    return status;
}

static void
_uiCmdProxy_io_func(void* opaque, int fd, unsigned events)
{
    UICmdProxy* uicmd = (UICmdProxy*)opaque;
    AsyncReader reader;
    AsyncStatus status;
    uint8_t read_buf[1];

    if (events & LOOP_IO_WRITE) {
        derror("Unexpected LOOP_IO_WRITE in _uiCmdProxy_io_func.\n");
        return;
    }

    
    asyncReader_init(&reader, read_buf, sizeof(read_buf), &uicmd->io);
    status = asyncReader_read(&reader);
    
    if (status != ASYNC_ERROR) {
        derror("Unexpected read status %d in _uiCmdProxy_io_func\n", status);
        return;
    }
    
    if (errno != ECONNRESET) {
        derror("Unexpected read error %d (%s) in _uiCmdProxy_io_func.\n",
               errno, errno_str);
        return;
    }

    
    destroy_uicmd_client();
}
static void
_uiCmdProxy_brightness_change_callback(void* opaque,
                                       const char* light,
                                       int brightness)
{
    
    const size_t cmd_size = sizeof(UICmdChangeDispBrightness) + strlen(light) + 1;
    
    UICmdChangeDispBrightness* cmd =
        (UICmdChangeDispBrightness*)qemu_malloc(cmd_size);
    cmd->brightness = brightness;
    strcpy(cmd->light, light);
    
    _uiCmdProxy_send_command(AUICMD_CHANGE_DISP_BRIGHTNESS, cmd, cmd_size);
    qemu_free(cmd);
}

int
uiCmdProxy_create(int fd)
{
    
    _uiCmdProxy.sock = fd;
    _uiCmdProxy.looper = looper_newCore();
    loopIo_init(&_uiCmdProxy.io, _uiCmdProxy.looper, _uiCmdProxy.sock,
                _uiCmdProxy_io_func, &_uiCmdProxy);
    loopIo_wantRead(&_uiCmdProxy.io);
    _uiCmdProxy.sync_writer = syncsocket_init(fd);
    if (_uiCmdProxy.sync_writer == NULL) {
        derror("Unable to initialize UICmdProxy writer: %s\n", errno_str);
        uiCmdProxy_destroy();
        return -1;
    }
    {
        
        
        AndroidHwControlFuncs  funcs;
        funcs.light_brightness = _uiCmdProxy_brightness_change_callback;
        android_hw_control_set(&_uiCmdProxy, &funcs);
    }
    return 0;
}

void
uiCmdProxy_destroy()
{
    
    if (_uiCmdProxy.sync_writer != NULL) {
        syncsocket_close(_uiCmdProxy.sync_writer);
        syncsocket_free(_uiCmdProxy.sync_writer);
    }
    if (_uiCmdProxy.looper != NULL) {
        
        loopIo_done(&_uiCmdProxy.io);
        looper_free(_uiCmdProxy.looper);
        _uiCmdProxy.looper = NULL;
    }
    _uiCmdProxy.sock = -1;
}

int
uicmd_set_window_scale(double scale, int is_dpi)
{
    UICmdSetWindowsScale cmd;
    cmd.scale = scale;
    cmd.is_dpi = is_dpi;
    return _uiCmdProxy_send_command(AUICMD_SET_WINDOWS_SCALE, &cmd, sizeof(cmd));
}

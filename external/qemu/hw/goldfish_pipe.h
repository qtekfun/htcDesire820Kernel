/* Copyright (C) 2011 The Android Open Source Project
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
#ifndef _HW_GOLDFISH_PIPE_H
#define _HW_GOLDFISH_PIPE_H

#include <stdint.h>
#include "hw/hw.h"


typedef struct GoldfishPipeBuffer {
    uint8_t*  data;
    size_t    size;
} GoldfishPipeBuffer;

typedef struct {
    void*        (*init)( void* hwpipe, void* pipeOpaque, const char* args );

    void         (*close)( void* pipe );

    int          (*sendBuffers)( void* pipe, const GoldfishPipeBuffer*  buffers, int numBuffers );

    
    int          (*recvBuffers)( void* pipe, GoldfishPipeBuffer* buffers, int numBuffers );

    unsigned     (*poll)( void* pipe );

    void         (*wakeOn)( void* opaque, int flags );

    void         (*save)( void* pipe, QEMUFile* file );

    void*        (*load)( void* hwpipe, void* pipeOpaque, const char* args, QEMUFile* file);
} GoldfishPipeFuncs;

extern void  goldfish_pipe_add_type(const char*               pipeName,
                                     void*                     pipeOpaque,
                                     const GoldfishPipeFuncs*  pipeFuncs );

extern void goldfish_pipe_close( void* hwpipe );

extern void goldfish_pipe_wake( void* hwpipe, unsigned flags );


#define PIPE_REG_COMMAND            0x00  
#define PIPE_REG_STATUS             0x04  
#define PIPE_REG_CHANNEL            0x08  
#define PIPE_REG_SIZE               0x0c  
#define PIPE_REG_ADDRESS            0x10  
#define PIPE_REG_WAKES              0x14  
#define PIPE_REG_PARAMS_ADDR_LOW     0x18
#define PIPE_REG_PARAMS_ADDR_HIGH    0x1c
#define PIPE_REG_ACCESS_PARAMS       0x20

#define PIPE_CMD_OPEN               1  
#define PIPE_CMD_CLOSE              2  
#define PIPE_CMD_POLL               3  

#define PIPE_POLL_IN   (1 << 0)
#define PIPE_POLL_OUT  (1 << 1)
#define PIPE_POLL_HUP  (1 << 2)

#define PIPE_CMD_WRITE_BUFFER       4  
#define PIPE_CMD_WAKE_ON_WRITE      5  

#define PIPE_CMD_READ_BUFFER        6  
#define PIPE_CMD_WAKE_ON_READ       7  

#define PIPE_ERROR_INVAL       -1
#define PIPE_ERROR_AGAIN       -2
#define PIPE_ERROR_NOMEM       -3
#define PIPE_ERROR_IO          -4

#define PIPE_WAKE_CLOSED       (1 << 0)  
#define PIPE_WAKE_READ         (1 << 1)  
#define PIPE_WAKE_WRITE        (1 << 2)  /* pipe can now be written to */

void pipe_dev_init(void);

struct access_params{
    uint32_t channel;
    uint32_t size;
    uint32_t address;
    uint32_t cmd;
    uint32_t result;
    
    uint32_t flags;
};

#endif 

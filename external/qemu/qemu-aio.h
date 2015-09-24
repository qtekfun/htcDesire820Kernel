/*
 * QEMU aio implementation
 *
 * Copyright IBM, Corp. 2008
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 */

#ifndef QEMU_AIO_H
#define QEMU_AIO_H

#include "qemu-common.h"
#include "qemu-char.h"

typedef int (AioFlushHandler)(void *opaque);

typedef int (AioProcessQueue)(void *opaque);

void qemu_aio_flush(void);

void qemu_aio_wait(void);

int qemu_aio_process_queue(void);

int qemu_aio_set_fd_handler(int fd,
                            IOHandler *io_read,
                            IOHandler *io_write,
                            AioFlushHandler *io_flush,
                            AioProcessQueue *io_process_queue,
                            void *opaque);

#endif

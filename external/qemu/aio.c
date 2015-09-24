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

#include "qemu-common.h"
#include "block.h"
#include "qemu-queue.h"
#include "qemu_socket.h"

typedef struct AioHandler AioHandler;

static QLIST_HEAD(, AioHandler) aio_handlers;

static int walking_handlers;

struct AioHandler
{
    int fd;
    IOHandler *io_read;
    IOHandler *io_write;
    AioFlushHandler *io_flush;
    AioProcessQueue *io_process_queue;
    int deleted;
    void *opaque;
    QLIST_ENTRY(AioHandler) node;
};

static AioHandler *find_aio_handler(int fd)
{
    AioHandler *node;

    QLIST_FOREACH(node, &aio_handlers, node) {
        if (node->fd == fd)
            if (!node->deleted)
                return node;
    }

    return NULL;
}

int qemu_aio_set_fd_handler(int fd,
                            IOHandler *io_read,
                            IOHandler *io_write,
                            AioFlushHandler *io_flush,
                            AioProcessQueue *io_process_queue,
                            void *opaque)
{
    AioHandler *node;

    node = find_aio_handler(fd);

    
    if (!io_read && !io_write) {
        if (node) {
            
            if (walking_handlers)
                node->deleted = 1;
            else {
                QLIST_REMOVE(node, node);
                qemu_free(node);
            }
        }
    } else {
        if (node == NULL) {
            
            node = qemu_mallocz(sizeof(AioHandler));
            node->fd = fd;
            QLIST_INSERT_HEAD(&aio_handlers, node, node);
        }
        
        node->io_read = io_read;
        node->io_write = io_write;
        node->io_flush = io_flush;
        node->io_process_queue = io_process_queue;
        node->opaque = opaque;
    }

    qemu_set_fd_handler2(fd, NULL, io_read, io_write, opaque);

    return 0;
}

void qemu_aio_flush(void)
{
    AioHandler *node;
    int ret;

    do {
        ret = 0;

        qemu_aio_wait();

        QLIST_FOREACH(node, &aio_handlers, node) {
            if (node->io_flush) {
                ret |= node->io_flush(node->opaque);
            }
        }
    } while (qemu_bh_poll() || ret > 0);
}

int qemu_aio_process_queue(void)
{
    AioHandler *node;
    int ret = 0;

    walking_handlers = 1;

    QLIST_FOREACH(node, &aio_handlers, node) {
        if (node->io_process_queue) {
            if (node->io_process_queue(node->opaque)) {
                ret = 1;
            }
        }
    }

    walking_handlers = 0;

    return ret;
}

void qemu_aio_wait(void)
{
    int ret;

    if (qemu_bh_poll())
        return;

    if (qemu_aio_process_queue())
        return;

    do {
        AioHandler *node;
        fd_set rdfds, wrfds;
        int max_fd = -1;

        walking_handlers = 1;

        FD_ZERO(&rdfds);
        FD_ZERO(&wrfds);

        
        QLIST_FOREACH(node, &aio_handlers, node) {
            if (node->io_flush && node->io_flush(node->opaque) == 0)
                continue;

            if (!node->deleted && node->io_read) {
                FD_SET(node->fd, &rdfds);
                max_fd = MAX(max_fd, node->fd + 1);
            }
            if (!node->deleted && node->io_write) {
                FD_SET(node->fd, &wrfds);
                max_fd = MAX(max_fd, node->fd + 1);
            }
        }

        walking_handlers = 0;

        
        if (max_fd == -1)
            break;

        
        ret = select(max_fd, &rdfds, &wrfds, NULL, NULL);
        if (ret == -1 && errno == EINTR)
            continue;

        
        if (ret > 0) {
            walking_handlers = 1;

            node = QLIST_FIRST(&aio_handlers);
            while (node) {
                AioHandler *tmp;

                if (!node->deleted &&
                    FD_ISSET(node->fd, &rdfds) &&
                    node->io_read) {
                    node->io_read(node->opaque);
                }
                if (!node->deleted &&
                    FD_ISSET(node->fd, &wrfds) &&
                    node->io_write) {
                    node->io_write(node->opaque);
                }

                tmp = node;
                node = QLIST_NEXT(node, node);

                if (tmp->deleted) {
                    QLIST_REMOVE(tmp, node);
                    qemu_free(tmp);
                }
            }

            walking_handlers = 0;
        }
    } while (ret == 0);
}

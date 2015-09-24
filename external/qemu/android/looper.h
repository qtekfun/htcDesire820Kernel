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
#ifndef ANDROID_LOOPER_H
#define ANDROID_LOOPER_H

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <android/utils/system.h>


typedef int64_t   Duration;

#define  DURATION_INFINITE       ((Duration)INT64_MAX)



typedef struct Looper    Looper;

Looper*  looper_newGeneric(void);

Looper*  looper_newCore(void);


typedef struct LoopTimer LoopTimer;
typedef void (*LoopTimerFunc)(void* opaque);

typedef struct LoopIo    LoopIo;
typedef void (*LoopIoFunc)(void* opaque, int fd, unsigned events);

struct Looper {
    Duration (*now)   (Looper* looper);
    void (*timer_init)(Looper* looper, LoopTimer* timer, LoopTimerFunc callback, void* opaque);
    void (*io_init)   (Looper* looper, LoopIo* io, int fd, LoopIoFunc callback, void* opaque);
    int  (*run)       (Looper* looper, Duration deadline_ms);
    void (*forceQuit) (Looper* looper);
    void (*destroy)   (Looper* looper);
};





typedef struct LoopTimerClass  LoopTimerClass;

struct LoopTimer {
    LoopTimerClass*  clazz;
    void*            impl;
};

struct LoopTimerClass {
    void (*startRelative)(void* impl, Duration timeout_ms);
    void (*startAbsolute)(void* impl, Duration deadline_ms);
    void (*stop)         (void* impl);
    int  (*isActive)     (void* impl);
    void (*done)         (void* impl);
};

AINLINED void
loopTimer_init(LoopTimer*     timer,
               Looper*        looper,
               LoopTimerFunc  callback,
               void*          opaque)
{
    looper->timer_init(looper, timer, callback, opaque);
}

AINLINED void
loopTimer_done(LoopTimer* timer)
{
    timer->clazz->done(timer->impl);
    timer->clazz = NULL;
    timer->impl  = NULL;
}

AINLINED void
loopTimer_startRelative(LoopTimer* timer, Duration timeout_ms)
{
    timer->clazz->startRelative(timer->impl, timeout_ms);
}

AINLINED void
loopTimer_startAbsolute(LoopTimer* timer, Duration deadline_ms)
{
    timer->clazz->startAbsolute(timer->impl, deadline_ms);
}

AINLINED void
loopTimer_stop(LoopTimer* timer)
{
    timer->clazz->stop(timer->impl);
}

AINLINED int
loopTimer_isActive(LoopTimer* timer)
{
    return timer->clazz->isActive(timer->impl);
}


typedef struct LoopIoClass  LoopIoClass;

struct LoopIo {
    LoopIoClass*  clazz;
    void*         impl;
    int           fd;
};

enum {
    LOOP_IO_READ  = (1 << 0),
    LOOP_IO_WRITE = (1 << 1),
};

struct LoopIoClass {
    void (*wantRead)(void* impl);
    void (*wantWrite)(void* impl);
    void (*dontWantRead)(void* impl);
    void (*dontWantWrite)(void* impl);
    unsigned (*poll)(void* impl);
    void (*done)(void* impl);
};

AINLINED void
loopIo_init(LoopIo* io, Looper* looper, int fd, LoopIoFunc callback, void* opaque)
{
    looper->io_init(looper, io, fd, callback, opaque);
    io->fd = fd;
}

AINLINED void
loopIo_done(LoopIo* io)
{
    io->clazz->done(io->impl);
}

/* The following functions are used to indicate whether you want the callback
 * to be fired when there is data to be read or when the file is ready to
 * be written to. */
AINLINED void
loopIo_wantRead(LoopIo* io)
{
    io->clazz->wantRead(io->impl);
}
AINLINED void
loopIo_wantWrite(LoopIo* io)
{
    io->clazz->wantWrite(io->impl);
}
AINLINED void
loopIo_dontWantRead(LoopIo* io)
{
    io->clazz->dontWantRead(io->impl);
}
AINLINED void
loopIo_dontWantWrite(LoopIo* io)
{
    io->clazz->dontWantWrite(io->impl);
}
AINLINED unsigned
loopIo_poll(LoopIo* io)
{
    return io->clazz->poll(io->impl);
}


AINLINED Duration
looper_now(Looper* looper)
{
    return looper->now(looper);
}
AINLINED void
looper_run(Looper* looper)
{
    (void) looper->run(looper, DURATION_INFINITE);
}

AINLINED int
looper_runWithTimeout(Looper* looper, Duration timeout_ms)
{
    if (timeout_ms != DURATION_INFINITE)
        timeout_ms += looper_now(looper);

    return looper->run(looper, timeout_ms);
}

AINLINED int
looper_runWithDeadline(Looper* looper, Duration deadline_ms)
{
    return looper->run(looper, deadline_ms);
}

AINLINED void
looper_forceQuit(Looper* looper)
{
    looper->forceQuit(looper);
}

AINLINED void
looper_free(Looper* looper)
{
    if (looper)
        looper->destroy(looper);
}


#endif 

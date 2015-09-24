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


#include "user-events.h"
#include "android/globals.h"
#include "android/android.h"
#include "android/looper.h"
#include "android/async-utils.h"
#include "android/sync-utils.h"
#include "android/utils/system.h"
#include "android/utils/debug.h"
#include "android/protocol/user-events-protocol.h"
#include "android/protocol/user-events-impl.h"

typedef enum UserEventsImplState {
    
    EXPECTS_HEADER,

    
    EXPECTS_PARAMETERS,
} UserEventsImplState;


typedef struct UserEventsImpl {
    
    AsyncReader         user_events_reader;

    
    LoopIo              io;

    
    Looper*             looper;

    
    int                 sock;

    
    UserEventsImplState state;

    
    UserEventHeader     event_header;

    
    union {
        UserEventGeneric    generic_event;
        UserEventMouse      mouse_event;
        UserEventKeycode    keycode_event;
    };
} UserEventsImpl;

extern void destroy_user_events_client(void);

static UserEventsImpl   _UserEventsImpl;

static void
_userEventsImpl_io_func(void* opaque, int fd, unsigned events)
{
    UserEventsImpl* ueimpl;
    AsyncStatus status;

    if (events & LOOP_IO_WRITE) {
        
        
        derror("Unexpected LOOP_IO_WRITE in _userEventsImpl_io_func\n");
        return;
    }

    ueimpl = (UserEventsImpl*)opaque;
    
    status = asyncReader_read(&ueimpl->user_events_reader);


    switch (status) {
        case ASYNC_COMPLETE:
            switch (ueimpl->state) {
                case EXPECTS_HEADER:
                    
                    ueimpl->state = EXPECTS_PARAMETERS;
                    
                    switch (ueimpl->event_header.event_type) {
                        case AUSER_EVENT_MOUSE:
                            asyncReader_init(&ueimpl->user_events_reader,
                                             &ueimpl->mouse_event,
                                             sizeof(ueimpl->mouse_event),
                                             &ueimpl->io);
                            break;

                        case AUSER_EVENT_KEYCODE:
                            asyncReader_init(&ueimpl->user_events_reader,
                                             &ueimpl->keycode_event,
                                             sizeof(ueimpl->keycode_event),
                                             &ueimpl->io);
                            break;

                        case AUSER_EVENT_GENERIC:
                            asyncReader_init(&ueimpl->user_events_reader,
                                             &ueimpl->generic_event,
                                             sizeof(ueimpl->generic_event),
                                             &ueimpl->io);
                            break;

                        default:
                            derror("Unexpected user event type %d\n",
                                   ueimpl->event_header.event_type);
                            break;
                    }
                    break;

                case EXPECTS_PARAMETERS:
                    
                    switch (ueimpl->event_header.event_type) {
                        case AUSER_EVENT_MOUSE:
                            user_event_mouse(ueimpl->mouse_event.dx,
                                             ueimpl->mouse_event.dy,
                                             ueimpl->mouse_event.dz,
                                             ueimpl->mouse_event.buttons_state);
                            break;

                        case AUSER_EVENT_KEYCODE:
                            user_event_keycode(ueimpl->keycode_event.keycode);
                            break;

                        case AUSER_EVENT_GENERIC:
                            user_event_generic(ueimpl->generic_event.type,
                                               ueimpl->generic_event.code,
                                               ueimpl->generic_event.value);
                            break;

                        default:
                            derror("Unexpected user event type %d\n",
                                   ueimpl->event_header.event_type);
                            break;
                    }
                    
                    ueimpl->event_header.event_type = -1;
                    ueimpl->state = EXPECTS_HEADER;
                    asyncReader_init(&ueimpl->user_events_reader,
                                     &ueimpl->event_header,
                                     sizeof(ueimpl->event_header), &ueimpl->io);
                    break;
            }
            break;
        case ASYNC_ERROR:
            loopIo_dontWantRead(&ueimpl->io);
            if (errno == ECONNRESET) {
                
                destroy_user_events_client();
            } else {
                derror("User event read error %d -> %s\n", errno, errno_str);
            }
            break;

        case ASYNC_NEED_MORE:
            
            return;
    }
}

int
userEventsImpl_create(int fd)
{
    _UserEventsImpl.sock = fd;
    _UserEventsImpl.event_header.event_type = -1;
    _UserEventsImpl.state = EXPECTS_HEADER;
    _UserEventsImpl.looper = looper_newCore();
    loopIo_init(&_UserEventsImpl.io, _UserEventsImpl.looper, _UserEventsImpl.sock,
                _userEventsImpl_io_func, &_UserEventsImpl);
    asyncReader_init(&_UserEventsImpl.user_events_reader,
                     &_UserEventsImpl.event_header,
                     sizeof(_UserEventsImpl.event_header), &_UserEventsImpl.io);
    return 0;
}

void
userEventsImpl_destroy(void)
{
    if (_UserEventsImpl.looper != NULL) {
        
        loopIo_done(&_UserEventsImpl.io);
        looper_free(_UserEventsImpl.looper);
        _UserEventsImpl.looper = NULL;
    }
}

/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include "qemu-common.h"
#include "user-events.h"
#include "android/display-core.h"
#include "android/hw-events.h"
#include "android/charmap.h"
#include "android/globals.h"  
#include "android/utils/misc.h"
#include "android/utils/debug.h"
#include "android/multitouch-screen.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(mtscreen,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(mtscreen)

#define TRACE_ON    1

#if TRACE_ON
#define  T(...)    VERBOSE_PRINT(mtscreen,__VA_ARGS__)
#else
#define  T(...)
#endif


#define MTS_POINTERS_NUM    10
#define MTS_POINTER_UP      -1
#define MTS_POINTER_MOUSE   -2

typedef struct MTSPointerState {
    
    int tracking_id;
    
    int x;
    
    int y;
    
    int pressure;
} MTSPointerState;

typedef struct MTSState {
    
    AndroidMTSPort* mtsp;
    
    DisplayState*   ds;
    
    int             tracked_ptr_num;
    int             current_slot;
    
    int             touch_major;
    
    MTSPointerState tracked_pointers[MTS_POINTERS_NUM];
    
    MTFrameHeader   fb_header;
    int             fb_transfer_in_progress;
    int             ydir;
    
    uint8_t*        current_fb;
} MTSState;

static MTSState _MTSState = { 0 };

static void
_push_event(int type, int code, int value)
{
    user_event_generic(type, code, value);
}

static int
_mtsstate_get_pointer_index(const MTSState* mts_state, int tracking_id)
{
    int index;
    for (index = 0; index < MTS_POINTERS_NUM; index++) {
        if (mts_state->tracked_pointers[index].tracking_id == tracking_id) {
            return index;
        }
    }
    return -1;
}

static int
_mtsstate_get_available_pointer_index(const MTSState* mts_state)
{
    return _mtsstate_get_pointer_index(mts_state, MTS_POINTER_UP);
}

static void
_mts_pointer_down(MTSState* mts_state, int tracking_id, int x, int y, int pressure)
{
    
    const int slot_index = _mtsstate_get_available_pointer_index(mts_state);

    
    if (slot_index >= 0) {
        
        mts_state->tracked_ptr_num++;
        mts_state->tracked_pointers[slot_index].tracking_id = tracking_id;
        mts_state->tracked_pointers[slot_index].x = x;
        mts_state->tracked_pointers[slot_index].y = y;
        mts_state->tracked_pointers[slot_index].pressure = pressure;

        
        
        if (slot_index != mts_state->current_slot) {
            _push_event(EV_ABS, ABS_MT_SLOT, slot_index);
        }
        _push_event(EV_ABS, ABS_MT_TRACKING_ID, slot_index);
        _push_event(EV_ABS, ABS_MT_TOUCH_MAJOR, ++mts_state->touch_major);
        _push_event(EV_ABS, ABS_MT_PRESSURE, pressure);
        _push_event(EV_ABS, ABS_MT_POSITION_X, x);
        _push_event(EV_ABS, ABS_MT_POSITION_Y, y);
        _push_event(EV_SYN, SYN_REPORT, 0);
        mts_state->current_slot = slot_index;
    } else {
        D("MTS pointer count is exceeded.");
        return;
    }
}

static void
_mts_pointer_up(MTSState* mts_state, int slot_index)
{
    
    if (slot_index != mts_state->current_slot) {
        _push_event(EV_ABS, ABS_MT_SLOT, slot_index);
    }

    
    _push_event(EV_ABS, ABS_MT_TRACKING_ID, -1);
    _push_event(EV_SYN, SYN_REPORT, 0);

    
    mts_state->tracked_pointers[slot_index].tracking_id = MTS_POINTER_UP;
    mts_state->tracked_pointers[slot_index].x = 0;
    mts_state->tracked_pointers[slot_index].y = 0;
    mts_state->tracked_pointers[slot_index].pressure = 0;

    mts_state->current_slot = -1;
    mts_state->tracked_ptr_num--;
    assert(mts_state->tracked_ptr_num >= 0);
}

static void
_mts_pointer_move(MTSState* mts_state, int slot_index, int x, int y, int pressure)
{
    MTSPointerState* ptr_state = &mts_state->tracked_pointers[slot_index];

    
    if (ptr_state->x == x && ptr_state->y == y) {
        
        return;
    }

    
    if (slot_index != mts_state->current_slot) {
        _push_event(EV_ABS, ABS_MT_SLOT, slot_index);
        mts_state->current_slot = slot_index;
    }

    
    if (ptr_state->pressure != pressure && pressure != 0) {
        _push_event(EV_ABS, ABS_MT_PRESSURE, pressure);
        ptr_state->pressure = pressure;
    }
    if (ptr_state->x != x) {
        _push_event(EV_ABS, ABS_MT_POSITION_X, x);
        ptr_state->x = x;
    }
    if (ptr_state->y != y) {
        _push_event(EV_ABS, ABS_MT_POSITION_Y, y);
        ptr_state->y = y;
    }
    _push_event(EV_SYN, SYN_REPORT, 0);
}


static int _is_mt_initialized = 0;

static AsyncIOAction
_on_fb_sent(void* opaque, SDKCtlDirectPacket* packet, AsyncIOState status)
{
    MTSState* const mts_state = (MTSState*)opaque;

    if (status == ASIO_STATE_SUCCEEDED) {
        if (mts_state->fb_header.w && mts_state->fb_header.h &&
            !mts_state->fb_transfer_in_progress) {
            mts_state->fb_transfer_in_progress = 1;
            
            if (mts_port_send_frame(mts_state->mtsp, &mts_state->fb_header,
                                    mts_state->current_fb, _on_fb_sent, mts_state,
                                    mts_state->ydir)) {
                mts_state->fb_transfer_in_progress = 0;
            }
        }
    }

    return ASIO_ACTION_DONE;
}

static void
_mt_fb_common_update(MTSState* mts_state, int x, int y, int w, int h)
{
    if (mts_state->fb_header.w == 0 && mts_state->fb_header.h == 0) {
        
        mts_state->fb_header.x = x;
        mts_state->fb_header.y = y;
        mts_state->fb_header.w = w;
        mts_state->fb_header.h = h;
    } else {

        
        int right = mts_state->fb_header.x + mts_state->fb_header.w;
        int bottom = mts_state->fb_header.y + mts_state->fb_header.h;

        
        const int new_right = x + w;
        const int new_bottom = y + h;

        
        if (mts_state->fb_header.x > x) {
            mts_state->fb_header.x = x;
        }
        if (mts_state->fb_header.y > y) {
            mts_state->fb_header.y = y;
        }
        if (right < new_right) {
            right = new_right;
        }
        if (bottom < new_bottom) {
            bottom = new_bottom;
        }
        mts_state->fb_header.w = right - mts_state->fb_header.x;
        mts_state->fb_header.h = bottom - mts_state->fb_header.y;
    }

    if (!mts_state->fb_transfer_in_progress) {
        mts_state->fb_transfer_in_progress = 1;
        if (mts_port_send_frame(mts_state->mtsp, &mts_state->fb_header,
                                mts_state->current_fb, _on_fb_sent, mts_state,
                                mts_state->ydir)) {
            mts_state->fb_transfer_in_progress = 0;
        }
    }
}

static void
_mt_fb_update(void* opaque, int x, int y, int w, int h)
{
    MTSState* const mts_state = (MTSState*)opaque;
    const DisplaySurface* const surface = mts_state->ds->surface;

    T("Multi-touch: Software renderer framebuffer update: %d:%d -> %dx%d",
      x, y, w, h);

    mts_state->fb_header.bpp = surface->pf.bytes_per_pixel;
    mts_state->fb_header.bpl = surface->linesize;
    mts_state->fb_header.disp_width = surface->width;
    mts_state->fb_header.disp_height = surface->height;
    mts_state->current_fb = surface->data;
    mts_state->ydir = 1;

    _mt_fb_common_update(mts_state, x, y, w, h);
}

void
multitouch_opengles_fb_update(void* context,
                              int w, int h, int ydir,
                              int format, int type,
                              unsigned char* pixels)
{
    MTSState* const mts_state = &_MTSState;

    
    if (!_is_mt_initialized) {
        return;
    }

    T("Multi-touch: openGLES framebuffer update: 0:0 -> %dx%d", w, h);

    
    mts_state->fb_header.bpp = 4;
    mts_state->fb_header.bpl = 4 * w;
    mts_state->fb_header.disp_width = w;
    mts_state->fb_header.disp_height = h;
    mts_state->current_fb = pixels;
    mts_state->ydir = ydir;

    
    _mt_fb_common_update(mts_state, 0, 0, w, h);
}

void
multitouch_refresh_screen(void)
{
    MTSState* const mts_state = &_MTSState;

    
    if (!_is_mt_initialized) {
        return;
    }

    
    if (NULL != mts_state->current_fb) {
        _mt_fb_common_update(mts_state, 0, 0, mts_state->fb_header.disp_width,
                             mts_state->fb_header.disp_height);
    }
}

void
multitouch_fb_updated(void)
{
    MTSState* const mts_state = &_MTSState;

    
    mts_state->fb_transfer_in_progress = 0;

    if (mts_state->fb_header.w && mts_state->fb_header.h) {
        mts_state->fb_transfer_in_progress = 1;
        
        if (mts_port_send_frame(mts_state->mtsp, &mts_state->fb_header,
                                mts_state->current_fb, _on_fb_sent, mts_state,
                                mts_state->ydir)) {
            mts_state->fb_transfer_in_progress = 0;
        }
    }
}

void
multitouch_init(AndroidMTSPort* mtsp)
{
    if (!_is_mt_initialized) {
        MTSState* const mts_state = &_MTSState;
        DisplayState* const ds = get_displaystate();
        DisplayUpdateListener* dul;
        int index;


        memset(mts_state, 0, sizeof(MTSState));
        mts_state->tracked_ptr_num = 0;
        mts_state->current_slot = -1;
        for (index = 0; index < MTS_POINTERS_NUM; index++) {
            mts_state->tracked_pointers[index].tracking_id = MTS_POINTER_UP;
        }
        mts_state->mtsp = mtsp;
        mts_state->fb_header.header_size = sizeof(MTFrameHeader);
        mts_state->fb_transfer_in_progress = 0;


        ANEW0(dul);
        dul->opaque = &_MTSState;
        dul->dpy_update = _mt_fb_update;

        
        mts_state->ds = ds;
        mts_state->fb_header.disp_width = ds->surface->width;
        mts_state->fb_header.disp_height = ds->surface->height;
        mts_state->fb_header.x = mts_state->fb_header.y = 0;
        mts_state->fb_header.w = mts_state->fb_header.h = 0;
        mts_state->fb_header.bpp = ds->surface->pf.bytes_per_pixel;
        mts_state->fb_header.bpl = ds->surface->linesize;
        mts_state->fb_transfer_in_progress = 0;

        register_displayupdatelistener(ds, dul);

        _is_mt_initialized = 1;
    }
}

void
multitouch_update_pointer(MTESource source,
                          int tracking_id,
                          int x,
                          int y,
                          int pressure)
{
    MTSState* const mts_state = &_MTSState;

    
    if (source == MTES_MOUSE) {
        tracking_id = MTS_POINTER_MOUSE;
    }

    
    const int slot_index = _mtsstate_get_pointer_index(mts_state, tracking_id);
    if (slot_index < 0) {
        if (pressure == 0) {
            if (tracking_id != MTS_POINTER_MOUSE) {
                D("Unexpected MTS pointer update for tracking id: %d",
                   tracking_id);
            }
            return;
        }

        
        _mts_pointer_down(mts_state, tracking_id, x, y, pressure);
    } else if (pressure == 0) {
        
        _mts_pointer_up(mts_state, slot_index);
    } else {
        
        _mts_pointer_move(mts_state, slot_index, x, y, pressure);
    }
}

int
multitouch_get_max_slot()
{
    return MTS_POINTERS_NUM - 1;
}

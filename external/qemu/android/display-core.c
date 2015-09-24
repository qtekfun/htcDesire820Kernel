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


#include "android/display-core.h"
#include "qemu-common.h"

static void
coredisplay_refresh(DisplayState* ds)
{
    (void)ds;
    vga_hw_update();
}

static void
coredisplay_update(DisplayState* ds, int x, int y, int w, int h)
{
    (void)ds;
    (void)x;
    (void)y;
    (void)w;
    (void)h;
}

static void
coredisplay_resize(DisplayState* ds)
{
    (void)ds;
}

void
coredisplay_init(DisplayState* ds)
{
    static DisplayChangeListener dcl[1];

    dcl->dpy_update  = coredisplay_update;
    dcl->dpy_refresh = coredisplay_refresh;
    dcl->dpy_resize  = coredisplay_resize;
    register_displaychangelistener(ds, dcl);
}

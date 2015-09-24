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

#ifndef ANDROID_MULTITOUCH_SCREEN_H_
#define ANDROID_MULTITOUCH_SCREEN_H_

#include "android/sdk-controller-socket.h"
#include "android/multitouch-port.h"


typedef enum MTESource {
    
    MTES_MOUSE,
    
    MTES_DEVICE,
} MTESource;

extern void multitouch_init(AndroidMTSPort* mtsp);

extern void multitouch_update_pointer(MTESource source,
                                      int tracking_id,
                                      int x,
                                      int y,
                                      int pressure);

extern int multitouch_get_max_slot();

/* A callback set to monitor OpenGLES framebuffer updates.
 * This callback is called by the renderer just before each new frame is
 * displayed, providing a copy of the framebuffer contents.
 * The callback will be called from one of the renderer's threads, so it may
 * require synchronization on any data structures it modifies. The pixels buffer
 * may be overwritten as soon as the callback returns.
 * The pixels buffer is intentionally not const: the callback may modify the data
 * without copying to another buffer if it wants, e.g. in-place RGBA to RGB
 * conversion, or in-place y-inversion.
 * Param:
 *   context        The pointer optionally provided when the callback was
 *                  registered. The client can use this to pass whatever
 *                  information it wants to the callback.
 *   width, height  Dimensions of the image, in pixels. Rows are tightly packed;
 *                  there is no inter-row padding.
 *   ydir           Indicates row order: 1 means top-to-bottom order, -1 means
 *                  bottom-to-top order.
 *   format, type   Format and type GL enums, as used in glTexImage2D() or
 *                  glReadPixels(), describing the pixel format.
 *   pixels         The framebuffer image.
 *
 * In the first implementation, ydir is always -1 (bottom to top), format and
 * type are always GL_RGBA and GL_UNSIGNED_BYTE, and the width and height will
 * always be the same as the ones passed to initOpenGLRenderer().
 */
extern void multitouch_opengles_fb_update(void* context,
                                          int width,
                                          int height,
                                          int ydir,
                                          int format,
                                          int type,
                                          unsigned char* pixels);

extern void multitouch_refresh_screen(void);

extern void multitouch_fb_updated(void);

#endif  

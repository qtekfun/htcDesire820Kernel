// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_PUBLIC_BROWSER_DRAW_GL_H_
#define ANDROID_WEBVIEW_PUBLIC_BROWSER_DRAW_GL_H_

#ifdef __cplusplus
extern "C" {
#endif

struct AwDrawGLInfo {
  
  enum Mode {
    kModeDraw,
    kModeProcess,
  } mode;

  
  
  
  int clip_left;
  int clip_top;
  int clip_right;
  int clip_bottom;

  
  int width;
  int height;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool is_layer;

  
  
  float transform[16];

  
  enum StatusMask {
    kStatusMaskDone = 0x0,
    kStatusMaskDraw = 0x1,
    kStatusMaskInvoke = 0x2,
  };

  
  unsigned int status_mask;

  
  float dirty_left;
  float dirty_top;
  float dirty_right;
  float dirty_bottom;
};

typedef void (AwDrawGLFunction)(int view_context,
                                AwDrawGLInfo* draw_info,
                                void* spare);
enum AwMapMode {
  MAP_READ_ONLY,
  MAP_WRITE_ONLY,
  MAP_READ_WRITE,
};

typedef int AwCreateGraphicBufferFunction(int w, int h);
typedef void AwReleaseGraphicBufferFunction(int buffer_id);
typedef int AwMapFunction(int buffer_id, AwMapMode mode, void** vaddr);
typedef int AwUnmapFunction(int buffer_id);
typedef void* AwGetNativeBufferFunction(int buffer_id);
typedef unsigned int AwGetStrideFunction(int buffer_id);

struct AwDrawGLFunctionTable {
  AwCreateGraphicBufferFunction* create_graphic_buffer;
  AwReleaseGraphicBufferFunction* release_graphic_buffer;
  AwMapFunction* map;
  AwUnmapFunction* unmap;
  AwGetNativeBufferFunction* get_native_buffer;
  AwGetStrideFunction* get_stride;
};

#ifdef __cplusplus
}  
#endif

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_PUBLIC_BROWSER_DRAW_SW_H_
#define ANDROID_WEBVIEW_PUBLIC_BROWSER_DRAW_SW_H_

#include <jni.h>
#include <stddef.h>

#ifndef __cplusplus
#error "Can't mix C and C++ when using jni.h"
#endif

class SkCanvasState;
class SkPicture;

static const int kAwPixelInfoVersion = 3;

struct AwPixelInfo {
  int version;          
  SkCanvasState* state; 
  
};

typedef AwPixelInfo* (AwAccessPixelsFunction)(JNIEnv* env, jobject canvas);

typedef void (AwReleasePixelsFunction)(AwPixelInfo* pixels);

typedef jobject (AwCreatePictureFunction)(JNIEnv* env, SkPicture* picture);

typedef void (SkiaVersionFunction)(int* major, int* minor, int* patch);

typedef bool (AwIsSkiaVersionCompatibleFunction)(SkiaVersionFunction function);

struct AwDrawSWFunctionTable {
  AwAccessPixelsFunction* access_pixels;
  AwReleasePixelsFunction* release_pixels;
  AwCreatePictureFunction* create_picture;
  AwIsSkiaVersionCompatibleFunction* is_skia_version_compatible;
};

#endif  

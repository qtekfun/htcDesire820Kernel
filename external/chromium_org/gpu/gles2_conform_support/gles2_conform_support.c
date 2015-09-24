// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <EGL/egl.h>
#include "gpu/gles2_conform_support/gtf/gtf_stubs.h"

int GTFMain(int argc, char** argv) {
  EGLint major, minor;
  EGLDisplay eglDisplay;
  EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY;

  eglDisplay = eglGetDisplay(nativeDisplay);
  eglInitialize(eglDisplay, &major, &minor);

  return 0;
}



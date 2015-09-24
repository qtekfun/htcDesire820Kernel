// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_BINDINGS_SKIA_IN_PROCESS_H_
#define UI_GL_GL_BINDINGS_SKIA_IN_PROCESS_H_

#include "ui/gl/gl_export.h"

struct GrGLInterface;

namespace gfx {

GL_EXPORT GrGLInterface* CreateInProcessSkiaGLBinding();

}  

#endif  

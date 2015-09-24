// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_CONTEXT_NSVIEW_H_
#define UI_GL_GL_CONTEXT_NSVIEW_H_

#import <AppKit/NSOpenGL.h>

#include "base/compiler_specific.h"
#include "base/mac/scoped_nsobject.h"
#include "ui/gl/gl_context.h"

namespace gfx {

class GLSurface;

class GLContextNSView : public GLContextReal {
 public:
  explicit GLContextNSView(GLShareGroup* group);

  
  virtual bool Initialize(GLSurface* surface,
                          GpuPreference gpu_preference) OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool MakeCurrent(GLSurface* surface) OVERRIDE;
  virtual void ReleaseCurrent(GLSurface* surface) OVERRIDE;
  virtual bool IsCurrent(GLSurface* surface) OVERRIDE;
  virtual void* GetHandle() OVERRIDE;
  virtual void SetSwapInterval(int interval) OVERRIDE;

  
  void FlushBuffer();

 private:
  virtual ~GLContextNSView();

  base::scoped_nsobject<NSOpenGLContext> context_;
  GpuPreference gpu_preference_;

  DISALLOW_COPY_AND_ASSIGN(GLContextNSView);
};

}  

#endif  

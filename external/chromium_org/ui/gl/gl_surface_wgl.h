// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SURFACE_WGL_H_
#define UI_GL_GL_SURFACE_WGL_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/gl/gl_surface.h"

namespace gfx {

class GLSurfaceWGL : public GLSurface {
 public:
  GLSurfaceWGL();
  virtual ~GLSurfaceWGL();

  
  virtual void* GetDisplay();

  static bool InitializeOneOff();
  static HDC GetDisplayDC();

 private:
  DISALLOW_COPY_AND_ASSIGN(GLSurfaceWGL);
};

class NativeViewGLSurfaceWGL : public GLSurfaceWGL {
 public:
  explicit NativeViewGLSurfaceWGL(gfx::AcceleratedWidget window);
  virtual ~NativeViewGLSurfaceWGL();

  
  virtual bool Initialize();
  virtual void Destroy();
  virtual bool IsOffscreen();
  virtual bool SwapBuffers();
  virtual gfx::Size GetSize();
  virtual void* GetHandle();

 private:
  gfx::AcceleratedWidget window_;
  gfx::AcceleratedWidget child_window_;
  HDC device_context_;

  DISALLOW_COPY_AND_ASSIGN(NativeViewGLSurfaceWGL);
};


class PbufferGLSurfaceWGL : public GLSurfaceWGL {
 public:
  explicit PbufferGLSurfaceWGL(const gfx::Size& size);
  virtual ~PbufferGLSurfaceWGL();

  
  virtual bool Initialize();
  virtual void Destroy();
  virtual bool IsOffscreen();
  virtual bool SwapBuffers();
  virtual gfx::Size GetSize();
  virtual void* GetHandle();

 private:
  gfx::Size size_;
  HDC device_context_;
  void* pbuffer_;

  DISALLOW_COPY_AND_ASSIGN(PbufferGLSurfaceWGL);
};

}  

#endif  

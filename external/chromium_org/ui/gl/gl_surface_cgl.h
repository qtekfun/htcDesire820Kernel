// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SURFACE_CGL_H_
#define UI_GL_GL_SURFACE_CGL_H_

#include "ui/gfx/size.h"
#include "ui/gl/gl_surface.h"

namespace gfx {

class GLSurfaceCGL : public GLSurface {
 public:
  GLSurfaceCGL();

  static bool InitializeOneOff();
  static void* GetPixelFormat();

 protected:
  virtual ~GLSurfaceCGL();

 private:
  DISALLOW_COPY_AND_ASSIGN(GLSurfaceCGL);
};

class GL_EXPORT NoOpGLSurfaceCGL : public GLSurfaceCGL {
 public:
  explicit NoOpGLSurfaceCGL(const gfx::Size& size);

  
  virtual bool Initialize() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual void* GetHandle() OVERRIDE;
  virtual void* GetDisplay() OVERRIDE;

 protected:
  virtual ~NoOpGLSurfaceCGL();

 private:
  gfx::Size size_;

  DISALLOW_COPY_AND_ASSIGN(NoOpGLSurfaceCGL);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SURFACE_NSVIEW_H_
#define UI_GL_GL_SURFACE_NSVIEW_H_

#include "base/compiler_specific.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_surface.h"

namespace gfx {

class GLContextNSView;

class GL_EXPORT GLSurfaceNSView : public GLSurface {
 public:
  explicit GLSurfaceNSView(AcceleratedWidget view);

  
  virtual void Destroy() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual void* GetHandle() OVERRIDE;

  
  virtual bool OnMakeCurrent(GLContext* context) OVERRIDE;

 private:
  virtual ~GLSurfaceNSView();

  
  AcceleratedWidget view_;

  
  GLContextNSView* context_;

  DISALLOW_COPY_AND_ASSIGN(GLSurfaceNSView);
};

}  

#endif  

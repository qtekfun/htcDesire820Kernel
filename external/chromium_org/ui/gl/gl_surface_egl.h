// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SURFACE_EGL_H_
#define UI_GL_GL_SURFACE_EGL_H_

#if defined(OS_WIN)
#include <windows.h>
#endif

#include <string>

#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/gfx/vsync_provider.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_surface.h"

namespace gfx {

class GL_EXPORT GLSurfaceEGL : public GLSurface {
 public:
  GLSurfaceEGL();

  
  virtual EGLDisplay GetDisplay() OVERRIDE;

  static bool InitializeOneOff();
  static EGLDisplay GetHardwareDisplay();
  static EGLNativeDisplayType GetNativeDisplay();

  
  
  
  static const char* GetEGLExtensions();
  static bool HasEGLExtension(const char* name);
  static bool IsCreateContextRobustnessSupported();

 protected:
  virtual ~GLSurfaceEGL();

 private:
  DISALLOW_COPY_AND_ASSIGN(GLSurfaceEGL);
};

class GL_EXPORT NativeViewGLSurfaceEGL : public GLSurfaceEGL {
 public:
  explicit NativeViewGLSurfaceEGL(gfx::AcceleratedWidget window);

  
  virtual EGLConfig GetConfig() OVERRIDE;
  virtual bool Initialize() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool Resize(const gfx::Size& size) OVERRIDE;
  virtual bool Recreate() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual EGLSurface GetHandle() OVERRIDE;
  virtual std::string GetExtensions() OVERRIDE;
  virtual bool PostSubBuffer(int x, int y, int width, int height) OVERRIDE;
  virtual VSyncProvider* GetVSyncProvider() OVERRIDE;

  
  
  virtual bool Initialize(VSyncProvider* sync_provider);

 protected:
  virtual ~NativeViewGLSurfaceEGL();
  void SetHandle(EGLSurface surface);

 private:
  gfx::AcceleratedWidget window_;
  EGLSurface surface_;
  bool supports_post_sub_buffer_;
  EGLConfig config_;

  scoped_ptr<VSyncProvider> vsync_provider_;

  DISALLOW_COPY_AND_ASSIGN(NativeViewGLSurfaceEGL);
};

class GL_EXPORT PbufferGLSurfaceEGL : public GLSurfaceEGL {
 public:
  explicit PbufferGLSurfaceEGL(const gfx::Size& size);

  
  virtual EGLConfig GetConfig() OVERRIDE;
  virtual bool Initialize() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual bool Resize(const gfx::Size& size) OVERRIDE;
  virtual EGLSurface GetHandle() OVERRIDE;
  virtual void* GetShareHandle() OVERRIDE;

 protected:
  virtual ~PbufferGLSurfaceEGL();

 private:
  gfx::Size size_;
  EGLSurface surface_;

  DISALLOW_COPY_AND_ASSIGN(PbufferGLSurfaceEGL);
};

class GL_EXPORT SurfacelessEGL : public GLSurfaceEGL {
 public:
  explicit SurfacelessEGL(const gfx::Size& size);

  
  virtual EGLConfig GetConfig() OVERRIDE;
  virtual bool Initialize() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual bool Resize(const gfx::Size& size) OVERRIDE;
  virtual EGLSurface GetHandle() OVERRIDE;
  virtual void* GetShareHandle() OVERRIDE;

 protected:
  virtual ~SurfacelessEGL();

 private:
  gfx::Size size_;
  DISALLOW_COPY_AND_ASSIGN(SurfacelessEGL);
};

}  

#endif  

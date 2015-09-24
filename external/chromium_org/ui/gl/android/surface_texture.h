// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_ANDROID_SURFACE_TEXTURE_H_
#define UI_GL_ANDROID_SURFACE_TEXTURE_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "ui/gl/gl_export.h"

struct ANativeWindow;

namespace gfx {

class GL_EXPORT SurfaceTexture
    : public base::RefCountedThreadSafe<SurfaceTexture>{
 public:
  explicit SurfaceTexture(int texture_id);

  
  
  
  
  
  void SetFrameAvailableCallback(const base::Closure& callback);

  
  void UpdateTexImage();

  
  
  void GetTransformMatrix(float mtx[16]);

  
  void SetDefaultBufferSize(int width, int height);

  
  
  void AttachToGLContext();

  
  
  void DetachFromGLContext();

  
  
  
  ANativeWindow* CreateSurface();

  const base::android::JavaRef<jobject>& j_surface_texture() const {
    return j_surface_texture_;
  }

  static bool RegisterSurfaceTexture(JNIEnv* env);

 private:
  friend class base::RefCountedThreadSafe<SurfaceTexture>;
  ~SurfaceTexture();

  
  base::android::ScopedJavaGlobalRef<jobject> j_surface_texture_;

  DISALLOW_COPY_AND_ASSIGN(SurfaceTexture);
};

}  

#endif  

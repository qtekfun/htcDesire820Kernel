// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_ANDROID_SCOPED_JAVA_SURFACE_H_
#define UI_GL_ANDROID_SCOPED_JAVA_SURFACE_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/move.h"
#include "ui/gl/gl_export.h"

namespace gfx {

class SurfaceTexture;

class GL_EXPORT ScopedJavaSurface {
  MOVE_ONLY_TYPE_FOR_CPP_03(ScopedJavaSurface, RValue);

 public:
  ScopedJavaSurface();

  
  explicit ScopedJavaSurface(const base::android::JavaRef<jobject>& surface);

  
  
  explicit ScopedJavaSurface(const SurfaceTexture* surface_texture);

  
  
  ScopedJavaSurface(RValue rvalue);
  ScopedJavaSurface& operator=(RValue rhs);

  
  
  static ScopedJavaSurface AcquireExternalSurface(jobject surface);

  ~ScopedJavaSurface();

  
  bool IsEmpty() const;

  
  
  bool is_protected() const { return is_protected_; }

  const base::android::JavaRef<jobject>& j_surface() const {
    return j_surface_;
  }

 private:
  
  void MoveFrom(ScopedJavaSurface& other);

  bool auto_release_;
  bool is_protected_;

  base::android::ScopedJavaGlobalRef<jobject> j_surface_;
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_IMAGE_H_
#define UI_GL_GL_IMAGE_H_

#include "base/memory/ref_counted.h"
#include "ui/gfx/gpu_memory_buffer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_export.h"

namespace gfx {

class GLSurface;

class GL_EXPORT GLImage : public base::RefCounted<GLImage> {
 public:
  GLImage();

  
  virtual void Destroy() = 0;

  
  virtual gfx::Size GetSize() = 0;

  
  virtual bool BindTexImage(unsigned target);

  
  virtual void ReleaseTexImage(unsigned target);

  
  virtual void WillUseTexImage();

  
  virtual void DidUseTexImage();

  
  virtual void WillModifyTexImage();

  
  virtual void DidModifyTexImage();

  
  
  virtual void SetReleaseAfterUse();

  
  static scoped_refptr<GLImage> CreateGLImage(gfx::PluginWindowHandle window);

  
  static scoped_refptr<GLImage> CreateGLImageForGpuMemoryBuffer(
      gfx::GpuMemoryBufferHandle buffer,
      gfx::Size size,
      unsigned internalformat);

 protected:
  virtual ~GLImage();

 private:
  friend class base::RefCounted<GLImage>;

  DISALLOW_COPY_AND_ASSIGN(GLImage);
};

}  

#endif  

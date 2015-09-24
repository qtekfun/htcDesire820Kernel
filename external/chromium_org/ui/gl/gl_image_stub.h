// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_IMAGE_STUB_H_
#define UI_GL_GL_IMAGE_STUB_H_

#include "ui/gl/gl_image.h"

namespace gfx {

class GL_EXPORT GLImageStub : public GLImage {
 public:
  GLImageStub();

  
  virtual void Destroy() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual bool BindTexImage(unsigned target) OVERRIDE;
  virtual void ReleaseTexImage(unsigned target) OVERRIDE;
  virtual void WillUseTexImage() OVERRIDE;
  virtual void DidUseTexImage() OVERRIDE;

 protected:
  virtual ~GLImageStub();
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SHARE_GROUP_H_
#define UI_GL_GL_SHARE_GROUP_H_

#include <set>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gl/gl_export.h"

namespace gfx {

class GLContext;

class GL_EXPORT GLShareGroup : public base::RefCounted<GLShareGroup> {
 public:
  GLShareGroup();

  
  
  void AddContext(GLContext* context);
  void RemoveContext(GLContext* context);

  
  
  void* GetHandle();

  
  
  GLContext* GetContext();

  
  
  void SetSharedContext(GLContext* context);
  GLContext* GetSharedContext();

#if defined(OS_MACOSX)
  
  
  void SetRendererID(int renderer_id);
  int GetRendererID();
#endif

 private:
  friend class base::RefCounted<GLShareGroup>;

  ~GLShareGroup();

  
  
  typedef std::set<GLContext*> ContextSet;
  ContextSet contexts_;

  GLContext* shared_context_;

#if defined(OS_MACOSX)
  int renderer_id_;
#endif

  DISALLOW_COPY_AND_ASSIGN(GLShareGroup);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_CLIENT_SHARE_GROUP_H_
#define GPU_COMMAND_BUFFER_CLIENT_SHARE_GROUP_H_

#include <GLES2/gl2.h>
#include "base/memory/scoped_ptr.h"
#include "gles2_impl_export.h"
#include "gpu/command_buffer/client/ref_counted.h"
#include "gpu/command_buffer/common/gles2_cmd_format.h"

namespace gpu {
namespace gles2 {

class GLES2Implementation;
class GLES2ImplementationTest;
class ProgramInfoManager;

typedef void (GLES2Implementation::*DeleteFn)(GLsizei n, const GLuint* ids);

class IdHandlerInterface {
 public:
  IdHandlerInterface() { }
  virtual ~IdHandlerInterface() { }

  
  virtual void MakeIds(
      GLES2Implementation* gl_impl,
      GLuint id_offset, GLsizei n, GLuint* ids) = 0;

  
  virtual bool FreeIds(
      GLES2Implementation* gl_impl, GLsizei n, const GLuint* ids,
      DeleteFn delete_fn) = 0;

  
  virtual bool MarkAsUsedForBind(GLuint id) = 0;
};

class GLES2_IMPL_EXPORT ShareGroup
    : public gpu::RefCountedThreadSafe<ShareGroup> {
 public:
  ShareGroup(bool bind_generates_resource);

  bool bind_generates_resource() const {
    return bind_generates_resource_;
  }

  bool Initialize();

  IdHandlerInterface* GetIdHandler(int namespace_id) const {
    return id_handlers_[namespace_id].get();
  }

  ProgramInfoManager* program_info_manager() {
    return program_info_manager_.get();
  }

 private:
  friend class gpu::RefCountedThreadSafe<ShareGroup>;
  friend class gpu::gles2::GLES2ImplementationTest;
  ~ShareGroup();

  
  void set_program_info_manager(ProgramInfoManager* manager);

  scoped_ptr<IdHandlerInterface> id_handlers_[id_namespaces::kNumIdNamespaces];
  scoped_ptr<ProgramInfoManager> program_info_manager_;

  bool bind_generates_resource_;

  DISALLOW_COPY_AND_ASSIGN(ShareGroup);
};

}  
}  

#endif  

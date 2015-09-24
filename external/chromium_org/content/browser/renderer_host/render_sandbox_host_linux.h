// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_RENDERER_HOST_RENDER_SANDBOX_HOST_LINUX_H_
#define CONTENT_BROWSER_RENDERER_HOST_RENDER_SANDBOX_HOST_LINUX_H_

#include <string>

#include "base/logging.h"
#include "content/common/content_export.h"

template <typename T> struct DefaultSingletonTraits;

namespace content {

class CONTENT_EXPORT RenderSandboxHostLinux {
 public:
  
  static RenderSandboxHostLinux* GetInstance();

  
  
  int GetRendererSocket() const {
    DCHECK(initialized_);
    return renderer_socket_;
  }
  pid_t pid() const {
    DCHECK(initialized_);
    return pid_;
  }
  void Init(const std::string& sandbox_path);

 private:
  friend struct DefaultSingletonTraits<RenderSandboxHostLinux>;
  
  RenderSandboxHostLinux();
  ~RenderSandboxHostLinux();

  
  bool initialized_;

  int renderer_socket_;
  int childs_lifeline_fd_;
  pid_t pid_;

  DISALLOW_COPY_AND_ASSIGN(RenderSandboxHostLinux);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_DEBUG_MARKER_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_DEBUG_MARKER_MANAGER_H_

#include <stack>
#include <string>
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class GPU_EXPORT DebugMarkerManager {
 public:
   DebugMarkerManager();
   ~DebugMarkerManager();

  
  const std::string& GetMarker() const;
  
  void SetMarker(const std::string& marker);
  
  void PushGroup(const std::string& name);
  
  void PopGroup(void);

 private:
  
  class Group {
   public:
    explicit Group(const std::string& name);
    ~Group();

    const std::string& name() const {
      return name_;
    }

    void SetMarker(const std::string& marker);

    const std::string& marker() const {
      return marker_;
    }

   private:
    std::string name_;
    std::string marker_;
  };

  typedef std::stack<Group> GroupStack;

  GroupStack group_stack_;
  std::string empty_;
};

}  
}  

#endif  


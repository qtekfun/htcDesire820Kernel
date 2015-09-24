// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_CONFIG_DX_DIAG_NODE_H_
#define GPU_CONFIG_DX_DIAG_NODE_H_

#include <map>
#include <string>

#include "gpu/gpu_export.h"

namespace gpu {

struct GPU_EXPORT DxDiagNode {
  DxDiagNode();
  ~DxDiagNode();
  std::map<std::string, std::string> values;
  std::map<std::string, DxDiagNode> children;
};

}  

#endif  

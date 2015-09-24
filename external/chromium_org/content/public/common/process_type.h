// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_PROCESS_TYPE_H_
#define CONTENT_PUBLIC_COMMON_PROCESS_TYPE_H_

#include <string>

#include "content/common/content_export.h"

namespace content {

enum ProcessType {
  PROCESS_TYPE_UNKNOWN = 1,
  PROCESS_TYPE_BROWSER,
  PROCESS_TYPE_RENDERER,
  PROCESS_TYPE_PLUGIN,
  PROCESS_TYPE_WORKER,
  PROCESS_TYPE_UTILITY,
  PROCESS_TYPE_ZYGOTE,
  PROCESS_TYPE_SANDBOX_HELPER,
  PROCESS_TYPE_GPU,
  PROCESS_TYPE_PPAPI_PLUGIN,
  PROCESS_TYPE_PPAPI_BROKER,
  
  PROCESS_TYPE_CONTENT_END,
  
  
  
  PROCESS_TYPE_MAX = PROCESS_TYPE_CONTENT_END + 10,
};

CONTENT_EXPORT std::string GetProcessTypeNameInEnglish(int type);

}  

#endif  

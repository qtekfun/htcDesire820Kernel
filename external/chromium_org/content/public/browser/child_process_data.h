// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_CHILD_PROCESS_DATA_H_
#define CONTENT_PUBLIC_BROWSER_CHILD_PROCESS_DATA_H_

#include "base/process/process.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"

namespace content {

struct ChildProcessData {
  
  int process_type;

  
  
  base::string16 name;

  
  
  
  int id;

  
  base::ProcessHandle handle;

  explicit ChildProcessData(int process_type)
      : process_type(process_type), id(0), handle(base::kNullProcessHandle) {
  }
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_COMMON_NACL_PROCESS_TYPE_H_
#define COMPONENTS_NACL_COMMON_NACL_PROCESS_TYPE_H_

#include "content/public/common/process_type.h"

enum NaClProcessType {
  
  
  PROCESS_TYPE_NACL_LOADER = content::PROCESS_TYPE_CONTENT_END + 1,
  PROCESS_TYPE_NACL_BROKER,
};

#endif  

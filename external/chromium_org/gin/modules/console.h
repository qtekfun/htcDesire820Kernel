// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GIN_MODULES_CONSOLE_H_
#define GIN_MODULES_CONSOLE_H_

#include "gin/gin_export.h"
#include "v8/include/v8.h"

namespace gin {

class GIN_EXPORT Console {
 public:
  static const char kModuleName[];
  static v8::Local<v8::ObjectTemplate> GetTemplate(v8::Isolate* isolate);
};

}  

#endif  

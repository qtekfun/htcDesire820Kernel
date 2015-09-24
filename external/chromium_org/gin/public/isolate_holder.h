// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GIN_PUBLIC_ISOLATE_HOLDER_H_
#define GIN_PUBLIC_ISOLATE_HOLDER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "gin/gin_export.h"

namespace v8 {
class Isolate;
}

namespace gin {

class PerIsolateData;

class GIN_EXPORT IsolateHolder {
 public:
  IsolateHolder();
  explicit IsolateHolder(v8::Isolate* isolate);

  ~IsolateHolder();

  v8::Isolate* isolate() { return isolate_; }

 private:
  void Init();

  bool isolate_owner_;
  v8::Isolate* isolate_;
  scoped_ptr<PerIsolateData> isolate_data_;

  DISALLOW_COPY_AND_ASSIGN(IsolateHolder);
};

}  

#endif  

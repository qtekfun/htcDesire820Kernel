// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GIN_PUBLIC_WRAPPER_INFO_H_
#define GIN_PUBLIC_WRAPPER_INFO_H_

#include "gin/gin_export.h"
#include "gin/public/gin_embedders.h"
#include "v8/include/v8.h"

namespace gin {


enum InternalFields {
  kWrapperInfoIndex,
  kEncodedValueIndex,
  kNumberOfInternalFields,
};

struct GIN_EXPORT WrapperInfo {
  static WrapperInfo* From(v8::Handle<v8::Object> object);
  const GinEmbedder embedder;
};

}  

#endif  

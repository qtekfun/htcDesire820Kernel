// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_ID_ASSIGNMENT_H_
#define PPAPI_SHARED_IMPL_ID_ASSIGNMENT_H_

#include <limits>

#include "base/basictypes.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

enum PPIdType {
  PP_ID_TYPE_MODULE,
  PP_ID_TYPE_INSTANCE,
  PP_ID_TYPE_RESOURCE,
  PP_ID_TYPE_VAR,

  
  PP_ID_TYPE_COUNT
};

PPAPI_SHARED_EXPORT extern const unsigned int kPPIdTypeBits;

extern const int32 kMaxPPId;

template <typename T> inline T MakeTypedId(T value, PPIdType type) {
  return (value << kPPIdTypeBits) | static_cast<T>(type);
}

template <typename T> inline bool CheckIdType(T id, PPIdType type) {
  
  
  
  if (type == PP_ID_TYPE_RESOURCE && !id)
    return true;
  const T mask = (static_cast<T>(1) << kPPIdTypeBits) - 1;
  return (id & mask) == type;
}

}  

#endif  

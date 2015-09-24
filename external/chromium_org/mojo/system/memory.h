// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_MEMORY_H_
#define MOJO_SYSTEM_MEMORY_H_

#include <stddef.h>

#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

template <size_t size>
bool MOJO_SYSTEM_IMPL_EXPORT VerifyUserPointerForSize(const void* pointer,
                                                      size_t count);

bool MOJO_SYSTEM_IMPL_EXPORT VerifyUserPointerForSize(const void* pointer,
                                                      size_t size,
                                                      size_t count);

template <typename T>
bool VerifyUserPointer(const T* pointer, size_t count) {
  return VerifyUserPointerForSize<sizeof(T)>(pointer, count);
}

template <>
inline bool VerifyUserPointer<void>(const void* pointer, size_t count) {
  return VerifyUserPointerForSize<1>(pointer, count);
}

}  
}  

#endif  

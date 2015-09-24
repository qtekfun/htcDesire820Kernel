// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_SCOPED_NATIVE_LIBRARY_H_
#define BASE_MEMORY_SCOPED_NATIVE_LIBRARY_H_
#pragma once

#include "base/base_api.h"
#include "base/native_library.h"

class FilePath;

namespace base {

class BASE_API ScopedNativeLibrary {
 public:
  
  ScopedNativeLibrary();

  
  explicit ScopedNativeLibrary(NativeLibrary library);

  
  explicit ScopedNativeLibrary(const FilePath& library_path);

  ~ScopedNativeLibrary();

  
  bool is_valid() const { return !!library_; }

  void* GetFunctionPointer(const char* function_name) const;

  
  
  void Reset(NativeLibrary library);

  
  
  NativeLibrary Release();

 private:
  NativeLibrary library_;

  DISALLOW_COPY_AND_ASSIGN(ScopedNativeLibrary);
};

}  

#endif  

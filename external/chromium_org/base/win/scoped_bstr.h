// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_SCOPED_BSTR_H_
#define BASE_WIN_SCOPED_BSTR_H_

#include <windows.h>
#include <oleauto.h>

#include "base/base_export.h"
#include "base/logging.h"
#include "base/strings/string16.h"

namespace base {
namespace win {

class BASE_EXPORT ScopedBstr {
 public:
  ScopedBstr() : bstr_(NULL) {
  }

  
  
  
  
  explicit ScopedBstr(const char16* non_bstr);
  ~ScopedBstr();

  
  
  void Reset(BSTR bstr = NULL);

  
  BSTR Release();

  
  
  
  
  
  
  BSTR Allocate(const char16* str);

  
  
  BSTR AllocateBytes(size_t bytes);

  
  
  
  
  
  
  
  
  
  
  
  void SetByteLen(size_t bytes);

  
  void Swap(ScopedBstr& bstr2);

  
  
  
  
  BSTR* Receive();

  
  size_t Length() const;

  
  size_t ByteLength() const;

  operator BSTR() const {
    return bstr_;
  }

 protected:
  BSTR bstr_;

 private:
  
  
  bool operator==(const ScopedBstr& bstr2) const;
  bool operator!=(const ScopedBstr& bstr2) const;
  DISALLOW_COPY_AND_ASSIGN(ScopedBstr);
};

}  
}  

#endif  

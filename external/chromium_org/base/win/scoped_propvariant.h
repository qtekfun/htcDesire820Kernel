// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_SCOPED_PROPVARIANT_H_
#define BASE_WIN_SCOPED_PROPVARIANT_H_

#include <propidl.h>

#include "base/basictypes.h"
#include "base/logging.h"

namespace base {
namespace win {

class ScopedPropVariant {
 public:
  ScopedPropVariant() {
    PropVariantInit(&pv_);
  }

  ~ScopedPropVariant() {
    Reset();
  }

  
  
  PROPVARIANT* Receive() {
    DCHECK_EQ(pv_.vt, VT_EMPTY);
    return &pv_;
  }

  
  void Reset() {
    if (pv_.vt != VT_EMPTY) {
      HRESULT result = PropVariantClear(&pv_);
      DCHECK_EQ(result, S_OK);
    }
  }

  const PROPVARIANT& get() const { return pv_; }

  const PROPVARIANT* operator&() const { return &pv_; }

 private:
  PROPVARIANT pv_;

  
  bool operator==(const ScopedPropVariant&) const;
  bool operator!=(const ScopedPropVariant&) const;
  DISALLOW_COPY_AND_ASSIGN(ScopedPropVariant);
};

}  
}  

#endif  

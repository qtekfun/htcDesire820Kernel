// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_ENUM_VARIANT_H_
#define BASE_WIN_ENUM_VARIANT_H_

#include <unknwn.h>

#include "base/memory/scoped_ptr.h"
#include "base/win/iunknown_impl.h"

namespace base {
namespace win {

class BASE_EXPORT EnumVariant
  : public IEnumVARIANT,
    public IUnknownImpl {
 public:
  
  
  explicit EnumVariant(unsigned long count);

  
  VARIANT* ItemAt(unsigned long index);

  
  ULONG STDMETHODCALLTYPE AddRef() OVERRIDE;
  ULONG STDMETHODCALLTYPE Release() OVERRIDE;
  STDMETHODIMP QueryInterface(REFIID riid, void** ppv) OVERRIDE;

  
  STDMETHODIMP Next(ULONG requested_count,
                    VARIANT* out_elements,
                    ULONG* out_elements_received);
  STDMETHODIMP Skip(ULONG skip_count);
  STDMETHODIMP Reset();
  STDMETHODIMP Clone(IEnumVARIANT** out_cloned_object);

 private:
  ~EnumVariant();

  scoped_ptr<VARIANT[]> items_;
  unsigned long count_;
  unsigned long current_index_;
};

}  
}  

#endif  

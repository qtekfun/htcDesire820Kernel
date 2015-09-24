// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_SCOPED_VARIANT_H_
#define BASE_WIN_SCOPED_VARIANT_H_

#include <windows.h>
#include <oleauto.h>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace win {

class BASE_EXPORT ScopedVariant {
 public:
  
  static const VARIANT kEmptyVariant;

  
  ScopedVariant() {
    
    var_.vt = VT_EMPTY;
  }

  
  
  
  explicit ScopedVariant(const wchar_t* str);

  
  ScopedVariant(const wchar_t* str, UINT length);

  
  
  explicit ScopedVariant(int value, VARTYPE vt = VT_I4);

  
  
  explicit ScopedVariant(double value, VARTYPE vt = VT_R8);

  
  explicit ScopedVariant(IDispatch* dispatch);

  
  explicit ScopedVariant(IUnknown* unknown);

  
  explicit ScopedVariant(SAFEARRAY* safearray);

  
  explicit ScopedVariant(const VARIANT& var);

  ~ScopedVariant();

  inline VARTYPE type() const {
    return var_.vt;
  }

  
  void Reset(const VARIANT& var = kEmptyVariant);

  
  VARIANT Release();

  
  void Swap(ScopedVariant& var);

  
  VARIANT Copy() const;

  
  
  int Compare(const VARIANT& var, bool ignore_case = false) const;

  
  
  
  
  VARIANT* Receive();

  void Set(const wchar_t* str);

  
  void Set(int8 i8);
  void Set(uint8 ui8);
  void Set(int16 i16);
  void Set(uint16 ui16);
  void Set(int32 i32);
  void Set(uint32 ui32);
  void Set(int64 i64);
  void Set(uint64 ui64);
  void Set(float r32);
  void Set(double r64);
  void Set(bool b);

  
  
  
  void Set(const VARIANT& var);

  
  void Set(IDispatch* disp);
  void Set(IUnknown* unk);

  
  void Set(SAFEARRAY* array);

  
  
  void SetDate(DATE date);

  
  
  
  
  const VARIANT* operator&() const {
    return &var_;
  }

  
  
  ScopedVariant& operator=(const VARIANT& var);

  
  
  
  
  VARIANT* AsInput() const {
    
    
    return const_cast<VARIANT*>(&var_);
  }

  
  
  operator const VARIANT&() const {
    return var_;
  }

  
  static bool IsLeakableVarType(VARTYPE vt);

 protected:
  VARIANT var_;

 private:
  
  
  bool operator==(const ScopedVariant& var) const;
  bool operator!=(const ScopedVariant& var) const;
  DISALLOW_COPY_AND_ASSIGN(ScopedVariant);
};

}  
}  

#endif  


// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_IDISPATCH_DRIVER_WIN_H_
#define REMOTING_BASE_IDISPATCH_DRIVER_WIN_H_

#include <oaidl.h>

#include "base/basictypes.h"
#include "base/template_util.h"
#include "base/win/scoped_variant.h"

namespace remoting {

namespace dispatch {

namespace internal {

class ScopedVariantArg : public VARIANTARG {
 public:
  ScopedVariantArg() {
    vt = VT_EMPTY;
  }

  ~ScopedVariantArg() {
    VariantClear(this);
  }

  
  

  HRESULT Wrap(const VARIANT& param) {
    DCHECK(vt == VT_EMPTY);
    return VariantCopy(this, &param);
  }

  HRESULT Wrap(VARIANT* const & param) {
    DCHECK(vt == VT_EMPTY);

    
    
    
    
    
    
    Swap(param);
    return S_OK;
  }

  
  

  void Unwrap(const VARIANT& param_out) {
    
  }

  void Unwrap(VARIANT* const & param_out) {
    
    Swap(param_out);
  }

 private:
  
  
  void Swap(VARIANT* other) {
    VARIANT temp = *other;
    *other = *this;
    *static_cast<VARIANTARG*>(this) = temp;
  }

  DISALLOW_COPY_AND_ASSIGN(ScopedVariantArg);
};

COMPILE_ASSERT(sizeof(ScopedVariantArg) == sizeof(VARIANTARG),
               scoped_variant_arg_should_not_add_data_members);

}  

//         is overwritten when the function returns. The pointed-to value must

HRESULT Invoke(IDispatch* object,
               LPOLESTR name,
               WORD flags,
               VARIANT* const & result_out) {
  
  DISPID disp_id;
  HRESULT hr = object->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT,
                                     &disp_id);
  if (FAILED(hr))
    return hr;

  
  internal::ScopedVariantArg result;
  VARIANT* disp_result = NULL;
  if (result_out != NULL)
    disp_result = &result;


  
  
  
  
  DISPPARAMS disp_params = { NULL, NULL, 0, 0 };
  DISPID dispid_named = DISPID_PROPERTYPUT;
  if (flags == DISPATCH_PROPERTYPUT || flags == DISPATCH_PROPERTYPUTREF) {
    disp_params.cNamedArgs = 1;
    disp_params.rgdispidNamedArgs = &dispid_named;
  }

  hr = object->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, flags,
                      &disp_params, disp_result, NULL, NULL);
  if (FAILED(hr))
    return hr;


  
  if (result_out != NULL) {
    result.Unwrap(result_out);
  }

  return S_OK;
}

template <typename P1>
HRESULT Invoke(IDispatch* object,
               LPOLESTR name,
               WORD flags,
               const P1& p1,
               VARIANT* const & result_out) {
  
  DISPID disp_id;
  HRESULT hr = object->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT,
                                     &disp_id);
  if (FAILED(hr))
    return hr;

  
  internal::ScopedVariantArg result;
  VARIANT* disp_result = NULL;
  if (result_out != NULL)
    disp_result = &result;

  
  internal::ScopedVariantArg disp_args[1];
  hr = disp_args[1 - 1].Wrap(p1);
  if (FAILED(hr))
    return hr;

  
  
  
  
  DISPPARAMS disp_params = { disp_args, NULL, 1, 0 };
  DISPID dispid_named = DISPID_PROPERTYPUT;
  if (flags == DISPATCH_PROPERTYPUT || flags == DISPATCH_PROPERTYPUTREF) {
    disp_params.cNamedArgs = 1;
    disp_params.rgdispidNamedArgs = &dispid_named;
  }

  hr = object->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, flags,
                      &disp_params, disp_result, NULL, NULL);
  if (FAILED(hr))
    return hr;

  
  disp_args[1 - 1].Unwrap(p1);

  
  if (result_out != NULL) {
    result.Unwrap(result_out);
  }

  return S_OK;
}

template <typename P1, typename P2>
HRESULT Invoke(IDispatch* object,
               LPOLESTR name,
               WORD flags,
               const P1& p1,
               const P2& p2,
               VARIANT* const & result_out) {
  
  DISPID disp_id;
  HRESULT hr = object->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT,
                                     &disp_id);
  if (FAILED(hr))
    return hr;

  
  internal::ScopedVariantArg result;
  VARIANT* disp_result = NULL;
  if (result_out != NULL)
    disp_result = &result;

  
  internal::ScopedVariantArg disp_args[2];
  hr = disp_args[2 - 1].Wrap(p1);
  if (FAILED(hr))
    return hr;
  hr = disp_args[2 - 2].Wrap(p2);
  if (FAILED(hr))
    return hr;

  
  
  
  
  DISPPARAMS disp_params = { disp_args, NULL, 2, 0 };
  DISPID dispid_named = DISPID_PROPERTYPUT;
  if (flags == DISPATCH_PROPERTYPUT || flags == DISPATCH_PROPERTYPUTREF) {
    disp_params.cNamedArgs = 1;
    disp_params.rgdispidNamedArgs = &dispid_named;
  }

  hr = object->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, flags,
                      &disp_params, disp_result, NULL, NULL);
  if (FAILED(hr))
    return hr;

  
  disp_args[2 - 1].Unwrap(p1);
  disp_args[2 - 2].Unwrap(p2);

  
  if (result_out != NULL) {
    result.Unwrap(result_out);
  }

  return S_OK;
}

template <typename P1, typename P2, typename P3>
HRESULT Invoke(IDispatch* object,
               LPOLESTR name,
               WORD flags,
               const P1& p1,
               const P2& p2,
               const P3& p3,
               VARIANT* const & result_out) {
  
  DISPID disp_id;
  HRESULT hr = object->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT,
                                     &disp_id);
  if (FAILED(hr))
    return hr;

  
  internal::ScopedVariantArg result;
  VARIANT* disp_result = NULL;
  if (result_out != NULL)
    disp_result = &result;

  
  internal::ScopedVariantArg disp_args[3];
  hr = disp_args[3 - 1].Wrap(p1);
  if (FAILED(hr))
    return hr;
  hr = disp_args[3 - 2].Wrap(p2);
  if (FAILED(hr))
    return hr;
  hr = disp_args[3 - 3].Wrap(p3);
  if (FAILED(hr))
    return hr;

  
  
  
  
  DISPPARAMS disp_params = { disp_args, NULL, 3, 0 };
  DISPID dispid_named = DISPID_PROPERTYPUT;
  if (flags == DISPATCH_PROPERTYPUT || flags == DISPATCH_PROPERTYPUTREF) {
    disp_params.cNamedArgs = 1;
    disp_params.rgdispidNamedArgs = &dispid_named;
  }

  hr = object->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, flags,
                      &disp_params, disp_result, NULL, NULL);
  if (FAILED(hr))
    return hr;

  
  disp_args[3 - 1].Unwrap(p1);
  disp_args[3 - 2].Unwrap(p2);
  disp_args[3 - 3].Unwrap(p3);

  
  if (result_out != NULL) {
    result.Unwrap(result_out);
  }

  return S_OK;
}

template <typename P1, typename P2, typename P3, typename P4>
HRESULT Invoke(IDispatch* object,
               LPOLESTR name,
               WORD flags,
               const P1& p1,
               const P2& p2,
               const P3& p3,
               const P4& p4,
               VARIANT* const & result_out) {
  
  DISPID disp_id;
  HRESULT hr = object->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT,
                                     &disp_id);
  if (FAILED(hr))
    return hr;

  
  internal::ScopedVariantArg result;
  VARIANT* disp_result = NULL;
  if (result_out != NULL)
    disp_result = &result;

  
  internal::ScopedVariantArg disp_args[4];
  hr = disp_args[4 - 1].Wrap(p1);
  if (FAILED(hr))
    return hr;
  hr = disp_args[4 - 2].Wrap(p2);
  if (FAILED(hr))
    return hr;
  hr = disp_args[4 - 3].Wrap(p3);
  if (FAILED(hr))
    return hr;
  hr = disp_args[4 - 4].Wrap(p4);
  if (FAILED(hr))
    return hr;

  
  
  
  
  DISPPARAMS disp_params = { disp_args, NULL, 4, 0 };
  DISPID dispid_named = DISPID_PROPERTYPUT;
  if (flags == DISPATCH_PROPERTYPUT || flags == DISPATCH_PROPERTYPUTREF) {
    disp_params.cNamedArgs = 1;
    disp_params.rgdispidNamedArgs = &dispid_named;
  }

  hr = object->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, flags,
                      &disp_params, disp_result, NULL, NULL);
  if (FAILED(hr))
    return hr;

  
  disp_args[4 - 1].Unwrap(p1);
  disp_args[4 - 2].Unwrap(p2);
  disp_args[4 - 3].Unwrap(p3);
  disp_args[4 - 4].Unwrap(p4);

  
  if (result_out != NULL) {
    result.Unwrap(result_out);
  }

  return S_OK;
}

template <typename P1, typename P2, typename P3, typename P4, typename P5>
HRESULT Invoke(IDispatch* object,
               LPOLESTR name,
               WORD flags,
               const P1& p1,
               const P2& p2,
               const P3& p3,
               const P4& p4,
               const P5& p5,
               VARIANT* const & result_out) {
  
  DISPID disp_id;
  HRESULT hr = object->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT,
                                     &disp_id);
  if (FAILED(hr))
    return hr;

  
  internal::ScopedVariantArg result;
  VARIANT* disp_result = NULL;
  if (result_out != NULL)
    disp_result = &result;

  
  internal::ScopedVariantArg disp_args[5];
  hr = disp_args[5 - 1].Wrap(p1);
  if (FAILED(hr))
    return hr;
  hr = disp_args[5 - 2].Wrap(p2);
  if (FAILED(hr))
    return hr;
  hr = disp_args[5 - 3].Wrap(p3);
  if (FAILED(hr))
    return hr;
  hr = disp_args[5 - 4].Wrap(p4);
  if (FAILED(hr))
    return hr;
  hr = disp_args[5 - 5].Wrap(p5);
  if (FAILED(hr))
    return hr;

  
  
  
  
  DISPPARAMS disp_params = { disp_args, NULL, 5, 0 };
  DISPID dispid_named = DISPID_PROPERTYPUT;
  if (flags == DISPATCH_PROPERTYPUT || flags == DISPATCH_PROPERTYPUTREF) {
    disp_params.cNamedArgs = 1;
    disp_params.rgdispidNamedArgs = &dispid_named;
  }

  hr = object->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, flags,
                      &disp_params, disp_result, NULL, NULL);
  if (FAILED(hr))
    return hr;

  
  disp_args[5 - 1].Unwrap(p1);
  disp_args[5 - 2].Unwrap(p2);
  disp_args[5 - 3].Unwrap(p3);
  disp_args[5 - 4].Unwrap(p4);
  disp_args[5 - 5].Unwrap(p5);

  
  if (result_out != NULL) {
    result.Unwrap(result_out);
  }

  return S_OK;
}

template <typename P1, typename P2, typename P3, typename P4, typename P5,
    typename P6>
HRESULT Invoke(IDispatch* object,
               LPOLESTR name,
               WORD flags,
               const P1& p1,
               const P2& p2,
               const P3& p3,
               const P4& p4,
               const P5& p5,
               const P6& p6,
               VARIANT* const & result_out) {
  
  DISPID disp_id;
  HRESULT hr = object->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT,
                                     &disp_id);
  if (FAILED(hr))
    return hr;

  
  internal::ScopedVariantArg result;
  VARIANT* disp_result = NULL;
  if (result_out != NULL)
    disp_result = &result;

  
  internal::ScopedVariantArg disp_args[6];
  hr = disp_args[6 - 1].Wrap(p1);
  if (FAILED(hr))
    return hr;
  hr = disp_args[6 - 2].Wrap(p2);
  if (FAILED(hr))
    return hr;
  hr = disp_args[6 - 3].Wrap(p3);
  if (FAILED(hr))
    return hr;
  hr = disp_args[6 - 4].Wrap(p4);
  if (FAILED(hr))
    return hr;
  hr = disp_args[6 - 5].Wrap(p5);
  if (FAILED(hr))
    return hr;
  hr = disp_args[6 - 6].Wrap(p6);
  if (FAILED(hr))
    return hr;

  
  
  
  
  DISPPARAMS disp_params = { disp_args, NULL, 6, 0 };
  DISPID dispid_named = DISPID_PROPERTYPUT;
  if (flags == DISPATCH_PROPERTYPUT || flags == DISPATCH_PROPERTYPUTREF) {
    disp_params.cNamedArgs = 1;
    disp_params.rgdispidNamedArgs = &dispid_named;
  }

  hr = object->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, flags,
                      &disp_params, disp_result, NULL, NULL);
  if (FAILED(hr))
    return hr;

  
  disp_args[6 - 1].Unwrap(p1);
  disp_args[6 - 2].Unwrap(p2);
  disp_args[6 - 3].Unwrap(p3);
  disp_args[6 - 4].Unwrap(p4);
  disp_args[6 - 5].Unwrap(p5);
  disp_args[6 - 6].Unwrap(p6);

  
  if (result_out != NULL) {
    result.Unwrap(result_out);
  }

  return S_OK;
}

template <typename P1, typename P2, typename P3, typename P4, typename P5,
    typename P6, typename P7>
HRESULT Invoke(IDispatch* object,
               LPOLESTR name,
               WORD flags,
               const P1& p1,
               const P2& p2,
               const P3& p3,
               const P4& p4,
               const P5& p5,
               const P6& p6,
               const P7& p7,
               VARIANT* const & result_out) {
  
  DISPID disp_id;
  HRESULT hr = object->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT,
                                     &disp_id);
  if (FAILED(hr))
    return hr;

  
  internal::ScopedVariantArg result;
  VARIANT* disp_result = NULL;
  if (result_out != NULL)
    disp_result = &result;

  
  internal::ScopedVariantArg disp_args[7];
  hr = disp_args[7 - 1].Wrap(p1);
  if (FAILED(hr))
    return hr;
  hr = disp_args[7 - 2].Wrap(p2);
  if (FAILED(hr))
    return hr;
  hr = disp_args[7 - 3].Wrap(p3);
  if (FAILED(hr))
    return hr;
  hr = disp_args[7 - 4].Wrap(p4);
  if (FAILED(hr))
    return hr;
  hr = disp_args[7 - 5].Wrap(p5);
  if (FAILED(hr))
    return hr;
  hr = disp_args[7 - 6].Wrap(p6);
  if (FAILED(hr))
    return hr;
  hr = disp_args[7 - 7].Wrap(p7);
  if (FAILED(hr))
    return hr;

  
  
  
  
  DISPPARAMS disp_params = { disp_args, NULL, 7, 0 };
  DISPID dispid_named = DISPID_PROPERTYPUT;
  if (flags == DISPATCH_PROPERTYPUT || flags == DISPATCH_PROPERTYPUTREF) {
    disp_params.cNamedArgs = 1;
    disp_params.rgdispidNamedArgs = &dispid_named;
  }

  hr = object->Invoke(disp_id, IID_NULL, LOCALE_USER_DEFAULT, flags,
                      &disp_params, disp_result, NULL, NULL);
  if (FAILED(hr))
    return hr;

  
  disp_args[7 - 1].Unwrap(p1);
  disp_args[7 - 2].Unwrap(p2);
  disp_args[7 - 3].Unwrap(p3);
  disp_args[7 - 4].Unwrap(p4);
  disp_args[7 - 5].Unwrap(p5);
  disp_args[7 - 6].Unwrap(p6);
  disp_args[7 - 7].Unwrap(p7);

  
  if (result_out != NULL) {
    result.Unwrap(result_out);
  }

  return S_OK;
}

} 

} 

#endif 

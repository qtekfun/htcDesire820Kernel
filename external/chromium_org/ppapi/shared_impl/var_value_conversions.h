// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_VAR_VALUE_CONVERSIONS_H_
#define PPAPI_SHARED_IMPL_VAR_VALUE_CONVERSIONS_H_

#include <vector>

#include "ppapi/c/pp_var.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace base {
class ListValue;
class Value;
}

namespace ppapi {

PPAPI_SHARED_EXPORT base::Value* CreateValueFromVar(const PP_Var& var);

PPAPI_SHARED_EXPORT PP_Var CreateVarFromValue(const base::Value& value);

PPAPI_SHARED_EXPORT base::ListValue* CreateListValueFromVarVector(
    const std::vector<PP_Var>& vars);

PPAPI_SHARED_EXPORT bool CreateVarVectorFromListValue(
    const base::ListValue& list_value, std::vector<PP_Var>* vars);

}  

#endif  

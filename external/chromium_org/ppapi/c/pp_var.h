/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PP_VAR_H_
#define PPAPI_C_PP_VAR_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"



typedef enum {
  PP_VARTYPE_UNDEFINED = 0,
  PP_VARTYPE_NULL = 1,
  PP_VARTYPE_BOOL = 2,
  PP_VARTYPE_INT32 = 3,
  PP_VARTYPE_DOUBLE = 4,
  PP_VARTYPE_STRING = 5,
  PP_VARTYPE_OBJECT = 6,
  PP_VARTYPE_ARRAY = 7,
  PP_VARTYPE_DICTIONARY = 8,
  PP_VARTYPE_ARRAY_BUFFER = 9,
  PP_VARTYPE_RESOURCE = 10
} PP_VarType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VarType, 4);

union PP_VarValue {
  PP_Bool as_bool;
  int32_t as_int;
  double as_double;
  int64_t as_id;
};

struct PP_Var {
  PP_VarType type;
  int32_t padding;
  union PP_VarValue value;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_Var, 16);


PP_INLINE struct PP_Var PP_MakeUndefined(void) {
  struct PP_Var result = { PP_VARTYPE_UNDEFINED, 0, {PP_FALSE} };
  return result;
}

PP_INLINE struct PP_Var PP_MakeNull(void) {
  struct PP_Var result = { PP_VARTYPE_NULL, 0, {PP_FALSE} };
  return result;
}

PP_INLINE struct PP_Var PP_MakeBool(PP_Bool value) {
  struct PP_Var result = { PP_VARTYPE_BOOL, 0, {PP_FALSE} };
  result.value.as_bool = value;
  return result;
}

PP_INLINE struct PP_Var PP_MakeInt32(int32_t value) {
  struct PP_Var result = { PP_VARTYPE_INT32, 0, {PP_FALSE} };
  result.value.as_int = value;
  return result;
}

PP_INLINE struct PP_Var PP_MakeDouble(double value) {
  struct PP_Var result = { PP_VARTYPE_DOUBLE, 0, {PP_FALSE} };
  result.value.as_double = value;
  return result;
}

#endif  


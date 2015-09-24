// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef WEBKIT_RENDERER_CPP_VARIANT_H__
#define WEBKIT_RENDERER_CPP_VARIANT_H__

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "third_party/npapi/bindings/npruntime.h"
#include "webkit/renderer/webkit_renderer_export.h"

namespace webkit_glue {

class WEBKIT_RENDERER_EXPORT CppVariant : public NPVariant {
 public:
  CppVariant();
  ~CppVariant();
  void SetNull();
  void Set(bool value);
  void Set(int32_t value);
  void Set(double value);

  
  
  
  
  void Set(const char* value);  
  void Set(const std::string& value);
  void Set(const NPString& new_value);
  void Set(const NPVariant& new_value);

  
  
  
  
  
  void Set(NPObject* new_value);

  
  
  
  
  
  
  
  void CopyToNPVariant(NPVariant* result) const;
  CppVariant& operator=(const CppVariant& original);
  CppVariant(const CppVariant& original);

  
  
  
  
  void FreeData();

  
  
  
  
  
  bool isEqual(const CppVariant& other) const;

  
  
  
  
  
  bool isBool() const { return (type == NPVariantType_Bool); }
  bool isInt32() const { return (type == NPVariantType_Int32); }
  bool isDouble() const { return (type == NPVariantType_Double); }
  bool isNumber() const { return (isInt32() || isDouble()); }
  bool isString() const { return (type == NPVariantType_String); }
  bool isVoid() const { return (type == NPVariantType_Void); }
  bool isNull() const { return (type == NPVariantType_Null); }
  bool isEmpty() const { return (isVoid() || isNull()); }
  bool isObject() const { return (type == NPVariantType_Object); }

  
  
  std::string ToString() const;
  int32_t ToInt32() const;
  double ToDouble() const;
  bool ToBoolean() const;
  
  
  
  std::vector<CppVariant> ToVector() const;

  
  
  
  
  
  bool Invoke(const std::string& method, const CppVariant* args,
              uint32 arg_count, CppVariant& result) const;
};

}  

#endif  

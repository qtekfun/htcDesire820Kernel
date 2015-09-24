// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_MAPPER_H_
#define CHROMEOS_NETWORK_ONC_ONC_MAPPER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"

namespace base {
class DictionaryValue;
class ListValue;
class Value;
}

namespace chromeos {
namespace onc {

struct OncValueSignature;

class CHROMEOS_EXPORT Mapper {
 public:
  Mapper();
  virtual ~Mapper();

 protected:
  
  
  
  virtual scoped_ptr<base::Value> MapValue(const OncValueSignature& signature,
                                           const base::Value& onc_value,
                                           bool* error);

  
  
  
  
  virtual scoped_ptr<base::DictionaryValue> MapObject(
      const OncValueSignature& signature,
      const base::DictionaryValue& onc_object,
      bool* error);

  
  
  
  virtual scoped_ptr<base::Value> MapPrimitive(
      const OncValueSignature& signature,
      const base::Value& onc_primitive,
      bool* error);

  
  
  
  
  
  virtual void MapFields(const OncValueSignature& object_signature,
                         const base::DictionaryValue& onc_object,
                         bool* found_unknown_field,
                         bool* nested_error,
                         base::DictionaryValue* result);

  
  
  
  
  virtual scoped_ptr<base::Value> MapField(
      const std::string& field_name,
      const OncValueSignature& object_signature,
      const base::Value& onc_value,
      bool* found_unknown_field,
      bool* error);

  
  
  
  
  
  virtual scoped_ptr<base::ListValue> MapArray(
      const OncValueSignature& array_signature,
      const base::ListValue& onc_array,
      bool* nested_error);

  
  
  virtual scoped_ptr<base::Value> MapEntry(int index,
                                           const OncValueSignature& signature,
                                           const base::Value& onc_value,
                                           bool* error);

 private:
  DISALLOW_COPY_AND_ASSIGN(Mapper);
};

}  
}  

#endif  

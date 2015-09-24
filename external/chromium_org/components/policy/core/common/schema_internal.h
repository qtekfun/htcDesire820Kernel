// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_SCHEMA_INTERNAL_H_
#define COMPONENTS_POLICY_CORE_COMMON_SCHEMA_INTERNAL_H_

#include "base/values.h"
#include "components/policy/policy_export.h"

namespace policy {
namespace internal {


struct POLICY_EXPORT SchemaNode {
  
  base::Value::Type type;

  
  
  
  
  
  
  
  
  
  int extra;
};

struct POLICY_EXPORT PropertyNode {
  
  const char* key;

  
  
  int schema;
};

struct POLICY_EXPORT PropertiesNode {
  
  
  int begin;

  
  
  
  
  
  
  
  
  int end;

  
  
  
  
  
  int additional;
};

struct POLICY_EXPORT SchemaData {
  const SchemaNode* schema_nodes;
  const PropertyNode* property_nodes;
  const PropertiesNode* properties_nodes;
};

}  
}  

#endif  

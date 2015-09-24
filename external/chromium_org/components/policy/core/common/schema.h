// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_SCHEMA_H_
#define COMPONENTS_POLICY_CORE_COMMON_SCHEMA_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "components/policy/policy_export.h"

namespace policy {
namespace internal {

struct POLICY_EXPORT SchemaData;
struct POLICY_EXPORT SchemaNode;
struct POLICY_EXPORT PropertyNode;
struct POLICY_EXPORT PropertiesNode;

}  

class POLICY_EXPORT Schema {
 public:
  
  class InternalStorage;

  
  Schema();

  
  Schema(const Schema& schema);

  ~Schema();

  Schema& operator=(const Schema& schema);

  
  
  
  static Schema Wrap(const internal::SchemaData* data);

  
  
  
  static Schema Parse(const std::string& schema, std::string* error);

  
  
  bool valid() const { return node_ != NULL; }

  base::Value::Type type() const;

  
  bool Validate(const base::Value& value) const;

  
  class POLICY_EXPORT Iterator {
   public:
    Iterator(const scoped_refptr<const InternalStorage>& storage,
             const internal::PropertiesNode* node);
    Iterator(const Iterator& iterator);
    ~Iterator();

    Iterator& operator=(const Iterator& iterator);

    
    bool IsAtEnd() const;

    
    void Advance();

    
    const char* key() const;

    
    Schema schema() const;

   private:
    scoped_refptr<const InternalStorage> storage_;
    const internal::PropertyNode* it_;
    const internal::PropertyNode* end_;
  };

  
  

  
  
  Iterator GetPropertiesIterator() const;

  
  
  Schema GetKnownProperty(const std::string& key) const;

  
  
  Schema GetAdditionalProperties() const;

  
  
  Schema GetProperty(const std::string& key) const;

  
  
  
  Schema GetItems() const;

 private:
  
  
  Schema(const scoped_refptr<const InternalStorage>& storage,
         const internal::SchemaNode* node);

  scoped_refptr<const InternalStorage> storage_;
  const internal::SchemaNode* node_;
};

}  

#endif  

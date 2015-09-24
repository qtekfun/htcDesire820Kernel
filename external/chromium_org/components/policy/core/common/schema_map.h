// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_SCHEMA_MAP_H_
#define COMPONENTS_POLICY_CORE_COMMON_SCHEMA_MAP_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "components/policy/core/common/policy_namespace.h"
#include "components/policy/core/common/schema.h"
#include "components/policy/policy_export.h"

namespace policy {

class PolicyBundle;

typedef std::map<std::string, Schema> ComponentMap;
typedef std::map<PolicyDomain, ComponentMap> DomainMap;

class POLICY_EXPORT SchemaMap : public base::RefCountedThreadSafe<SchemaMap> {
 public:
  SchemaMap();
  
  explicit SchemaMap(DomainMap& map);

  const DomainMap& GetDomains() const;

  const ComponentMap* GetComponents(PolicyDomain domain) const;

  const Schema* GetSchema(const PolicyNamespace& ns) const;

  
  
  void FilterBundle(PolicyBundle* bundle) const;

  
  
  bool HasComponents() const;

  void GetChanges(const scoped_refptr<SchemaMap>& older,
                  PolicyNamespaceList* removed,
                  PolicyNamespaceList* added) const;

 private:
  friend class base::RefCountedThreadSafe<SchemaMap>;

  void GetNamespacesNotInOther(const SchemaMap* other,
                               PolicyNamespaceList* list) const;

  ~SchemaMap();

  DomainMap map_;

  DISALLOW_COPY_AND_ASSIGN(SchemaMap);
};

}  

#endif  

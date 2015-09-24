// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_DEDUPING_FACTORY_H__
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_DEDUPING_FACTORY_H__

#include <list>
#include <string>

#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/stl_util.h"

namespace base {
class Value;
}  

namespace extensions {

template<typename BaseClassT>
class DedupingFactory {
 public:
  
  
  
  
  
  typedef scoped_refptr<const BaseClassT>
      (* FactoryMethod)(const std::string& instance_type,
                        const base::Value*  ,
                        std::string* ,
                        bool* );

  enum Parameterized {
    
    
    IS_PARAMETERIZED,
    
    
    IS_NOT_PARAMETERIZED
  };

  
  
  
  
  explicit DedupingFactory(size_t max_number_prototypes);
  ~DedupingFactory();

  void RegisterFactoryMethod(const std::string& instance_type,
                             Parameterized parameterized,
                             FactoryMethod factory_method);

  scoped_refptr<const BaseClassT> Instantiate(const std::string& instance_type,
                                              const base::Value* value,
                                              std::string* error,
                                              bool* bad_message);

  void ClearPrototypes();

 private:
  typedef std::string InstanceType;
  
  
  typedef std::list<scoped_refptr<const BaseClassT> > PrototypeList;
  typedef base::hash_map<InstanceType, PrototypeList> ExistingPrototypes;
  typedef base::hash_map<InstanceType, FactoryMethod> FactoryMethods;
  typedef base::hash_set<InstanceType> ParameterizedTypes;

  const size_t max_number_prototypes_;
  ExistingPrototypes prototypes_;
  FactoryMethods factory_methods_;
  ParameterizedTypes parameterized_types_;

  DISALLOW_COPY_AND_ASSIGN(DedupingFactory);
};

template<typename BaseClassT>
DedupingFactory<BaseClassT>::DedupingFactory(size_t max_number_prototypes)
    : max_number_prototypes_(max_number_prototypes) {}

template<typename BaseClassT>
DedupingFactory<BaseClassT>::~DedupingFactory() {}

template<typename BaseClassT>
void DedupingFactory<BaseClassT>::RegisterFactoryMethod(
    const std::string& instance_type,
    typename DedupingFactory<BaseClassT>::Parameterized parameterized,
    FactoryMethod factory_method) {
  DCHECK(!ContainsKey(factory_methods_, instance_type));
  factory_methods_[instance_type] = factory_method;
  if (parameterized == IS_PARAMETERIZED)
    parameterized_types_.insert(instance_type);
}

template<typename BaseClassT>
scoped_refptr<const BaseClassT> DedupingFactory<BaseClassT>::Instantiate(
    const std::string& instance_type,
    const base::Value* value,
    std::string* error,
    bool* bad_message) {
  typename FactoryMethods::const_iterator factory_method_iter =
      factory_methods_.find(instance_type);
  if (factory_method_iter == factory_methods_.end()) {
    *error = "Invalid instance type " + instance_type;
    *bad_message = true;
    return scoped_refptr<const BaseClassT>();
  }

  FactoryMethod factory_method = factory_method_iter->second;

  PrototypeList& prototypes = prototypes_[instance_type];

  
  
  
  if (!ContainsKey(parameterized_types_, instance_type)) {
    if (prototypes.empty()) {
      scoped_refptr<const BaseClassT> new_object =
          (*factory_method)(instance_type, value, error, bad_message);
      if (!new_object.get() || !error->empty() || *bad_message)
        return scoped_refptr<const BaseClassT>();
      prototypes.push_back(new_object);
    }
    return prototypes.front();
  }

  
  scoped_refptr<const BaseClassT> new_object =
      (*factory_method)(instance_type, value, error, bad_message);
  if (!new_object.get() || !error->empty() || *bad_message)
    return scoped_refptr<const BaseClassT>();

  size_t length = 0;
  for (typename PrototypeList::iterator i = prototypes.begin();
       i != prototypes.end();
       ++i) {
    if ((*i)->Equals(new_object.get())) {
      
      
      scoped_refptr<const BaseClassT> old_object = *i;
      prototypes.erase(i);
      prototypes.push_back(old_object);
      return old_object;
    }
    ++length;
  }

  if (length >= max_number_prototypes_)
    prototypes.pop_front();
  prototypes.push_back(new_object);

  return new_object;
}

template<typename BaseClassT>
void DedupingFactory<BaseClassT>::ClearPrototypes() {
  prototypes_.clear();
}

}  

#endif  

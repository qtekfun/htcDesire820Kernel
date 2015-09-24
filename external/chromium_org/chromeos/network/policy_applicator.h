// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_POLICY_APPLICATOR_H_
#define CHROMEOS_NETWORK_POLICY_APPLICATOR_H_

#include <map>
#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "chromeos/network/network_profile.h"

namespace chromeos {

class PolicyApplicator : public base::RefCounted<PolicyApplicator> {
 public:
  class ConfigurationHandler {
    public:
     virtual ~ConfigurationHandler() {}
     
     
     
     
     virtual void CreateConfigurationFromPolicy(
         const base::DictionaryValue& shill_properties) = 0;

     virtual void UpdateExistingConfigurationWithPropertiesFromPolicy(
         const base::DictionaryValue& existing_properties,
         const base::DictionaryValue& new_properties) = 0;

    private:
     DISALLOW_ASSIGN(ConfigurationHandler);
  };

  typedef std::map<std::string, const base::DictionaryValue*> GuidToPolicyMap;

  
  PolicyApplicator(base::WeakPtr<ConfigurationHandler> handler,
                   const NetworkProfile& profile,
                   const GuidToPolicyMap& all_policies,
                   const base::DictionaryValue& global_network_config,
                   std::set<std::string>* modified_policies);

  void Run();

 private:
  friend class base::RefCounted<PolicyApplicator>;

  
  
  void GetProfilePropertiesCallback(
      const base::DictionaryValue& profile_properties);

  
  
  
  void GetEntryCallback(const std::string& entry,
                        const base::DictionaryValue& entry_properties);

  
  void DeleteEntry(const std::string& entry);

  
  void WriteNewShillConfiguration(const base::DictionaryValue& shill_dictionary,
                                  const base::DictionaryValue& policy);

  
  
  
  
  void GetPropertiesForUnmanagedEntry(
      const base::DictionaryValue& entry_properties,
      base::DictionaryValue* properties_to_update) const;

  
  
  virtual ~PolicyApplicator();

  
  
  void ApplyRemainingPolicies();

  std::set<std::string> remaining_policies_;
  base::WeakPtr<ConfigurationHandler> handler_;
  NetworkProfile profile_;
  GuidToPolicyMap all_policies_;
  base::DictionaryValue global_network_config_;

  DISALLOW_COPY_AND_ASSIGN(PolicyApplicator);
};

}  

#endif  

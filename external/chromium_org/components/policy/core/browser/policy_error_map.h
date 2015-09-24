// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_BROWSER_POLICY_ERROR_MAP_H_
#define COMPONENTS_POLICY_CORE_BROWSER_POLICY_ERROR_MAP_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "components/policy/policy_export.h"

namespace policy {

class POLICY_EXPORT PolicyErrorMap {
 public:
  typedef std::multimap<std::string, base::string16> PolicyMapType;
  typedef PolicyMapType::const_iterator const_iterator;

  PolicyErrorMap();
  virtual ~PolicyErrorMap();

  
  
  
  bool IsReady() const;

  
  
  void AddError(const std::string& policy, int message_id);

  
  
  void AddError(const std::string& policy,
                const std::string& subkey,
                int message_id);

  
  
  void AddError(const std::string& policy,
                int index,
                int message_id);

  
  
  
  void AddError(const std::string& policy,
                int message_id,
                const std::string& replacement_string);

  
  
  
  void AddError(const std::string& policy,
                const std::string& subkey,
                int message_id,
                const std::string& replacement_string);

  
  
  
  void AddError(const std::string& policy,
                int index,
                int message_id,
                const std::string& replacement_string);

  
  
  base::string16 GetErrors(const std::string& policy);

  bool empty();
  size_t size();

  const_iterator begin();
  const_iterator end();

  void Clear();

 private:
  struct PendingError;

  
  void AddError(const PendingError& error);

  
  void Convert(const PendingError& error);

  
  void CheckReadyAndConvert();

  std::vector<PendingError> pending_;
  PolicyMapType map_;

  DISALLOW_COPY_AND_ASSIGN(PolicyErrorMap);
};

}  

#endif  

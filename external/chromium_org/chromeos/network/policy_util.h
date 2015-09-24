// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_POLICY_UTIL_H_
#define CHROMEOS_NETWORK_POLICY_UTIL_H_

#include <map>
#include <string>

#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {

struct NetworkProfile;

namespace policy_util {

typedef std::map<std::string, const base::DictionaryValue*> GuidToPolicyMap;

scoped_ptr<base::DictionaryValue> CreateShillConfiguration(
    const NetworkProfile& profile,
    const std::string& guid,
    const base::DictionaryValue* policy,
    const base::DictionaryValue* settings);

const base::DictionaryValue* FindMatchingPolicy(
    const GuidToPolicyMap& policies,
    const base::DictionaryValue& actual_network);

}  

}  

#endif  

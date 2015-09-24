// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_CLOUD_EXTERNAL_DATA_MANAGER_BASE_TEST_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_CLOUD_EXTERNAL_DATA_MANAGER_BASE_TEST_UTIL_H_

#include <string>

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}

namespace policy {

class CloudPolicyCore;

namespace test {

void ExternalDataFetchCallback(scoped_ptr<std::string>* destination,
                               const base::Closure& done_callback,
                               scoped_ptr<std::string> data);

scoped_ptr<base::DictionaryValue> ConstructExternalDataReference(
    const std::string& url,
    const std::string& data);

void SetExternalDataReference(CloudPolicyCore* core,
                              const std::string& policy,
                              scoped_ptr<base::DictionaryValue> metadata);

}  
}  

#endif  

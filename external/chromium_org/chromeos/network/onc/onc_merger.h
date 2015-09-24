// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_MERGER_H_
#define CHROMEOS_NETWORK_ONC_ONC_MERGER_H_

#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {
namespace onc {

struct OncValueSignature;

CHROMEOS_EXPORT scoped_ptr<base::DictionaryValue>
MergeSettingsAndPoliciesToEffective(
    const base::DictionaryValue* user_policy,
    const base::DictionaryValue* device_policy,
    const base::DictionaryValue* user_settings,
    const base::DictionaryValue* shared_settings);

// overrides all other values. Credentials from policies are not written to the
CHROMEOS_EXPORT scoped_ptr<base::DictionaryValue>
MergeSettingsAndPoliciesToAugmented(
    const OncValueSignature& signature,
    const base::DictionaryValue* user_policy,
    const base::DictionaryValue* device_policy,
    const base::DictionaryValue* user_settings,
    const base::DictionaryValue* shared_settings,
    const base::DictionaryValue* active_settings);

}  
}  

#endif  

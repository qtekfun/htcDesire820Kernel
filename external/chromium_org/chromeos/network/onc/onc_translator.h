// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_NETWORK_ONC_ONC_TRANSLATOR_H_
#define CHROMEOS_NETWORK_ONC_ONC_TRANSLATOR_H_

#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {
namespace onc {

struct OncValueSignature;

CHROMEOS_EXPORT
scoped_ptr<base::DictionaryValue> TranslateONCObjectToShill(
    const OncValueSignature* signature,
    const base::DictionaryValue& onc_object);

CHROMEOS_EXPORT
scoped_ptr<base::DictionaryValue> TranslateShillServiceToONCPart(
    const base::DictionaryValue& shill_dictionary,
    const OncValueSignature* onc_signature);

}  
}  

#endif  

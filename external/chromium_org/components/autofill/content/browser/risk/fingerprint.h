// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_BROWSER_RISK_FINGERPRINT_H_
#define COMPONENTS_AUTOFILL_CONTENT_BROWSER_RISK_FINGERPRINT_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "components/autofill/core/browser/autofill_manager_delegate.h"

class PrefService;

namespace base {
class Time;
}

namespace content {
class WebContents;
}

namespace gfx {
class Rect;
}

namespace blink {
struct WebScreenInfo;
}

namespace autofill {
namespace risk {

class Fingerprint;

void GetFingerprint(
    uint64 obfuscated_gaia_id,
    const gfx::Rect& window_bounds,
    const content::WebContents& web_contents,
    const std::string& version,
    const std::string& charset,
    const std::string& accept_languages,
    const base::Time& install_time,
    const std::string& app_locale,
    const base::Callback<void(scoped_ptr<Fingerprint>)>& callback);

}  
}  

#endif  

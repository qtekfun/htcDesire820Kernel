// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_UI_ACCOUNT_TWEAKS_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_UI_ACCOUNT_TWEAKS_H_

#include "base/values.h"
#include "base/compiler_specific.h"

namespace content {
class WebUIDataSource;
}

namespace chromeos {

void AddAccountUITweaksLocalizedValues(
    base::DictionaryValue* localized_strings);

void AddAccountUITweaksLocalizedValues(content::WebUIDataSource* source);

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_INSTANT_UI_H_
#define CHROME_BROWSER_UI_WEBUI_INSTANT_UI_H_

#include "content/public/browser/web_ui_controller.h"


namespace content {
class BrowserContext;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class InstantUI : public content::WebUIController {
 public:
  
  
  explicit InstantUI(content::WebUI* web_ui);

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

 private:
  DISALLOW_COPY_AND_ASSIGN(InstantUI);
};

#endif  

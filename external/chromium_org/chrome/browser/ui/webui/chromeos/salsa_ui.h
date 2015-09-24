// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_SALSA_UI_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_SALSA_UI_H_

#include <map>

#include "base/values.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/web_ui_controller.h"

namespace base {
class ListValue;
class Value;
}  

class SalsaUI : public content::WebUIController {
 public:
  
  explicit SalsaUI(content::WebUI* web_ui);
  virtual ~SalsaUI();

 private:
  
  
  
  void SetPreferenceValue(const base::ListValue* args);

  
  
  void BackupPreferenceValue(const base::ListValue* args);

  
  
  int WhitelistIndex(const char* key) const;

  std::map<int, const base::Value*> orig_values_;

  DISALLOW_COPY_AND_ASSIGN(SalsaUI);
};

#endif  


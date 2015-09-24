// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_GESTURE_CONFIG_UI_H_
#define CHROME_BROWSER_UI_WEBUI_GESTURE_CONFIG_UI_H_

#include "content/public/browser/web_ui_controller.h"

namespace base {
  class ListValue;
}  

class GestureConfigUI : public content::WebUIController {
 public:
  
  explicit GestureConfigUI(content::WebUI* web_ui);
  virtual ~GestureConfigUI();

 private:
  
  

  void UpdatePreferenceValue(const base::ListValue* args);

  void ResetPreferenceValue(const base::ListValue* args);

  void SetPreferenceValue(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(GestureConfigUI);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_SET_AS_DEFAULT_BROWSER_UI_H_
#define CHROME_BROWSER_UI_WEBUI_SET_AS_DEFAULT_BROWSER_UI_H_

#include "ui/web_dialogs/web_dialog_ui.h"

class Browser;
class Profile;

class SetAsDefaultBrowserUI : public ui::WebDialogUI {
 public:
  explicit SetAsDefaultBrowserUI(content::WebUI* web_ui);

  
  static void Show(Profile* profile, Browser* browser);

 private:
  DISALLOW_COPY_AND_ASSIGN(SetAsDefaultBrowserUI);
};

#endif  

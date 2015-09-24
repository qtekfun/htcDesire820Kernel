// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_INLINE_LOGIN_UI_H_
#define CHROME_BROWSER_UI_WEBUI_INLINE_LOGIN_UI_H_

#include "chrome/browser/extensions/signin/scoped_gaia_auth_extension.h"
#include "ui/web_dialogs/web_dialog_ui.h"

class InlineLoginUI : public ui::WebDialogUI {
 public:
  explicit InlineLoginUI(content::WebUI* web_ui);
  virtual ~InlineLoginUI();

 private:
  ScopedGaiaAuthExtension auth_extension_;

  DISALLOW_COPY_AND_ASSIGN(InlineLoginUI);
};

#endif  

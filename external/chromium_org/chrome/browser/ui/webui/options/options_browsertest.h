// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_OPTIONS_BROWSERTEST_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_OPTIONS_BROWSERTEST_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/test/base/web_ui_browsertest.h"
#include "content/public/browser/web_ui_message_handler.h"

class OptionsBrowserTest : public WebUIBrowserTest,
                           public content::WebUIMessageHandler {
 public:
  OptionsBrowserTest();
  virtual ~OptionsBrowserTest();

 protected:
  
  void ClearPref(const char* path);

 private:
  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual content::WebUIMessageHandler* GetMockMessageHandler() OVERRIDE;

  
  
  
  void ReportHistory(const base::ListValue* list_value);

  
  
  
  void HandleSetPref(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(OptionsBrowserTest);
};

#endif  

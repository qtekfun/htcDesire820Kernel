// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_WEB_UI_TEST_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_WEB_UI_TEST_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace base {
class ListValue;
class Value;
}

namespace content {
class RenderViewHost;
}

class WebUITestHandler : public content::WebUIMessageHandler {
 public:
  WebUITestHandler();

  
  
  void PreloadJavaScript(const base::string16& js_text,
                         content::RenderViewHost* preload_host);

  
  void RunJavaScript(const base::string16& js_text);

  
  
  bool RunJavaScriptTestWithResult(const base::string16& js_text);

  
  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  void HandleTestResult(const base::ListValue* test_result);

  
  void JavaScriptComplete(const base::Value* result);

  
  
  bool WaitForResult();

  
  bool test_done_;

  
  bool test_succeeded_;

  
  
  
  bool run_test_done_;

  
  
  bool run_test_succeeded_;

  
  bool is_waiting_;

  DISALLOW_COPY_AND_ASSIGN(WebUITestHandler);
};

#endif  

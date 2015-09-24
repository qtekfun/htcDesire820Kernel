// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_UI_MESSAGE_HANDLER_H_
#define CONTENT_PUBLIC_BROWSER_WEB_UI_MESSAGE_HANDLER_H_

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"

class GURL;
class WebUIBrowserTest;

namespace base {
class DictionaryValue;
class ListValue;
}

namespace content {

class WebUI;
class WebUIImpl;

class CONTENT_EXPORT WebUIMessageHandler {
 public:
  WebUIMessageHandler() : web_ui_(NULL) {}
  virtual ~WebUIMessageHandler() {}

 protected:
  FRIEND_TEST_ALL_PREFIXES(WebUIMessageHandlerTest, ExtractIntegerValue);
  FRIEND_TEST_ALL_PREFIXES(WebUIMessageHandlerTest, ExtractDoubleValue);
  FRIEND_TEST_ALL_PREFIXES(WebUIMessageHandlerTest, ExtractStringValue);

  

  
  static bool ExtractIntegerValue(const base::ListValue* value, int* out_int);

  
  static bool ExtractDoubleValue(const base::ListValue* value,
                                 double* out_value);

  
  static base::string16 ExtractStringValue(const base::ListValue* value);

  
  
  
  virtual void RegisterMessages() = 0;

  
  WebUI* web_ui() const { return web_ui_; }

  
  void set_web_ui(WebUI* web_ui) { web_ui_ = web_ui; }

 private:
  
  friend class WebUIImpl;
  friend class ::WebUIBrowserTest;

  WebUI* web_ui_;
};

}  

#endif  

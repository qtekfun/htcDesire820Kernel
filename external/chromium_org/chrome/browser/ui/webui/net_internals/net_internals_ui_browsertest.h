// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NET_INTERNALS_NET_INTERNALS_UI_BROWSERTEST_H_
#define CHROME_BROWSER_UI_WEBUI_NET_INTERNALS_NET_INTERNALS_UI_BROWSERTEST_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/test/base/web_ui_browsertest.h"

class GURL;

namespace base {
class ListValue;
}  

class NetInternalsTest : public WebUIBrowserTest {
 protected:
  NetInternalsTest();
  virtual ~NetInternalsTest();

 private:
  class MessageHandler;

  
  virtual void SetUp() OVERRIDE;
  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;
  virtual void SetUpOnMainThread() OVERRIDE;

  
  virtual content::WebUIMessageHandler* GetMockMessageHandler() OVERRIDE;

  GURL CreatePrerenderLoaderUrl(const GURL& prerender_url);

  
  
  bool StartTestServer();

  scoped_ptr<MessageHandler> message_handler_;

  
  bool test_server_started_;

  DISALLOW_COPY_AND_ASSIGN(NetInternalsTest);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_CHROME_CHROME_H_
#define CHROME_TEST_CHROMEDRIVER_CHROME_CHROME_H_

#include <list>
#include <string>

class ChromeDesktopImpl;
class Status;
class WebView;

class Chrome {
 public:
  virtual ~Chrome() {}

  virtual ChromeDesktopImpl* GetAsDesktop() = 0;

  virtual std::string GetVersion() = 0;

  virtual int GetBuildNo() = 0;

  virtual bool HasCrashedWebView() = 0;

  
  virtual Status GetWebViewIds(std::list<std::string>* web_view_ids) = 0;

  
  virtual Status GetWebViewById(const std::string& id, WebView** web_view) = 0;

  
  virtual Status CloseWebView(const std::string& id) = 0;

  
  virtual Status ActivateWebView(const std::string& id) = 0;

  
  virtual std::string GetOperatingSystemName() = 0;

  
  virtual Status Quit() = 0;
};

#endif  

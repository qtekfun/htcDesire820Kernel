// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_CHROME_WEB_VIEW_H_
#define CHROME_TEST_CHROMEDRIVER_CHROME_WEB_VIEW_H_

#include <list>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class FilePath;
class ListValue;
class TimeDelta;
class Value;
}

class DevToolsClient;
struct Geoposition;
class JavaScriptDialogManager;
struct KeyEvent;
struct MouseEvent;
struct TouchEvent;
class Status;

class WebView {
 public:
  virtual ~WebView() {}

  
  virtual std::string GetId() = 0;

  
  virtual bool WasCrashed() = 0;

  
  virtual Status ConnectIfNecessary() = 0;

  
  virtual Status HandleReceivedEvents() = 0;

  
  virtual Status Load(const std::string& url) = 0;

  
  virtual Status Reload() = 0;

  
  
  
  
  
  virtual Status EvaluateScript(const std::string& frame,
                                const std::string& expression,
                                scoped_ptr<base::Value>* result) = 0;

  
  
  
  
  
  
  virtual Status CallFunction(const std::string& frame,
                              const std::string& function,
                              const base::ListValue& args,
                              scoped_ptr<base::Value>* result) = 0;

  
  
  
  
  
  virtual Status CallAsyncFunction(const std::string& frame,
                                   const std::string& function,
                                   const base::ListValue& args,
                                   const base::TimeDelta& timeout,
                                   scoped_ptr<base::Value>* result) = 0;

  
  
  
  
  virtual Status CallUserAsyncFunction(const std::string& frame,
                                       const std::string& function,
                                       const base::ListValue& args,
                                       const base::TimeDelta& timeout,
                                       scoped_ptr<base::Value>* result) = 0;

  
  
  
  virtual Status GetFrameByFunction(const std::string& frame,
                                    const std::string& function,
                                    const base::ListValue& args,
                                    std::string* out_frame) = 0;

  
  virtual Status DispatchMouseEvents(const std::list<MouseEvent>& events,
                                     const std::string& frame) = 0;

  
  virtual Status DispatchTouchEvents(const std::list<TouchEvent>& events) = 0;

  
  virtual Status DispatchKeyEvents(const std::list<KeyEvent>& events) = 0;

  
  virtual Status GetCookies(scoped_ptr<base::ListValue>* cookies) = 0;

  
  virtual Status DeleteCookie(const std::string& name,
                              const std::string& url) = 0;

  
  
  
  
  
  
  
  virtual Status WaitForPendingNavigations(const std::string& frame_id,
                                           const base::TimeDelta& timeout,
                                           bool stop_load_on_timeout) = 0;

  
  virtual Status IsPendingNavigation(
      const std::string& frame_id, bool* is_pending) = 0;

  
  virtual JavaScriptDialogManager* GetJavaScriptDialogManager() = 0;

  
  virtual Status OverrideGeolocation(const Geoposition& geoposition) = 0;

  
  virtual Status CaptureScreenshot(std::string* screenshot) = 0;

  
  
  virtual Status SetFileInputFiles(
      const std::string& frame,
      const base::DictionaryValue& element,
      const std::vector<base::FilePath>& files) = 0;

  
  
  
  
  virtual Status TakeHeapSnapshot(scoped_ptr<base::Value>* snapshot) = 0;
};

#endif  

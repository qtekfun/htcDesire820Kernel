// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_UI_H_
#define CONTENT_PUBLIC_BROWSER_WEB_UI_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/public/common/page_transition_types.h"
#include "ui/base/layout.h"

class GURL;

namespace base {
class ListValue;
class Value;
}

namespace content {

class WebContents;
class WebUIController;
class WebUIMessageHandler;

class CONTENT_EXPORT WebUI {
 public:
  
  
  typedef void* TypeID;

  
  
  static const TypeID kNoWebUI;

  
  
  static base::string16 GetJavascriptCall(
      const std::string& function_name,
      const std::vector<const base::Value*>& arg_list);

  virtual ~WebUI() {}

  virtual WebContents* GetWebContents() const = 0;

  virtual WebUIController* GetController() const = 0;
  virtual void SetController(WebUIController* controller) = 0;

  
  
  
  virtual ui::ScaleFactor GetDeviceScaleFactor() const = 0;

  
  
  
  virtual const base::string16& GetOverriddenTitle() const = 0;
  virtual void OverrideTitle(const base::string16& title) = 0;

  
  
  virtual PageTransition GetLinkTransitionType() const = 0;
  virtual void SetLinkTransitionType(PageTransition type) = 0;

  
  
  virtual int GetBindings() const = 0;
  virtual void SetBindings(int bindings) = 0;

  
  virtual void SetFrameXPath(const std::string& xpath) = 0;

  
  virtual void AddMessageHandler(WebUIMessageHandler* handler) = 0;

  
  
  
  typedef base::Callback<void(const base::ListValue*)> MessageCallback;
  virtual void RegisterMessageCallback(const std::string& message,
                                       const MessageCallback& callback) = 0;

  
  
  virtual void ProcessWebUIMessage(const GURL& source_url,
                                   const std::string& message,
                                   const base::ListValue& args) = 0;

  
  
  
  
  
  
  virtual void CallJavascriptFunction(const std::string& function_name) = 0;
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg) = 0;
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg1,
                                      const base::Value& arg2) = 0;
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg1,
                                      const base::Value& arg2,
                                      const base::Value& arg3) = 0;
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg1,
                                      const base::Value& arg2,
                                      const base::Value& arg3,
                                      const base::Value& arg4) = 0;
  virtual void CallJavascriptFunction(
      const std::string& function_name,
      const std::vector<const base::Value*>& args) = 0;
};

}  

#endif  

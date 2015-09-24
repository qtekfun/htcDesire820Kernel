// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEBUI_WEB_UI_IMPL_H_
#define CONTENT_BROWSER_WEBUI_WEB_UI_IMPL_H_

#include <map>

#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_ui.h"
#include "ipc/ipc_listener.h"

namespace content {
class RenderViewHost;

class CONTENT_EXPORT WebUIImpl : public WebUI,
                                 public IPC::Listener,
                                 public base::SupportsWeakPtr<WebUIImpl> {
 public:
  explicit WebUIImpl(WebContents* contents);
  virtual ~WebUIImpl();

  
  
  
  void RenderViewCreated(RenderViewHost* render_view_host);

  
  virtual WebContents* GetWebContents() const OVERRIDE;
  virtual WebUIController* GetController() const OVERRIDE;
  virtual void SetController(WebUIController* controller) OVERRIDE;
  virtual ui::ScaleFactor GetDeviceScaleFactor() const OVERRIDE;
  virtual const base::string16& GetOverriddenTitle() const OVERRIDE;
  virtual void OverrideTitle(const base::string16& title) OVERRIDE;
  virtual PageTransition GetLinkTransitionType() const OVERRIDE;
  virtual void SetLinkTransitionType(PageTransition type) OVERRIDE;
  virtual int GetBindings() const OVERRIDE;
  virtual void SetBindings(int bindings) OVERRIDE;
  virtual void SetFrameXPath(const std::string& xpath) OVERRIDE;
  virtual void AddMessageHandler(WebUIMessageHandler* handler) OVERRIDE;
  typedef base::Callback<void(const base::ListValue*)> MessageCallback;
  virtual void RegisterMessageCallback(
      const std::string& message,
      const MessageCallback& callback) OVERRIDE;
  virtual void ProcessWebUIMessage(const GURL& source_url,
                                   const std::string& message,
                                   const base::ListValue& args) OVERRIDE;
  virtual void CallJavascriptFunction(
      const std::string& function_name) OVERRIDE;
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg) OVERRIDE;
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg1,
                                      const base::Value& arg2) OVERRIDE;
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg1,
                                      const base::Value& arg2,
                                      const base::Value& arg3) OVERRIDE;
  virtual void CallJavascriptFunction(const std::string& function_name,
                                      const base::Value& arg1,
                                      const base::Value& arg2,
                                      const base::Value& arg3,
                                      const base::Value& arg4) OVERRIDE;
  virtual void CallJavascriptFunction(
      const std::string& function_name,
      const std::vector<const base::Value*>& args) OVERRIDE;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  
  void OnWebUISend(const GURL& source_url,
                   const std::string& message,
                   const base::ListValue& args);

  
  void ExecuteJavascript(const base::string16& javascript);

  
  typedef std::map<std::string, MessageCallback> MessageCallbackMap;
  MessageCallbackMap message_callbacks_;

  
  
  base::string16 overridden_title_;  
  PageTransition link_transition_type_;  
  int bindings_;  
                  

  
  ScopedVector<WebUIMessageHandler> handlers_;

  
  WebContents* web_contents_;

  
  
  std::string frame_xpath_;

  scoped_ptr<WebUIController> controller_;

  DISALLOW_COPY_AND_ASSIGN(WebUIImpl);
};

}  

#endif  

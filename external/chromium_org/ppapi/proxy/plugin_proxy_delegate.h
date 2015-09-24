// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_PROXY_DELEGATE_H_
#define PPAPI_PROXY_PLUGIN_PROXY_DELEGATE_H_

#include <string>

namespace IPC {
class Sender;
}

namespace ppapi {
namespace proxy {

class PPAPI_PROXY_EXPORT PluginProxyDelegate {
 public:
  virtual ~PluginProxyDelegate() {}

  
  
  
  
  virtual IPC::Sender* GetBrowserSender() = 0;

  
  virtual std::string GetUILanguage() = 0;

  
  
  
  virtual void PreCacheFont(const void* logfontw) = 0;

  
  virtual void SetActiveURL(const std::string& url) = 0;

  
  
  virtual PP_Resource CreateBrowserFont(
      Connection connection,
      PP_Instance instance,
      const PP_BrowserFont_Trusted_Description& desc,
      const Preferences& prefs) = 0;
};

}  
}  

#endif  

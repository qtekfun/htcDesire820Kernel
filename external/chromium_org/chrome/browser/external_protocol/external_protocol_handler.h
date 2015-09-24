// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTERNAL_PROTOCOL_EXTERNAL_PROTOCOL_HANDLER_H_
#define CHROME_BROWSER_EXTERNAL_PROTOCOL_EXTERNAL_PROTOCOL_HANDLER_H_

#include <string>

#include "chrome/browser/shell_integration.h"

class GURL;
class PrefRegistrySimple;

namespace base {
class DictionaryValue;
}

class ExternalProtocolHandler {
 public:
  enum BlockState {
    DONT_BLOCK,
    BLOCK,
    UNKNOWN,
  };

  
  class Delegate {
   public:
    virtual ShellIntegration::DefaultProtocolClientWorker* CreateShellWorker(
        ShellIntegration::DefaultWebClientObserver* observer,
        const std::string& protocol) = 0;
    virtual BlockState GetBlockState(const std::string& scheme) = 0;
    virtual void BlockRequest() = 0;
    virtual void RunExternalProtocolDialog(const GURL& url,
                                           int render_process_host_id,
                                           int routing_id) = 0;
    virtual void LaunchUrlWithoutSecurityCheck(const GURL& url) = 0;
    virtual void FinishedProcessingCheck() = 0;
    virtual ~Delegate() {}
  };

  
  static BlockState GetBlockState(const std::string& scheme);

  
  static void SetBlockState(const std::string& scheme, BlockState state);

  
  
  
  
  
  
  
  static void LaunchUrl(const GURL& url, int render_process_host_id,
                        int tab_contents_id) {
      LaunchUrlWithDelegate(url, render_process_host_id, tab_contents_id, NULL);
  }

  
  
  static void LaunchUrlWithDelegate(const GURL& url, int render_process_host_id,
                                    int tab_contents_id, Delegate* delegate);

  
  
  
  
  
  
  
  
  
  
  static void RunExternalProtocolDialog(const GURL& url,
                                        int render_process_host_id,
                                        int routing_id);

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  
  
  
  
  
  
  
  static void LaunchUrlWithoutSecurityCheck(const GURL& url,
                                            int render_process_host_id,
                                            int tab_contents_id);

  
  
  static void PrepopulateDictionary(base::DictionaryValue* win_pref);

  
  
  
  
  static void PermitLaunchUrl();
};

#endif  

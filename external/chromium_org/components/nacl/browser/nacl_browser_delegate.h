// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_BROWSER_NACL_BROWSER_DELEGATE_H_
#define COMPONENTS_NACL_BROWSER_NACL_BROWSER_DELEGATE_H_

#include <string>

#include "base/callback_forward.h"

class GURL;

namespace base {
class FilePath;
}

namespace ppapi {
namespace host {
class HostFactory;
}
}

namespace content {
class BrowserPpapiHost;
}

class NaClBrowserDelegate {
 public:
  virtual ~NaClBrowserDelegate() {}

  
  virtual void ShowNaClInfobar(int render_process_id, int render_view_id,
                               int error_id) = 0;
  
  
  virtual bool DialogsAreSuppressed() = 0;
  
  
  virtual bool GetCacheDirectory(base::FilePath* cache_dir) = 0;
  
  
  
  virtual bool GetPluginDirectory(base::FilePath* plugin_dir) = 0;
  
  
  
  virtual bool GetPnaclDirectory(base::FilePath* pnacl_dir) = 0;
  
  
  virtual bool GetUserDirectory(base::FilePath* user_dir) = 0;
  
  
  virtual std::string GetVersionString() const = 0;
  
  virtual ppapi::host::HostFactory* CreatePpapiHostFactory(
      content::BrowserPpapiHost* ppapi_host) = 0;
  
  
  
  
  
  
  
  
  virtual bool MapUrlToLocalFilePath(const GURL& url,
                                     bool use_blocking_api,
                                     base::FilePath* file_path) = 0;
  
  virtual void SetDebugPatterns(std::string debug_patterns) = 0;

  
  virtual bool URLMatchesDebugPatterns(const GURL& manifest_url) = 0;
};

#endif  

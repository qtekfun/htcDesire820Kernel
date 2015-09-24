// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_BROWSER_PPAPI_HOST_H_
#define CONTENT_PUBLIC_BROWSER_BROWSER_PPAPI_HOST_H_

#include "base/callback_forward.h"
#include "base/process/process.h"
#include "content/common/content_export.h"
#include "content/public/browser/render_view_host.h"
#include "ppapi/c/pp_instance.h"
#include "url/gurl.h"

namespace IPC {
class ChannelProxy;
struct ChannelHandle;
class Sender;
}

namespace ppapi {
class PpapiPermissions;
namespace host {
class PpapiHost;
}
}

namespace content {

class CONTENT_EXPORT BrowserPpapiHost {
 public:
  
  
  static BrowserPpapiHost* CreateExternalPluginProcess(
      IPC::Sender* sender,
      ppapi::PpapiPermissions permissions,
      base::ProcessHandle plugin_child_process,
      IPC::ChannelProxy* channel,
      int render_process_id,
      int render_view_id,
      const base::FilePath& profile_directory);

  virtual ~BrowserPpapiHost() {}

  
  virtual ppapi::host::PpapiHost* GetPpapiHost() = 0;

  
  virtual base::ProcessHandle GetPluginProcessHandle() const = 0;

  
  virtual bool IsValidInstance(PP_Instance instance) const = 0;

  
  
  
  
  
  
  
  
  
  virtual bool GetRenderViewIDsForInstance(PP_Instance instance,
                                           int* render_process_id,
                                           int* render_view_id) const = 0;

  
  virtual const std::string& GetPluginName() = 0;

  
  virtual const base::FilePath& GetPluginPath() = 0;

  
  virtual const base::FilePath& GetProfileDataDirectory() = 0;

  
  virtual GURL GetDocumentURLForInstance(PP_Instance instance) = 0;
  virtual GURL GetPluginURLForInstance(PP_Instance instance) = 0;
};

}  

#endif  

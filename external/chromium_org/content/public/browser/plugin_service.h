// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_PLUGIN_SERVICE_H_
#define CONTENT_PUBLIC_BROWSER_PLUGIN_SERVICE_H_

#include <string>
#include <vector>

#include "base/callback.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"

class GURL;

namespace base {
class FilePath;
}

namespace content {

class BrowserContext;
class PluginProcessHost;
class PluginServiceFilter;
class ResourceContext;
struct PepperPluginInfo;
struct WebPluginInfo;

class PluginService {
 public:
  typedef base::Callback<void(const std::vector<WebPluginInfo>&)>
      GetPluginsCallback;

  
  CONTENT_EXPORT static PluginService* GetInstance();

  
  
  
  
  
  CONTENT_EXPORT static void PurgePluginListCache(
      BrowserContext* browser_context,
      bool reload_pages);

  virtual ~PluginService() {}

  
  virtual void Init() = 0;

  
  virtual void StartWatchingPlugins() = 0;

  
  
  
  virtual bool GetPluginInfoArray(
      const GURL& url,
      const std::string& mime_type,
      bool allow_wildcard,
      std::vector<WebPluginInfo>* info,
      std::vector<std::string>* actual_mime_types) = 0;

  
  
  
  virtual bool GetPluginInfo(int render_process_id,
                             int render_frame_id,
                             ResourceContext* context,
                             const GURL& url,
                             const GURL& page_url,
                             const std::string& mime_type,
                             bool allow_wildcard,
                             bool* is_stale,
                             WebPluginInfo* info,
                             std::string* actual_mime_type) = 0;

  
  
  
  virtual bool GetPluginInfoByPath(const base::FilePath& plugin_path,
                                   WebPluginInfo* info) = 0;

  
  
  
  virtual base::string16 GetPluginDisplayNameByPath(
      const base::FilePath& plugin_path) = 0;

  
  
  virtual void GetPlugins(const GetPluginsCallback& callback) = 0;

  
  
  
  virtual PepperPluginInfo* GetRegisteredPpapiPluginInfo(
      const base::FilePath& plugin_path) = 0;

  virtual void SetFilter(PluginServiceFilter* filter) = 0;
  virtual PluginServiceFilter* GetFilter() = 0;

  
  virtual void ForcePluginShutdown(const base::FilePath& plugin_path) = 0;

  
  
  virtual bool IsPluginUnstable(const base::FilePath& plugin_path) = 0;

  
  
  virtual void RefreshPlugins() = 0;

  
  
  virtual void AddExtraPluginPath(const base::FilePath& path) = 0;
  virtual void RemoveExtraPluginPath(const base::FilePath& path) = 0;

  
  virtual void AddExtraPluginDir(const base::FilePath& path) = 0;

  
  
  
  
  
  virtual void RegisterInternalPlugin(const WebPluginInfo& info,
                                      bool add_at_beginning) = 0;

  
  
  virtual void UnregisterInternalPlugin(const base::FilePath& path) = 0;

  
  virtual void GetInternalPlugins(std::vector<WebPluginInfo>* plugins) = 0;

  
  
  virtual bool NPAPIPluginsSupported() = 0;

  
  
  virtual void DisablePluginsDiscoveryForTesting() = 0;

#if defined(OS_MACOSX)
  
  
  virtual void AppActivated() = 0;
#elif defined(OS_WIN)
  
  
  
  virtual bool GetPluginInfoFromWindow(HWND window,
                                       base::string16* plugin_name,
                                       base::string16* plugin_version) = 0;
#endif

  
  virtual bool PpapiDevChannelSupported() = 0;
};

}  

#endif  

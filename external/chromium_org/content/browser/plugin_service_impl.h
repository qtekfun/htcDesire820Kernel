// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_PLUGIN_SERVICE_IMPL_H_
#define CONTENT_BROWSER_PLUGIN_SERVICE_IMPL_H_

#include <map>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/singleton.h"
#include "base/synchronization/waitable_event_watcher.h"
#include "base/threading/sequenced_worker_pool.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "content/browser/plugin_process_host.h"
#include "content/browser/ppapi_plugin_process_host.h"
#include "content/common/content_export.h"
#include "content/public/browser/plugin_service.h"
#include "content/public/common/pepper_plugin_info.h"
#include "ipc/ipc_channel_handle.h"
#include "url/gurl.h"

#if defined(OS_WIN)
#include "base/memory/scoped_ptr.h"
#include "base/win/registry.h"
#endif

#if defined(OS_POSIX) && !defined(OS_OPENBSD) && !defined(OS_ANDROID)
#include "base/files/file_path_watcher.h"
#endif

namespace base {
class MessageLoopProxy;
}

namespace webkit {
namespace npapi {
class PluginList;
}
}

namespace content {
class BrowserContext;
class PluginDirWatcherDelegate;
class PluginLoaderPosix;
class PluginServiceFilter;
class ResourceContext;
struct PepperPluginInfo;

struct PluginServiceFilterParams {
  int render_process_id;
  int render_frame_id;
  GURL page_url;
  ResourceContext* resource_context;
};

class CONTENT_EXPORT PluginServiceImpl
    : NON_EXPORTED_BASE(public PluginService) {
 public:
  
  static PluginServiceImpl* GetInstance();

  
  virtual void Init() OVERRIDE;
  virtual void StartWatchingPlugins() OVERRIDE;
  virtual bool GetPluginInfoArray(
      const GURL& url,
      const std::string& mime_type,
      bool allow_wildcard,
      std::vector<WebPluginInfo>* info,
      std::vector<std::string>* actual_mime_types) OVERRIDE;
  virtual bool GetPluginInfo(int render_process_id,
                             int render_frame_id,
                             ResourceContext* context,
                             const GURL& url,
                             const GURL& page_url,
                             const std::string& mime_type,
                             bool allow_wildcard,
                             bool* is_stale,
                             WebPluginInfo* info,
                             std::string* actual_mime_type) OVERRIDE;
  virtual bool GetPluginInfoByPath(const base::FilePath& plugin_path,
                                   WebPluginInfo* info) OVERRIDE;
  virtual base::string16 GetPluginDisplayNameByPath(
      const base::FilePath& path) OVERRIDE;
  virtual void GetPlugins(const GetPluginsCallback& callback) OVERRIDE;
  virtual PepperPluginInfo* GetRegisteredPpapiPluginInfo(
      const base::FilePath& plugin_path) OVERRIDE;
  virtual void SetFilter(PluginServiceFilter* filter) OVERRIDE;
  virtual PluginServiceFilter* GetFilter() OVERRIDE;
  virtual void ForcePluginShutdown(const base::FilePath& plugin_path) OVERRIDE;
  virtual bool IsPluginUnstable(const base::FilePath& plugin_path) OVERRIDE;
  virtual void RefreshPlugins() OVERRIDE;
  virtual void AddExtraPluginPath(const base::FilePath& path) OVERRIDE;
  virtual void RemoveExtraPluginPath(const base::FilePath& path) OVERRIDE;
  virtual void AddExtraPluginDir(const base::FilePath& path) OVERRIDE;
  virtual void RegisterInternalPlugin(
      const WebPluginInfo& info, bool add_at_beginning) OVERRIDE;
  virtual void UnregisterInternalPlugin(const base::FilePath& path) OVERRIDE;
  virtual void GetInternalPlugins(
      std::vector<WebPluginInfo>* plugins) OVERRIDE;
  virtual bool NPAPIPluginsSupported() OVERRIDE;
  virtual void DisablePluginsDiscoveryForTesting() OVERRIDE;
#if defined(OS_MACOSX)
  virtual void AppActivated() OVERRIDE;
#elif defined(OS_WIN)
  virtual bool GetPluginInfoFromWindow(HWND window,
                                       base::string16* plugin_name,
                                       base::string16* plugin_version) OVERRIDE;

  
  bool IsPluginWindow(HWND window);
#endif
  virtual bool PpapiDevChannelSupported() OVERRIDE;

  
  
  
  
  PluginProcessHost* FindOrStartNpapiPluginProcess(
      int render_process_id, const base::FilePath& plugin_path);
  PpapiPluginProcessHost* FindOrStartPpapiPluginProcess(
      int render_process_id,
      const base::FilePath& plugin_path,
      const base::FilePath& profile_data_directory);
  PpapiPluginProcessHost* FindOrStartPpapiBrokerProcess(
      int render_process_id, const base::FilePath& plugin_path);

  
  
  
  void OpenChannelToNpapiPlugin(int render_process_id,
                                int render_frame_id,
                                const GURL& url,
                                const GURL& page_url,
                                const std::string& mime_type,
                                PluginProcessHost::Client* client);
  void OpenChannelToPpapiPlugin(int render_process_id,
                                const base::FilePath& plugin_path,
                                const base::FilePath& profile_data_directory,
                                PpapiPluginProcessHost::PluginClient* client);
  void OpenChannelToPpapiBroker(int render_process_id,
                                const base::FilePath& path,
                                PpapiPluginProcessHost::BrokerClient* client);

  
  void CancelOpenChannelToNpapiPlugin(PluginProcessHost::Client* client);

  
  void RegisterPluginCrash(const base::FilePath& plugin_path);

 private:
  friend struct DefaultSingletonTraits<PluginServiceImpl>;

  
  
  PluginServiceImpl();
  virtual ~PluginServiceImpl();

  void OnWaitableEventSignaled(base::WaitableEvent* waitable_event);

  
  
  
  PluginProcessHost* FindNpapiPluginProcess(const base::FilePath& plugin_path);
  PpapiPluginProcessHost* FindPpapiPluginProcess(
      const base::FilePath& plugin_path,
      const base::FilePath& profile_data_directory);
  PpapiPluginProcessHost* FindPpapiBrokerProcess(
      const base::FilePath& broker_path);

  void RegisterPepperPlugins();

  
  void GetPluginsInternal(base::MessageLoopProxy* target_loop,
                          const GetPluginsCallback& callback);

  
  
  void ForwardGetAllowedPluginForOpenChannelToPlugin(
      const PluginServiceFilterParams& params,
      const GURL& url,
      const std::string& mime_type,
      PluginProcessHost::Client* client,
      const std::vector<WebPluginInfo>&);
  
  void GetAllowedPluginForOpenChannelToPlugin(
      int render_process_id,
      int render_frame_id,
      const GURL& url,
      const GURL& page_url,
      const std::string& mime_type,
      PluginProcessHost::Client* client,
      ResourceContext* resource_context);

  
  
  void FinishOpenChannelToPlugin(int render_process_id,
                                 const base::FilePath& plugin_path,
                                 PluginProcessHost::Client* client);

#if defined(OS_POSIX) && !defined(OS_OPENBSD) && !defined(OS_ANDROID)
  
  static void RegisterFilePathWatcher(base::FilePathWatcher* watcher,
                                      const base::FilePath& path);
#endif

#if defined(OS_WIN)
  
  base::win::RegKey hkcu_key_;
  base::win::RegKey hklm_key_;
  scoped_ptr<base::WaitableEvent> hkcu_event_;
  scoped_ptr<base::WaitableEvent> hklm_event_;
  base::WaitableEventWatcher hkcu_watcher_;
  base::WaitableEventWatcher hklm_watcher_;
#endif

#if defined(OS_POSIX) && !defined(OS_OPENBSD) && !defined(OS_ANDROID)
  ScopedVector<base::FilePathWatcher> file_watchers_;
#endif

  std::vector<PepperPluginInfo> ppapi_plugins_;

  
  PluginServiceFilter* filter_;

  std::set<PluginProcessHost::Client*> pending_plugin_clients_;

  
  base::SequencedWorkerPool::SequenceToken plugin_list_token_;

#if defined(OS_POSIX)
  scoped_refptr<PluginLoaderPosix> plugin_loader_;
#endif

  
  std::map<base::FilePath, std::vector<base::Time> > crash_times_;

  DISALLOW_COPY_AND_ASSIGN(PluginServiceImpl);
};

}  

#endif  

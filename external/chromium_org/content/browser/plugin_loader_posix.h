// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_PLUGIN_LOADER_POSIX_H_
#define CONTENT_BROWSER_PLUGIN_LOADER_POSIX_H_

#include <deque>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "content/browser/plugin_service_impl.h"
#include "content/public/browser/utility_process_host_client.h"
#include "content/public/common/webplugininfo.h"
#include "ipc/ipc_sender.h"

namespace base {
class MessageLoopProxy;
}

namespace content {
class UtilityProcessHost;

class CONTENT_EXPORT PluginLoaderPosix
    : public NON_EXPORTED_BASE(UtilityProcessHostClient),
      public IPC::Sender {
 public:
  PluginLoaderPosix();

  
  void LoadPlugins(
      scoped_refptr<base::MessageLoopProxy> target_loop,
      const PluginService::GetPluginsCallback& callback);

  
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

 private:
  struct PendingCallback {
    PendingCallback(scoped_refptr<base::MessageLoopProxy> target_loop,
                    const PluginService::GetPluginsCallback& callback);
    ~PendingCallback();

    scoped_refptr<base::MessageLoopProxy> target_loop;
    PluginService::GetPluginsCallback callback;
  };

  virtual ~PluginLoaderPosix();

  
  void GetPluginsToLoad();

  
  virtual void LoadPluginsInternal();

  
  void OnPluginLoaded(uint32 index, const WebPluginInfo& plugin);
  void OnPluginLoadFailed(uint32 index, const base::FilePath& plugin_path);

  
  
  bool MaybeAddInternalPlugin(const base::FilePath& plugin_path);

  
  
  
  bool MaybeRunPendingCallbacks();

  
  base::WeakPtr<UtilityProcessHost> process_host_;

  
  
  std::vector<base::FilePath> canonical_list_;

  
  
  size_t next_load_index_;

  
  std::vector<WebPluginInfo> internal_plugins_;

  
  std::vector<WebPluginInfo> loaded_plugins_;

  
  
  std::deque<PendingCallback> callbacks_;

  
  base::TimeTicks load_start_time_;

  friend class MockPluginLoaderPosix;
  DISALLOW_COPY_AND_ASSIGN(PluginLoaderPosix);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_APPLICATIONS_APP_SHIM_HOST_MANAGER_MAC_H_
#define CHROME_BROWSER_WEB_APPLICATIONS_APP_SHIM_HOST_MANAGER_MAC_H_

#include "apps/app_shim/extension_app_shim_handler_mac.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/browser_thread.h"
#include "ipc/ipc_channel_factory.h"

namespace base {
class FilePath;
}

namespace test {
class AppShimHostManagerTestApi;
}

class AppShimHostManager
    : public IPC::ChannelFactory::Delegate,
      public base::RefCountedThreadSafe<
          AppShimHostManager, content::BrowserThread::DeleteOnUIThread> {
 public:
  AppShimHostManager();

  
  
  
  void Init();

  apps::ExtensionAppShimHandler* extension_app_shim_handler() {
    return &extension_app_shim_handler_;
  }

 private:
  friend class base::RefCountedThreadSafe<AppShimHostManager>;
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  friend class base::DeleteHelper<AppShimHostManager>;
  friend class test::AppShimHostManagerTestApi;
  virtual ~AppShimHostManager();

  
  virtual void OnClientConnected(const IPC::ChannelHandle& handle) OVERRIDE;
  virtual void OnListenError() OVERRIDE;

  
  
  
  void InitOnFileThread();

  
  void ListenOnIOThread();

  
  static const base::FilePath* g_override_user_data_dir_;

  scoped_ptr<IPC::ChannelFactory> factory_;

  apps::ExtensionAppShimHandler extension_app_shim_handler_;

  DISALLOW_COPY_AND_ASSIGN(AppShimHostManager);
};

#endif  

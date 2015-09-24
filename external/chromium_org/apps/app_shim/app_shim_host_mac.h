// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEB_APPLICATIONS_APP_SHIM_HOST_MAC_H_
#define CHROME_BROWSER_WEB_APPLICATIONS_APP_SHIM_HOST_MAC_H_

#include <string>
#include <vector>

#include "apps/app_shim/app_shim_handler_mac.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"

namespace IPC {
struct ChannelHandle;
class ChannelProxy;
class Message;
}  

class AppShimHost : public IPC::Listener,
                    public IPC::Sender,
                    public apps::AppShimHandler::Host,
                    public base::NonThreadSafe {
 public:
  AppShimHost();
  virtual ~AppShimHost();

  
  
  
  void ServeChannel(const IPC::ChannelHandle& handle);

 protected:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

 private:
  
  
  
  
  void OnLaunchApp(const base::FilePath& profile_dir,
                   const std::string& app_id,
                   apps::AppShimLaunchType launch_type,
                   const std::vector<base::FilePath>& files);

  
  void OnFocus(apps::AppShimFocusType focus_type,
               const std::vector<base::FilePath>& files);

  void OnSetHidden(bool hidden);

  
  void OnQuit();

  
  virtual void OnAppLaunchComplete(apps::AppShimLaunchResult result) OVERRIDE;
  virtual void OnAppClosed() OVERRIDE;
  virtual void OnAppHide() OVERRIDE;
  virtual void OnAppRequestUserAttention() OVERRIDE;
  virtual base::FilePath GetProfilePath() const OVERRIDE;
  virtual std::string GetAppId() const OVERRIDE;

  
  void Close();

  scoped_ptr<IPC::ChannelProxy> channel_;
  std::string app_id_;
  base::FilePath profile_path_;
  bool initial_launch_finished_;
};

#endif  

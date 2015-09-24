// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_APP_SHIM_APP_SHIM_HANDLER_MAC_H_
#define APPS_APP_SHIM_APP_SHIM_HANDLER_MAC_H_

#include <string>
#include <vector>

#include "apps/app_shim/app_shim_launch.h"
#include "base/files/file_path.h"

class Profile;

namespace apps {

class AppShimHandler {
 public:
  class Host {
   public:
    
    virtual void OnAppLaunchComplete(AppShimLaunchResult result) = 0;
    
    virtual void OnAppClosed() = 0;
    
    virtual void OnAppHide() = 0;
    
    virtual void OnAppRequestUserAttention() = 0;

    
    virtual base::FilePath GetProfilePath() const = 0;
    virtual std::string GetAppId() const = 0;

   protected:
    virtual ~Host() {}
  };

  
  static void RegisterHandler(const std::string& app_mode_id,
                              AppShimHandler* handler);

  
  static void RemoveHandler(const std::string& app_mode_id);

  
  
  
  static AppShimHandler* GetForAppMode(const std::string& app_mode_id);

  
  
  static void SetDefaultHandler(AppShimHandler* handler);

  
  
  static void MaybeTerminate();

  
  
  
  
  
  virtual void OnShimLaunch(Host* host,
                            AppShimLaunchType launch_type,
                            const std::vector<base::FilePath>& files) = 0;

  
  virtual void OnShimClose(Host* host) = 0;

  
  
  
  virtual void OnShimFocus(Host* host,
                           AppShimFocusType focus_type,
                           const std::vector<base::FilePath>& files) = 0;

  
  virtual void OnShimSetHidden(Host* host, bool hidden) = 0;

  
  virtual void OnShimQuit(Host* host) = 0;

 protected:
  AppShimHandler() {}
  virtual ~AppShimHandler() {}
};

}  

#endif  

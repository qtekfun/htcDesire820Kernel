// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef APPS_APP_LOAD_SERVICE_H_
#define APPS_APP_LOAD_SERVICE_H_

#include <map>
#include <string>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace extensions {
struct UnloadedExtensionInfo;
}

namespace apps {

class AppLoadService : public BrowserContextKeyedService,
                       public content::NotificationObserver {
 public:
  enum PostReloadActionType {
    LAUNCH,
    RESTART,
    LAUNCH_WITH_COMMAND_LINE,
  };

  struct PostReloadAction {
    PostReloadAction();

    PostReloadActionType action_type;
    CommandLine command_line;
    base::FilePath current_dir;
  };

  explicit AppLoadService(Profile* profile);
  virtual ~AppLoadService();

  
  
  void RestartApplication(const std::string& extension_id);

  
  
  
  
  bool LoadAndLaunch(const base::FilePath& extension_path,
                     const CommandLine& command_line,
                     const base::FilePath& current_dir);

  static AppLoadService* Get(Profile* profile);

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  bool HasShellWindows(const std::string& extension_id);
  bool WasUnloadedForReload(
      const extensions::UnloadedExtensionInfo& unload_info);
  bool HasPostReloadAction(const std::string& extension_id);

  
  
  std::map<std::string, PostReloadAction> post_reload_actions_;
  content::NotificationRegistrar registrar_;
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(AppLoadService);
};

}  

#endif  

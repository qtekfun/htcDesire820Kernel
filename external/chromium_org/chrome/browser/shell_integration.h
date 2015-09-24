// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SHELL_INTEGRATION_H_
#define CHROME_BROWSER_SHELL_INTEGRATION_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "ui/gfx/image/image_family.h"
#include "url/gurl.h"

class CommandLine;

class ShellIntegration {
 public:
  
  
  static bool SetAsDefaultBrowser();

  
  
  
  
  
  
  static bool SetAsDefaultBrowserInteractive();

  
  
  static bool SetAsDefaultProtocolClient(const std::string& protocol);

  
  
  
  
  
  
  static bool SetAsDefaultProtocolClientInteractive(
      const std::string& protocol);

  
  
  
  enum DefaultWebClientSetPermission {
    SET_DEFAULT_NOT_ALLOWED,
    SET_DEFAULT_UNATTENDED,
    SET_DEFAULT_INTERACTIVE,
  };

  
  static DefaultWebClientSetPermission CanSetAsDefaultBrowser();

  
  
  static DefaultWebClientSetPermission CanSetAsDefaultProtocolClient();

  
  
  static std::string GetApplicationForProtocol(const GURL& url);

  
  
  
  enum DefaultWebClientState {
    NOT_DEFAULT,
    IS_DEFAULT,
    UNKNOWN_DEFAULT,
    NUM_DEFAULT_STATES
  };

  
  
  
  
  static DefaultWebClientState GetDefaultBrowser();

  
  
  static bool IsFirefoxDefaultBrowser();

  
  
  static DefaultWebClientState
      IsDefaultProtocolClient(const std::string& protocol);

  struct ShortcutInfo {
    ShortcutInfo();
    ~ShortcutInfo();

    GURL url;
    
    
    
    std::string extension_id;
    bool is_platform_app;
    base::string16 title;
    base::string16 description;
    base::FilePath extension_path;
    gfx::ImageFamily favicon;
    base::FilePath profile_path;
    std::string profile_name;
  };

  
  
  
  
  
  
  
  
  enum ApplicationsMenuLocation {
    APP_MENU_LOCATION_NONE,
    APP_MENU_LOCATION_ROOT,
    APP_MENU_LOCATION_SUBDIR_CHROME,
    APP_MENU_LOCATION_SUBDIR_CHROMEAPPS,
  };

  
  struct ShortcutLocations {
    ShortcutLocations();

    bool on_desktop;

    ApplicationsMenuLocation applications_menu_location;

    
    
    
    
    bool in_quick_launch_bar;

#if defined(OS_POSIX)
    
    
    
    
    bool hidden;
#endif
  };

  
  struct AppModeInfo {
  };
  static void SetAppModeInfo(const AppModeInfo* info);
  static const AppModeInfo* AppModeInfo();

  
  bool IsRunningInAppMode();

  
  
  
  
  
  static CommandLine CommandLineArgsForLauncher(
      const GURL& url,
      const std::string& extension_app_id,
      const base::FilePath& profile_path);

#if defined(OS_WIN)
  
  
  
  
  
  
  
  static base::string16 GetAppModelIdForProfile(const base::string16& app_name,
                                          const base::FilePath& profile_path);

  
  
  static base::string16 GetChromiumModelIdForProfile(
      const base::FilePath& profile_path);

  
  static base::string16 GetAppListAppModelIdForProfile(
      const base::FilePath& profile_path);

  
  
  static void MigrateChromiumShortcuts();

  
  
  
  
  
  
  
  static int MigrateShortcutsInPathInternal(const base::FilePath& chrome_exe,
                                            const base::FilePath& path,
                                            bool check_dual_mode);

  
  static base::FilePath GetStartMenuShortcut(const base::FilePath& chrome_exe);
#endif  

#if !defined(OS_WIN)
  
  
  
  
  
  static base::string16 GetAppShortcutsSubdirName();
#endif

  
  
  
  
  enum DefaultWebClientUIState {
    STATE_PROCESSING,
    STATE_NOT_DEFAULT,
    STATE_IS_DEFAULT,
    STATE_UNKNOWN
  };

  class DefaultWebClientObserver {
   public:
    virtual ~DefaultWebClientObserver() {}
    
    virtual void SetDefaultWebClientUIState(DefaultWebClientUIState state) = 0;
    
    
    virtual void OnSetAsDefaultConcluded(bool succeeded) {}
    
    
    virtual bool IsOwnedByWorker();
    
    
    virtual bool IsInteractiveSetDefaultPermitted();
  };

  
  
  
  
  
  class DefaultWebClientWorker
      : public base::RefCountedThreadSafe<DefaultWebClientWorker> {
   public:
    explicit DefaultWebClientWorker(DefaultWebClientObserver* observer);

    
    
    
    void StartCheckIsDefault();

    
    
    
    void StartSetAsDefault();

    
    void ObserverDestroyed();

   protected:
    friend class base::RefCountedThreadSafe<DefaultWebClientWorker>;

    virtual ~DefaultWebClientWorker() {}

   private:
    
    virtual DefaultWebClientState CheckIsDefault() = 0;

    
    
    virtual bool SetAsDefault(bool interactive_permitted) = 0;

    
    
    
    
    
    void ExecuteCheckIsDefault();

    
    
    
    
    
    
    
    
    void ExecuteSetAsDefault(bool interactive_permitted);

    
    
    
    void CompleteCheckIsDefault(DefaultWebClientState state);

    
    
    
    
    
    
    
    void CompleteSetAsDefault(bool succeeded);

    
    
    void UpdateUI(DefaultWebClientState state);

    DefaultWebClientObserver* observer_;

    DISALLOW_COPY_AND_ASSIGN(DefaultWebClientWorker);
  };

  
  class DefaultBrowserWorker : public DefaultWebClientWorker {
   public:
    explicit DefaultBrowserWorker(DefaultWebClientObserver* observer);

   private:
    virtual ~DefaultBrowserWorker() {}

    
    virtual DefaultWebClientState CheckIsDefault() OVERRIDE;

    
    virtual bool SetAsDefault(bool interactive_permitted) OVERRIDE;

    DISALLOW_COPY_AND_ASSIGN(DefaultBrowserWorker);
  };

  
  
  
  
  class DefaultProtocolClientWorker : public DefaultWebClientWorker {
   public:
    DefaultProtocolClientWorker(DefaultWebClientObserver* observer,
                                const std::string& protocol);

    const std::string& protocol() const { return protocol_; }

   protected:
    virtual ~DefaultProtocolClientWorker() {}

   private:
    
    virtual DefaultWebClientState CheckIsDefault() OVERRIDE;

    
    virtual bool SetAsDefault(bool interactive_permitted) OVERRIDE;

    std::string protocol_;

    DISALLOW_COPY_AND_ASSIGN(DefaultProtocolClientWorker);
  };
};

#endif  

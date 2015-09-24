// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_SHELL_UTIL_H_
#define CHROME_INSTALLER_UTIL_SHELL_UTIL_H_

#include <windows.h>

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/strings/string16.h"
#include "chrome/installer/util/work_item_list.h"

class BrowserDistribution;

class ShellUtil {
 public:
  
  enum ShellChange {
    CURRENT_USER = 0x1,  
    SYSTEM_LEVEL = 0x2   
  };

  
  enum DefaultState {
    UNKNOWN_DEFAULT,
    NOT_DEFAULT,
    IS_DEFAULT,
  };

  
  
  enum ShortcutLocation {
    SHORTCUT_LOCATION_FIRST = 0,
    SHORTCUT_LOCATION_DESKTOP = SHORTCUT_LOCATION_FIRST,
    SHORTCUT_LOCATION_QUICK_LAUNCH,
    SHORTCUT_LOCATION_START_MENU_ROOT,
    SHORTCUT_LOCATION_START_MENU_CHROME_DIR,
    SHORTCUT_LOCATION_START_MENU_CHROME_APPS_DIR,
    SHORTCUT_LOCATION_TASKBAR_PINS,  
    SHORTCUT_LOCATION_APP_SHORTCUTS,  
    NUM_SHORTCUT_LOCATIONS
  };

  enum ShortcutOperation {
    
    SHELL_SHORTCUT_CREATE_ALWAYS,
    
    
    SHELL_SHORTCUT_CREATE_IF_NO_SYSTEM_LEVEL,
    
    
    
    SHELL_SHORTCUT_REPLACE_EXISTING,
    
    SHELL_SHORTCUT_UPDATE_EXISTING,
  };

  
  
  
  
  
  
  struct ShortcutProperties {
    enum IndividualProperties {
      PROPERTIES_TARGET = 1 << 0,
      PROPERTIES_ARGUMENTS = 1 << 1,
      PROPERTIES_DESCRIPTION = 1 << 2,
      PROPERTIES_ICON = 1 << 3,
      PROPERTIES_APP_ID = 1 << 4,
      PROPERTIES_SHORTCUT_NAME = 1 << 5,
      PROPERTIES_DUAL_MODE = 1 << 6,
    };

    explicit ShortcutProperties(ShellChange level_in)
        : level(level_in), icon_index(0), dual_mode(false),
          pin_to_taskbar(false), options(0U) {}

    
    
    void set_target(const base::FilePath& target_in) {
      target = target_in;
      options |= PROPERTIES_TARGET;
    }

    
    
    
    void set_arguments(const string16& arguments_in) {
      
      
      DCHECK(arguments_in.length() < MAX_PATH);
      arguments = arguments_in;
      options |= PROPERTIES_ARGUMENTS;
    }

    
    
    void set_description(const string16& description_in) {
      
      
      DCHECK(description_in.length() < MAX_PATH);
      description = description_in;
      options |= PROPERTIES_DESCRIPTION;
    }

    
    
    void set_icon(const base::FilePath& icon_in, int icon_index_in) {
      icon = icon_in;
      icon_index = icon_index_in;
      options |= PROPERTIES_ICON;
    }

    
    void set_app_id(const string16& app_id_in) {
      app_id = app_id_in;
      options |= PROPERTIES_APP_ID;
    }

    
    
    
    void set_shortcut_name(const string16& shortcut_name_in) {
      shortcut_name = shortcut_name_in;
      options |= PROPERTIES_SHORTCUT_NAME;
    }

    
    
    
    
    void set_dual_mode(bool dual_mode_in) {
      dual_mode = dual_mode_in;
      options |= PROPERTIES_DUAL_MODE;
    }

    
    
    
    void set_pin_to_taskbar(bool pin_to_taskbar_in) {
      pin_to_taskbar = pin_to_taskbar_in;
    }

    bool has_target() const {
      return (options & PROPERTIES_TARGET) != 0;
    }

    bool has_arguments() const {
      return (options & PROPERTIES_ARGUMENTS) != 0;
    }

    bool has_description() const {
      return (options & PROPERTIES_DESCRIPTION) != 0;
    }

    bool has_icon() const {
      return (options & PROPERTIES_ICON) != 0;
    }

    bool has_app_id() const {
      return (options & PROPERTIES_APP_ID) != 0;
    }

    bool has_shortcut_name() const {
      return (options & PROPERTIES_SHORTCUT_NAME) != 0;
    }

    bool has_dual_mode() const {
      return (options & PROPERTIES_DUAL_MODE) != 0;
    }

    
    
    ShellChange level;

    base::FilePath target;
    string16 arguments;
    string16 description;
    base::FilePath icon;
    int icon_index;
    string16 app_id;
    string16 shortcut_name;
    bool dual_mode;
    bool pin_to_taskbar;
    
    
    
    
    uint32 options;
  };

  
  static const wchar_t* kRegURLProtocol;

  
  static const wchar_t* kRegDefaultIcon;

  
  static const wchar_t* kRegShellPath;

  
  
  static const wchar_t* kRegShellOpen;

  
  
  static const wchar_t* kRegStartMenuInternet;

  
  
  static const wchar_t* kRegClasses;

  
  
  static const wchar_t* kRegRegisteredApplications;

  
  
  static const wchar_t* kAppPathsRegistryKey;
  static const wchar_t* kAppPathsRegistryPathName;

  
  static const wchar_t* kRegVistaUrlPrefs;

  
  
  static const wchar_t* kDefaultFileAssociations[];

  
  
  static const wchar_t* kPotentialFileAssociations[];

  
  
  static const wchar_t* kBrowserProtocolAssociations[];

  
  static const wchar_t* kPotentialProtocolAssociations[];

  
  static const wchar_t* kRegUrlProtocol;

  
  
  static const wchar_t* kRegApplication;

  
  static const wchar_t* kRegAppUserModelId;

  
  static const wchar_t* kRegApplicationDescription;

  
  static const wchar_t* kRegApplicationName;

  
  static const wchar_t* kRegApplicationIcon;

  
  static const wchar_t* kRegApplicationCompany;

  
  static const wchar_t* kRegExePath;

  
  static const wchar_t* kRegVerbOpen;

  
  static const wchar_t* kRegVerbOpenNewWindow;

  
  static const wchar_t* kRegVerbRun;

  
  static const wchar_t* kRegCommand;

  
  static const wchar_t* kRegDelegateExecute;

  
  static const wchar_t* kRegOpenWithProgids;

  
  
  
  static bool QuickIsChromeRegisteredInHKLM(BrowserDistribution* dist,
                                            const string16& chrome_exe,
                                            const string16& suffix);

  
  
  static bool ShortcutLocationIsSupported(ShellUtil::ShortcutLocation location);

  
  
  
  
  static bool GetShortcutPath(ShellUtil::ShortcutLocation location,
                              BrowserDistribution* dist,
                              ShellChange level,
                              base::FilePath* path);

  
  
  
  
  
  
  
  
  
  static bool CreateOrUpdateShortcut(
      ShellUtil::ShortcutLocation location,
      BrowserDistribution* dist,
      const ShellUtil::ShortcutProperties& properties,
      ShellUtil::ShortcutOperation operation);

  
  
  static string16 FormatIconLocation(const string16& icon_path, int icon_index);

  
  // this command is written to the registry under shell\open\command key.
  
  static string16 GetChromeShellOpenCmd(const string16& chrome_exe);

  
  // handler to launch chrome on Windows 8. Typically this command is written to
  
  
  static string16 GetChromeDelegateCommand(const string16& chrome_exe);

  
  
  
  
  
  
  static void GetRegisteredBrowsers(BrowserDistribution* dist,
                                    std::map<string16, string16>* browsers);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static string16 GetCurrentInstallationSuffix(BrowserDistribution* dist,
                                               const string16& chrome_exe);

  
  
  
  
  
  static string16 GetApplicationName(BrowserDistribution* dist,
                                     const string16& chrome_exe);

  
  
  
  
  static string16 GetBrowserModelId(BrowserDistribution* dist,
                                    bool is_per_user_install);

  
  
  
  
  
  static string16 BuildAppModelId(const std::vector<string16>& components);

  
  
  
  static bool CanMakeChromeDefaultUnattended();

  
  static DefaultState GetChromeDefaultState();

  
  static DefaultState GetChromeDefaultProtocolClientState(
      const string16& protocol);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool MakeChromeDefault(BrowserDistribution* dist,
                                int shell_change,
                                const string16& chrome_exe,
                                bool elevate_if_not_admin);

  
  
  
  
  
  
  
  static bool ShowMakeChromeDefaultSystemUI(BrowserDistribution* dist,
                                            const string16& chrome_exe);

  
  
  
  static bool MakeChromeDefaultProtocolClient(BrowserDistribution* dist,
                                              const string16& chrome_exe,
                                              const string16& protocol);

  
  
  
  
  
  
  
  
  static bool ShowMakeChromeDefaultProtocolClientSystemUI(
      BrowserDistribution* dist,
      const string16& chrome_exe,
      const string16& protocol);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool RegisterChromeBrowser(BrowserDistribution* dist,
                                    const string16& chrome_exe,
                                    const string16& unique_suffix,
                                    bool elevate_if_not_admin);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool RegisterChromeForProtocol(BrowserDistribution* dist,
                                        const string16& chrome_exe,
                                        const string16& unique_suffix,
                                        const string16& protocol,
                                        bool elevate_if_not_admin);

  
  
  
  
  
  
  
  
  static bool RemoveShortcuts(ShellUtil::ShortcutLocation location,
                              BrowserDistribution* dist,
                              ShellChange level,
                              const base::FilePath& target_exe);

  
  
  
  
  
  
  static bool UpdateShortcutsWithArgs(
      ShellUtil::ShortcutLocation location,
      BrowserDistribution* dist,
      ShellChange level,
      const base::FilePath& target_exe,
      const ShellUtil::ShortcutProperties& properties);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool GetUserSpecificRegistrySuffix(string16* suffix);

  
  
  
  
  
  
  static bool GetOldUserSpecificRegistrySuffix(string16* suffix);

  
  
  
  
  
  static string16 ByteArrayToBase32(const uint8* bytes, size_t size);

 private:
  DISALLOW_COPY_AND_ASSIGN(ShellUtil);
};


#endif  

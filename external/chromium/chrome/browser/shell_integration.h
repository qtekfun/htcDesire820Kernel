// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SHELL_INTEGRATION_H_
#define CHROME_BROWSER_SHELL_INTEGRATION_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "googleurl/src/gurl.h"
#include "third_party/skia/include/core/SkBitmap.h"

class CommandLine;
class FilePath;
class PrefService;

#if defined(USE_X11)
namespace base {
class Environment;
}
#endif

class ShellIntegration {
 public:
  
  
  static bool SetAsDefaultBrowser();

  
  
  
  
  enum DefaultBrowserState {
    NOT_DEFAULT_BROWSER = 0,
    IS_DEFAULT_BROWSER,
    UNKNOWN_DEFAULT_BROWSER
  };

  
  
  
  
  static DefaultBrowserState IsDefaultBrowser();

  
  
  static bool IsFirefoxDefaultBrowser();

  struct ShortcutInfo {
    ShortcutInfo();
    ~ShortcutInfo();

    GURL url;
    
    
    
    std::string extension_id;
    string16 title;
    string16 description;
    SkBitmap favicon;

    bool create_on_desktop;
    bool create_in_applications_menu;

    
    
    
    
    bool create_in_quick_launch_bar;
  };

  
  
  
  
  
  static CommandLine CommandLineArgsForLauncher(
      const GURL& url,
      const std::string& extension_app_id);

#if defined(USE_X11)
  
  static std::string GetDesktopName(base::Environment* env);

  static bool GetDesktopShortcutTemplate(base::Environment* env,
                                         std::string* output);

  
  static FilePath GetDesktopShortcutFilename(const GURL& url);

  
  
  
  static std::string GetDesktopFileContents(
      const std::string& template_contents,
      const std::string& app_name,
      const GURL& url,
      const std::string& extension_id,
      const string16& title,
      const std::string& icon_name);

  static void CreateDesktopShortcut(const ShortcutInfo& shortcut_info,
                                    const std::string& shortcut_template);
#endif  

#if defined(OS_WIN)
  
  
  
  static std::wstring GetAppId(const std::wstring& app_name,
                               const FilePath& profile_path);

  
  
  static std::wstring GetChromiumAppId(const FilePath& profile_path);

  
  
  static void MigrateChromiumShortcuts();
#endif  

  
  enum DefaultBrowserUIState {
    STATE_PROCESSING,
    STATE_NOT_DEFAULT,
    STATE_IS_DEFAULT,
    STATE_UNKNOWN
  };

  class DefaultBrowserObserver {
   public:
    
    virtual void SetDefaultBrowserUIState(DefaultBrowserUIState state) = 0;
    virtual ~DefaultBrowserObserver() {}
  };
  
  
  
  
  
  class DefaultBrowserWorker
      : public base::RefCountedThreadSafe<DefaultBrowserWorker> {
   public:
    explicit DefaultBrowserWorker(DefaultBrowserObserver* observer);

    
    void StartCheckDefaultBrowser();

    
    void StartSetAsDefaultBrowser();

    
    void ObserverDestroyed();

   private:
    friend class base::RefCountedThreadSafe<DefaultBrowserWorker>;

    virtual ~DefaultBrowserWorker() {}

    
    
    
    
    void ExecuteCheckDefaultBrowser();
    void CompleteCheckDefaultBrowser(DefaultBrowserState state);

    
    
    
    
    void ExecuteSetAsDefaultBrowser();
    void CompleteSetAsDefaultBrowser();

    
    
    void UpdateUI(DefaultBrowserState state);

    DefaultBrowserObserver* observer_;

    DISALLOW_COPY_AND_ASSIGN(DefaultBrowserWorker);
  };
};

#endif  

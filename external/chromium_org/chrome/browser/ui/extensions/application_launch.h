// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_EXTENSIONS_APPLICATION_LAUNCH_H_
#define CHROME_BROWSER_UI_EXTENSIONS_APPLICATION_LAUNCH_H_

#include "base/files/file_path.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/common/extensions/extension_constants.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

class Browser;
class CommandLine;
class Profile;

namespace content {
class WebContents;
}

namespace extensions {
class Extension;
}

struct AppLaunchParams {
  AppLaunchParams(Profile* profile,
                  const extensions::Extension* extension,
                  extensions::LaunchContainer container,
                  WindowOpenDisposition disposition);

  
  
  AppLaunchParams(Profile* profile,
                  const extensions::Extension* extension,
                  WindowOpenDisposition disposition);

  
  
  
  
  AppLaunchParams(Profile* profile,
                  const extensions::Extension* extension,
                  int event_flags,
                  chrome::HostDesktopType desktop_type);

  
  Profile* profile;

  
  const extensions::Extension* extension;

  
  extensions::LaunchContainer container;

  
  WindowOpenDisposition disposition;

  
  chrome::HostDesktopType desktop_type;

  
  GURL override_url;

  
  
  gfx::Rect override_bounds;

  
  
  const CommandLine* command_line;

  
  
  base::FilePath current_directory;
};

void OpenApplicationWithReenablePrompt(const AppLaunchParams& params);

content::WebContents* OpenApplication(const AppLaunchParams& params);

content::WebContents* OpenAppShortcutWindow(Profile* profile,
                                            const GURL& url,
                                            const gfx::Rect& override_bounds);

#endif  

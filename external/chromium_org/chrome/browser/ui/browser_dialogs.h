// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_DIALOGS_H_
#define CHROME_BROWSER_UI_BROWSER_DIALOGS_H_

#include "base/callback.h"
#include "ipc/ipc_message.h"  
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/native_widget_types.h"

class Browser;
class Profile;
class SkBitmap;
class TabModalConfirmDialogDelegate;

namespace content {
class BrowserContext;
class ColorChooser;
class WebContents;
}

namespace extensions {
class Extension;
}

namespace ui {
class ProfileSigninConfirmationDelegate;
class WebDialogDelegate;
}

namespace chrome {

#if defined(IPC_MESSAGE_LOG_ENABLED)

void ShowAboutIPCDialog();

#endif  

gfx::NativeWindow ShowWebDialog(gfx::NativeWindow parent,
                                content::BrowserContext* context,
                                ui::WebDialogDelegate* delegate);

void ShowCollectedCookiesDialog(content::WebContents* web_contents);

void ShowExtensionInstalledBubble(const extensions::Extension* extension,
                                  Browser* browser,
                                  const SkBitmap& icon);

void ShowHungRendererDialog(content::WebContents* contents);
void HideHungRendererDialog(content::WebContents* contents);

void ShowTaskManager(Browser* browser);

#if !defined(OS_MACOSX)
void ShowCreateWebAppShortcutsDialog(gfx::NativeWindow parent_window,
                                     content::WebContents* web_contents);
#endif

void ShowCreateChromeAppShortcutsDialog(gfx::NativeWindow parent_window,
                                        Profile* profile,
                                        const extensions::Extension* app,
                                        const base::Closure& close_callback);

content::ColorChooser* ShowColorChooser(content::WebContents* web_contents,
                                        SkColor initial_color);

void ShowProfileSigninConfirmationDialog(
    Browser* browser,
    content::WebContents* web_contents,
    Profile* profile,
    const std::string& username,
    ui::ProfileSigninConfirmationDelegate* delegate);


void ShowUserManager(const base::FilePath& profile_path_to_focus);

void HideUserManager();

}  

#endif  

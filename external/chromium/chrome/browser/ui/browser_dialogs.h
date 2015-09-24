// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_DIALOGS_H_
#define CHROME_BROWSER_UI_BROWSER_DIALOGS_H_
#pragma once

#include "ipc/ipc_message.h"
#include "ui/gfx/native_widget_types.h"

class Browser;
class Extension;
class HtmlDialogUIDelegate;
class Profile;
class SkBitmap;
class TabContents;

namespace browser {

#if defined(IPC_MESSAGE_LOG_ENABLED)

void ShowAboutIPCDialog();

#endif  

gfx::NativeWindow ShowHtmlDialog(gfx::NativeWindow parent, Profile* profile,
                                 HtmlDialogUIDelegate* delegate);

void ShowExtensionInstalledBubble(const Extension* extension,
                                  Browser* browser,
                                  const SkBitmap& icon,
                                  Profile* profile);

void ShowHungRendererDialog(TabContents* contents);
void HideHungRendererDialog(TabContents* contents);

} 

#endif  

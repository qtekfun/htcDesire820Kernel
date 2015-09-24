// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_BROWSER_DIALOGS_H_
#define CHROME_BROWSER_UI_VIEWS_BROWSER_DIALOGS_H_
#pragma once

#include <string>

#include "ui/gfx/native_widget_types.h"


class BrowserView;
class EditSearchEngineControllerDelegate;
class Extension;
class FindBar;
class GURL;
class BubbleDelegate;
class Profile;
class TabContents;
class TabContentsWrapper;
class TemplateURL;

namespace gfx {
class Rect;
class Size;
}

namespace views {
class Window;
}

namespace browser {

void ShowBookmarkBubbleView(views::Window* parent,
                            const gfx::Rect& bounds,
                            BubbleDelegate* delegate,
                            Profile* profile,
                            const GURL& url,
                            bool newly_bookmarked);
void HideBookmarkBubbleView();
bool IsBookmarkBubbleViewShowing();

views::Window* ShowAboutChromeView(gfx::NativeWindow parent,
                                   Profile* profile);

FindBar* CreateFindBar(BrowserView* browser_view);

void ShowTaskManager();

void ShowBackgroundPages();

#if defined(OS_CHROMEOS)
void ShowLoginWizard(const std::string& start_screen, const gfx::Size& size);
#endif

void EditSearchEngine(gfx::NativeWindow parent,
                      const TemplateURL* template_url,
                      EditSearchEngineControllerDelegate* delegate,
                      Profile* profile);

void ShowRepostFormWarningDialog(gfx::NativeWindow parent_window,
                                 TabContents* tab_contents);

void ShowCollectedCookiesDialog(gfx::NativeWindow parent_window,
                                TabContents* tab_contents);


void ShowCreateWebAppShortcutsDialog(gfx::NativeWindow parent_window,
                                     TabContentsWrapper* tab_contents);

void ShowCreateChromeAppShortcutsDialog(gfx::NativeWindow parent_window,
                                        Profile* profile,
                                        const Extension* app);

}  

#endif  

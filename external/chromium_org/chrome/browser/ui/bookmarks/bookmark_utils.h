// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_UTILS_H_
#define CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_UTILS_H_

#include <vector>

#include "base/strings/string16.h"
#include "chrome/browser/ui/host_desktop.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/native_widget_types.h"

class BookmarkNode;
class Browser;
class GURL;
class PrefService;
class Profile;

namespace content {
class BrowserContext;
class PageNavigator;
class WebContents;
}

namespace chrome {

extern int num_bookmark_urls_before_prompting;

void OpenAll(gfx::NativeWindow parent,
             content::PageNavigator* navigator,
             const std::vector<const BookmarkNode*>& nodes,
             WindowOpenDisposition initial_disposition,
             content::BrowserContext* browser_context);

void OpenAll(gfx::NativeWindow parent,
             content::PageNavigator* navigator,
             const BookmarkNode* node,
             WindowOpenDisposition initial_disposition,
             content::BrowserContext* browser_context);

bool ConfirmDeleteBookmarkNode(const BookmarkNode* node,
                               gfx::NativeWindow window);

void ShowBookmarkAllTabsDialog(Browser* browser);

bool HasBookmarkURLs(const std::vector<const BookmarkNode*>& selection);

bool HasBookmarkURLsAllowedInIncognitoMode(
    const std::vector<const BookmarkNode*>& selection,
    content::BrowserContext* browser_context);

GURL GetURLToBookmark(content::WebContents* web_contents);

void GetURLAndTitleToBookmark(content::WebContents* web_contents,
                              GURL* url,
                              base::string16* title);

void ToggleBookmarkBarWhenVisible(content::BrowserContext* browser_context);

base::string16 FormatBookmarkURLForDisplay(const GURL& url,
                                           const PrefService* prefs);

bool IsAppsShortcutEnabled(Profile* profile,
                           chrome::HostDesktopType host_desktop_type);

bool ShouldShowAppsShortcutInBookmarkBar(
    Profile* profile,
    chrome::HostDesktopType host_desktop_type);

}  

#endif  

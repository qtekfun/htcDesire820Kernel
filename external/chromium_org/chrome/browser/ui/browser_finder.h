// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_FINDER_H_
#define CHROME_BROWSER_UI_BROWSER_FINDER_H_

#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/host_desktop.h"
#include "ui/gfx/native_widget_types.h"

class Profile;

namespace contents {
class WebContents;
}


namespace chrome {


Browser* FindTabbedBrowser(Profile* profile,
                           bool match_original_profiles,
                           HostDesktopType type);

Browser* FindAnyBrowser(Profile* profile,
                        bool match_original_profiles,
                        HostDesktopType type);

Browser* FindBrowserWithProfile(Profile* profile, HostDesktopType type);

Browser* FindBrowserWithID(SessionID::id_type desired_id);

Browser* FindBrowserWithWindow(gfx::NativeWindow window);

Browser* FindBrowserWithWebContents(const content::WebContents* web_contents);

Browser* FindLastActiveWithProfile(Profile* profile, HostDesktopType type);

Browser* FindLastActiveWithHostDesktopType(HostDesktopType type);

size_t GetTotalBrowserCount();

size_t GetTotalBrowserCountForProfile(Profile* profile);

size_t GetBrowserCount(Profile* profile, HostDesktopType type);

size_t GetTabbedBrowserCount(Profile* profile, HostDesktopType type);

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_RESTORE_H_
#define CHROME_BROWSER_SESSIONS_SESSION_RESTORE_H_

#include <vector>

#include "base/basictypes.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/ui/host_desktop.h"
#include "ui/base/window_open_disposition.h"

class Browser;
class Profile;

namespace content {
class WebContents;
}

class SessionRestore {
 public:
  enum Behavior {
    
    CLOBBER_CURRENT_TAB          = 1 << 0,

    
    
    ALWAYS_CREATE_TABBED_BROWSER = 1 << 1,

    
    
    SYNCHRONOUS                  = 1 << 2,
  };

  
  
  
  
  
  
  
  
  static Browser* RestoreSession(Profile* profile,
                                 Browser* browser,
                                 chrome::HostDesktopType host_desktop_type,
                                 uint32 behavior,
                                 const std::vector<GURL>& urls_to_open);

  
  
  
  
  static std::vector<Browser*> RestoreForeignSessionWindows(
      Profile* profile,
      chrome::HostDesktopType host_desktop_type,
      std::vector<const SessionWindow*>::const_iterator begin,
      std::vector<const SessionWindow*>::const_iterator end);

  
  
  
  
  
  static content::WebContents* RestoreForeignSessionTab(
      content::WebContents* source_web_contents,
      const SessionTab& tab,
      WindowOpenDisposition disposition);

  
  static bool IsRestoring(const Profile* profile);

  
  static bool IsRestoringSynchronously();

  
  
  static size_t num_tabs_to_load_;

 private:
  SessionRestore();

  DISALLOW_COPY_AND_ASSIGN(SessionRestore);
};

#endif  

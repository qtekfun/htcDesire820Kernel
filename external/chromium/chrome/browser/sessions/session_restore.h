// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_RESTORE_H_
#define CHROME_BROWSER_SESSIONS_SESSION_RESTORE_H_
#pragma once

#include <vector>

#include "chrome/browser/history/history.h"
#include "chrome/browser/sessions/session_types.h"
#include "base/basictypes.h"

class Browser;
class Profile;

class SessionRestore {
 public:
  
  
  
  
  
  
  
  
  
  
  static void RestoreSession(Profile* profile,
                             Browser* browser,
                             bool clobber_existing_window,
                             bool always_create_tabbed_browser,
                             const std::vector<GURL>& urls_to_open);

  
  
  static void RestoreForeignSessionWindows(
      Profile* profile,
      std::vector<SessionWindow*>::const_iterator begin,
      std::vector<SessionWindow*>::const_iterator end);

  
  
  static void RestoreForeignSessionTab(Profile* profile,
      const SessionTab& tab);

  
  
  
  
  
  
  
  static Browser* RestoreSessionSynchronously(
      Profile* profile,
      const std::vector<GURL>& urls_to_open);

  
  static bool IsRestoring();

  
  
  static size_t num_tabs_to_load_;

 private:
  SessionRestore();

  DISALLOW_COPY_AND_ASSIGN(SessionRestore);
};

#endif  

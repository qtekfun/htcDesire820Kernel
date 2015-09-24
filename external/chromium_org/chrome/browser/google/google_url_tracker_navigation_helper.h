// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GOOGLE_GOOGLE_URL_TRACKER_NAVIGATION_HELPER_H_
#define CHROME_BROWSER_GOOGLE_GOOGLE_URL_TRACKER_NAVIGATION_HELPER_H_

class GoogleURLTracker;
class InfoBarService;
class Profile;

namespace content {
class NavigationController;
}

class GoogleURLTrackerNavigationHelper {
 public:
  virtual ~GoogleURLTrackerNavigationHelper() {}

  
  virtual void SetGoogleURLTracker(GoogleURLTracker* tracker) = 0;

  
  
  virtual void SetListeningForNavigationStart(bool listen) = 0;

  
  
  virtual bool IsListeningForNavigationStart() = 0;

  
  
  
  virtual void SetListeningForNavigationCommit(
      const content::NavigationController* nav_controller,
      bool listen) = 0;

  
  
  virtual bool IsListeningForNavigationCommit(
      const content::NavigationController* nav_controller) = 0;

  
  
  
  virtual void SetListeningForTabDestruction(
      const content::NavigationController* nav_controller,
      bool listen) = 0;

  
  
  virtual bool IsListeningForTabDestruction(
      const content::NavigationController* nav_controller) = 0;
};

#endif  

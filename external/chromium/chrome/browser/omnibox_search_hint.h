// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_OMNIBOX_SEARCH_HINT_H_
#define CHROME_BROWSER_OMNIBOX_SEARCH_HINT_H_
#pragma once

#include <map>
#include <string>

#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Profile;
class TabContents;

class OmniboxSearchHint : public NotificationObserver {
 public:
  explicit OmniboxSearchHint(TabContents* tab);
  ~OmniboxSearchHint();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void ShowEnteringQuery();

  TabContents* tab() { return tab_; }

  
  void DisableHint();

  
  
  static bool IsEnabled(Profile* profile);

 private:
  void ShowInfoBar();

  NotificationRegistrar notification_registrar_;

  
  TabContents* tab_;

  
  
  std::map<std::string, int> search_engine_urls_;

  DISALLOW_COPY_AND_ASSIGN(OmniboxSearchHint);
};

#endif  

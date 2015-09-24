// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TABS_PINNED_TAB_SERVICE_H_
#define CHROME_BROWSER_UI_TABS_PINNED_TAB_SERVICE_H_

#include "base/compiler_specific.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

class PinnedTabService : public content::NotificationObserver,
                         public BrowserContextKeyedService {
 public:
  explicit PinnedTabService(Profile* profile);

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  Profile* profile_;

  
  
  bool save_pinned_tabs_;

  
  bool has_normal_browser_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(PinnedTabService);
};

#endif  

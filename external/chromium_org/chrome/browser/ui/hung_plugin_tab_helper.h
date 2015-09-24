// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_HUNG_PLUGIN_TAB_HELPER_H_
#define CHROME_BROWSER_UI_HUNG_PLUGIN_TAB_HELPER_H_

#include <map>

#include "base/memory/linked_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class InfoBarDelegate;

namespace base {
class FilePath;
}

class HungPluginTabHelper
    : public content::WebContentsObserver,
      public content::NotificationObserver,
      public content::WebContentsUserData<HungPluginTabHelper> {
 public:
  virtual ~HungPluginTabHelper();

  
  virtual void PluginCrashed(const base::FilePath& plugin_path,
                             base::ProcessId plugin_pid) OVERRIDE;
  virtual void PluginHungStatusChanged(int plugin_child_id,
                                       const base::FilePath& plugin_path,
                                       bool is_hung) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void KillPlugin(int child_id);

 private:
  friend class content::WebContentsUserData<HungPluginTabHelper>;

  struct PluginState;
  typedef std::map<int, linked_ptr<PluginState> > PluginStateMap;

  explicit HungPluginTabHelper(content::WebContents* contents);

  
  void OnReshowTimer(int child_id);

  
  
  void ShowBar(int child_id, PluginState* state);

  
  
  void CloseBar(PluginState* state);

  content::NotificationRegistrar registrar_;

  
  PluginStateMap hung_plugins_;

  DISALLOW_COPY_AND_ASSIGN(HungPluginTabHelper);
};

#endif  

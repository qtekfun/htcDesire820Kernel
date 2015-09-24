// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_NTP_RESOURCE_CACHE_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_NTP_RESOURCE_CACHE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/strings/string16.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class Profile;

namespace base {
class RefCountedMemory;
}

namespace content {
class RenderProcessHost;
}

class NTPResourceCache : public content::NotificationObserver,
                         public BrowserContextKeyedService {
 public:
  enum WindowType {
    NORMAL,
    INCOGNITO,
    GUEST,
  };

  explicit NTPResourceCache(Profile* profile);
  virtual ~NTPResourceCache();

  base::RefCountedMemory* GetNewTabHTML(WindowType win_type);
  base::RefCountedMemory* GetNewTabCSS(WindowType win_type);

  void set_should_show_apps_page(bool should_show_apps_page) {
    should_show_apps_page_ = should_show_apps_page;
  }
  void set_should_show_most_visited_page(bool should_show_most_visited_page) {
    should_show_most_visited_page_ = should_show_most_visited_page;
  }
  void set_should_show_other_devices_menu(bool should_show_other_devices_menu) {
    should_show_other_devices_menu_ = should_show_other_devices_menu;
  }
  void set_should_show_recently_closed_menu(
      bool should_show_recently_closed_menu) {
    should_show_recently_closed_menu_ = should_show_recently_closed_menu;
  }
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  static WindowType GetWindowType(
      Profile* profile, content::RenderProcessHost* render_host);

 private:
  void OnPreferenceChanged();

  void CreateNewTabHTML();

  
  
  
  bool NewTabCacheNeedsRefresh();

  Profile* profile_;

  scoped_refptr<base::RefCountedMemory> new_tab_html_;

#if !defined(OS_ANDROID)
  
  
  base::string16 GetSyncTypeMessage();

  void CreateNewTabIncognitoHTML();
  void CreateNewTabIncognitoCSS();

  void CreateNewTabGuestHTML();
  void CreateNewTabGuestCSS();

  void CreateNewTabCSS();

  scoped_refptr<base::RefCountedMemory> new_tab_guest_html_;
  scoped_refptr<base::RefCountedMemory> new_tab_guest_css_;
  scoped_refptr<base::RefCountedMemory> new_tab_incognito_html_;
  scoped_refptr<base::RefCountedMemory> new_tab_incognito_css_;
  scoped_refptr<base::RefCountedMemory> new_tab_css_;
  content::NotificationRegistrar registrar_;
  PrefChangeRegistrar profile_pref_change_registrar_;
  PrefChangeRegistrar local_state_pref_change_registrar_;
#endif

  
  bool is_swipe_tracking_from_scroll_events_enabled_;
  
  bool should_show_apps_page_;
  
  
  bool should_show_most_visited_page_;
  bool should_show_other_devices_menu_;
  bool should_show_recently_closed_menu_;

  DISALLOW_COPY_AND_ASSIGN(NTPResourceCache);
};

#endif  

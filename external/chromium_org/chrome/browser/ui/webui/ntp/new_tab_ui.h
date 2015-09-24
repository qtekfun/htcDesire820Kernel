// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_UI_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_NEW_TAB_UI_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/url_data_source.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_ui_controller.h"

class GURL;
class Profile;

namespace base {
class DictionaryValue;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class NewTabUI : public content::WebUIController,
                 public content::WebContentsObserver,
                 public content::NotificationObserver {
 public:
  explicit NewTabUI(content::WebUI* web_ui);
  virtual ~NewTabUI();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  
  
  
  
  
  static bool MightShowApps();

  
  static bool ShouldShowApps();

  
  static bool IsDiscoveryInNTPEnabled();

  
  
  static void SetUrlTitleAndDirection(base::DictionaryValue* dictionary,
                                      const base::string16& title,
                                      const GURL& gurl);

  
  static void SetFullNameAndDirection(const base::string16& full_name,
                                      base::DictionaryValue* dictionary);

  
  
  static NewTabUI* FromWebUIController(content::WebUIController* ui);

  
  static int current_pref_version() { return current_pref_version_; }

  
  virtual void RenderViewCreated(
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void RenderViewReused(
      content::RenderViewHost* render_view_host) OVERRIDE;

  
  virtual void WasHidden() OVERRIDE;

  bool showing_sync_bubble() { return showing_sync_bubble_; }
  void set_showing_sync_bubble(bool showing) { showing_sync_bubble_ = showing; }

  class NewTabHTMLSource : public content::URLDataSource {
   public:
    explicit NewTabHTMLSource(Profile* profile);
    virtual ~NewTabHTMLSource();

    
    virtual std::string GetSource() const OVERRIDE;
    virtual void StartDataRequest(
        const std::string& path,
        int render_process_id,
        int render_view_id,
        const content::URLDataSource::GotDataCallback& callback) OVERRIDE;
    virtual std::string GetMimeType(const std::string&) const OVERRIDE;
    virtual bool ShouldReplaceExistingSource() const OVERRIDE;
    virtual bool ShouldAddContentSecurityPolicy() const OVERRIDE;

    
    
    
    void AddResource(const char* resource,
                     const char* mime_type,
                     int resource_id);

   private:
    
    Profile* profile_;

    
    std::map<std::string, std::pair<std::string, int> > resource_map_;

    DISALLOW_COPY_AND_ASSIGN(NewTabHTMLSource);
  };

 private:
  FRIEND_TEST_ALL_PREFIXES(NewTabUITest, UpdateUserPrefsVersion);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  void EmitNtpStatistics();

  void OnShowBookmarkBarChanged();

  void StartTimingPaint(content::RenderViewHost* render_view_host);
  void PaintTimeout();

  Profile* GetProfile() const;

  content::NotificationRegistrar registrar_;

  
  base::TimeTicks start_;
  
  base::TimeTicks last_paint_;
  
  base::OneShotTimer<NewTabUI> timer_;
  
  static const int current_pref_version_ = 3;

  
  bool showing_sync_bubble_;

  PrefChangeRegistrar pref_change_registrar_;

  DISALLOW_COPY_AND_ASSIGN(NewTabUI);
};

#endif  

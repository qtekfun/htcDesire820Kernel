// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NEW_TAB_UI_H_
#define CHROME_BROWSER_UI_WEBUI_NEW_TAB_UI_H_
#pragma once

#include <string>

#include "base/gtest_prod_util.h"
#include "base/timer.h"
#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class MessageLoop;
class PrefService;
class Profile;

class NewTabUI : public WebUI,
                 public NotificationObserver {
 public:
  explicit NewTabUI(TabContents* manager);
  ~NewTabUI();

  
  
  virtual void RenderViewCreated(RenderViewHost* render_view_host);
  virtual void RenderViewReused(RenderViewHost* render_view_host);

  static void RegisterUserPrefs(PrefService* prefs);
  static void MigrateUserPrefs(PrefService* prefs, int old_pref_version,
                               int new_pref_version);

  
  
  static bool FirstRunDisabled();

  
  
  static void SetURLTitleAndDirection(DictionaryValue* dictionary,
                                      const string16& title,
                                      const GURL& gurl);

  
  
  static void AddRecentlyClosedEntries(
      const TabRestoreService::Entries& entries,
      ListValue* entry_list_value);

  
  static int current_pref_version() { return current_pref_version_; }

  class NewTabHTMLSource : public ChromeURLDataManager::DataSource {
   public:
    explicit NewTabHTMLSource(Profile* profile);

    
    
    virtual void StartDataRequest(const std::string& path,
                                  bool is_incognito,
                                  int request_id);

    virtual std::string GetMimeType(const std::string&) const;

    virtual bool ShouldReplaceExistingSource() const;

    
    static void set_first_run(bool first_run) { first_run_ = first_run; }
    static bool first_run() { return first_run_; }

   private:
    virtual ~NewTabHTMLSource() {}

    
    static bool first_run_;

    
    Profile* profile_;

    DISALLOW_COPY_AND_ASSIGN(NewTabHTMLSource);
  };

 private:
  FRIEND_TEST_ALL_PREFIXES(NewTabUITest, UpdateUserPrefsVersion);

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void InitializeCSSCaches();

  void StartTimingPaint(RenderViewHost* render_view_host);
  void PaintTimeout();

  
  
  static bool UpdateUserPrefsVersion(PrefService* prefs);

  NotificationRegistrar registrar_;

  
  base::TimeTicks start_;
  
  base::TimeTicks last_paint_;
  
  base::OneShotTimer<NewTabUI> timer_;
  
  static const int current_pref_version_ = 3;

  DISALLOW_COPY_AND_ASSIGN(NewTabUI);
};

#endif  

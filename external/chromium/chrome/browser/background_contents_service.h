// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BACKGROUND_CONTENTS_SERVICE_H_
#define CHROME_BROWSER_BACKGROUND_CONTENTS_SERVICE_H_
#pragma once

#include <map>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "chrome/browser/profiles/profile_keyed_service.h"
#include "chrome/browser/tab_contents/background_contents.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "content/common/window_container_type.h"
#include "googleurl/src/gurl.h"
#include "webkit/glue/window_open_disposition.h"

class CommandLine;
class DictionaryValue;
class NotificationDelegate;
class PrefService;
class Profile;
class TabContents;

namespace gfx {
class Rect;
}

struct BackgroundContentsOpenedDetails;

class BackgroundContentsService : private NotificationObserver,
                                  public BackgroundContents::Delegate,
                                  public ProfileKeyedService {
 public:
  BackgroundContentsService(Profile* profile, const CommandLine* command_line);
  virtual ~BackgroundContentsService();

  
  
  BackgroundContents* GetAppBackgroundContents(const string16& appid);

  
  std::vector<BackgroundContents*> GetBackgroundContents() const;

  static void RegisterUserPrefs(PrefService* prefs);

  
  virtual void AddTabContents(TabContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture);

  
  
  
  const string16& GetParentApplicationId(BackgroundContents* contents) const;

  
  
  
  
  
  
  BackgroundContents* CreateBackgroundContents(SiteInstance* site,
                                               int route_id,
                                               Profile* profile,
                                               const string16& frame_name,
                                               const string16& application_id);

  
  
  
  
  void LoadBackgroundContentsForExtension(Profile* profile,
                                          const std::string& extension_id);

 private:
  friend class BackgroundContentsServiceTest;
  friend class MockBackgroundContents;
  friend class TaskManagerBrowserTest;

  FRIEND_TEST_ALL_PREFIXES(BackgroundContentsServiceTest,
                           BackgroundContentsCreateDestroy);
  FRIEND_TEST_ALL_PREFIXES(BackgroundContentsServiceTest,
                           TestApplicationIDLinkage);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerBrowserTest,
                           NoticeBGContentsChanges);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerBrowserTest,
                           KillBGContents);

  
  void StartObserving(Profile* profile);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  void LoadBackgroundContentsFromPrefs(Profile* profile);

  
  
  void LoadBackgroundContentsFromDictionary(Profile* profile,
                                            const std::string& extension_id,
                                            const DictionaryValue* contents);

  
  
  void LoadBackgroundContentsFromManifests(Profile* profile);

  
  
  
  void LoadBackgroundContents(Profile* profile,
                              const GURL& url,
                              const string16& frame_name,
                              const string16& appid);

  
  void BackgroundContentsOpened(BackgroundContentsOpenedDetails* details);

  
  void BackgroundContentsShutdown(BackgroundContents* contents);

  
  
  
  
  void RegisterBackgroundContents(BackgroundContents* contents);

  
  void UnregisterBackgroundContents(BackgroundContents* contents);

  
  
  void ShutdownAssociatedBackgroundContents(const string16& appid);

  
  bool IsTracked(BackgroundContents* contents) const;

  
  
  PrefService* prefs_;
  NotificationRegistrar registrar_;

  
  struct BackgroundContentsInfo {
    
    BackgroundContents* contents;
    
    string16 frame_name;
  };

  
  
  
  
  typedef std::map<string16, BackgroundContentsInfo> BackgroundContentsMap;
  BackgroundContentsMap contents_map_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundContentsService);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BACKGROUND_BACKGROUND_CONTENTS_SERVICE_H_
#define CHROME_BROWSER_BACKGROUND_BACKGROUND_CONTENTS_SERVICE_H_

#include <map>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/tab_contents/background_contents.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/common/window_container_type.h"
#include "ui/base/window_open_disposition.h"
#include "url/gurl.h"

class CommandLine;
class PrefService;
class Profile;

namespace base {
class DictionaryValue;
}

namespace content {
class SessionStorageNamespace;
}

namespace extensions {
class Extension;
}

namespace gfx {
class Rect;
}

struct BackgroundContentsOpenedDetails;

class BackgroundContentsService : private content::NotificationObserver,
                                  public BackgroundContents::Delegate,
                                  public BrowserContextKeyedService {
 public:
  BackgroundContentsService(Profile* profile, const CommandLine* command_line);
  virtual ~BackgroundContentsService();

  
  
  static void SetRestartDelayForForceInstalledAppsAndExtensionsForTesting(
      int restart_delay_in_ms);

  
  
  BackgroundContents* GetAppBackgroundContents(const base::string16& appid);

  
  
  
  
  bool HasRegisteredBackgroundContents(const base::string16& appid);

  
  std::vector<BackgroundContents*> GetBackgroundContents() const;

  
  virtual void AddWebContents(content::WebContents* new_contents,
                              WindowOpenDisposition disposition,
                              const gfx::Rect& initial_pos,
                              bool user_gesture,
                              bool* was_blocked) OVERRIDE;

  
  
  
  const base::string16& GetParentApplicationId(BackgroundContents* contents) const;

  
  
  
  
  
  
  BackgroundContents* CreateBackgroundContents(
      content::SiteInstance* site,
      int route_id,
      Profile* profile,
      const base::string16& frame_name,
      const base::string16& application_id,
      const std::string& partition_id,
      content::SessionStorageNamespace* session_storage_namespace);

  
  
  
  
  void LoadBackgroundContentsForExtension(Profile* profile,
                                          const std::string& extension_id);

 private:
  friend class BackgroundContentsServiceTest;
  friend class MockBackgroundContents;
  friend class TaskManagerNoShowBrowserTest;

  FRIEND_TEST_ALL_PREFIXES(BackgroundContentsServiceTest,
                           BackgroundContentsCreateDestroy);
  FRIEND_TEST_ALL_PREFIXES(BackgroundContentsServiceTest,
                           TestApplicationIDLinkage);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerNoShowBrowserTest,
                           NoticeBGContentsChanges);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerNoShowBrowserTest,
                           KillBGContents);

  
  void StartObserving(Profile* profile);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void RestartForceInstalledExtensionOnCrash(
      const extensions::Extension* extension,
      Profile* profile);

  
  void LoadBackgroundContentsFromPrefs(Profile* profile);

  
  
  void LoadBackgroundContentsFromDictionary(
      Profile* profile,
      const std::string& extension_id,
      const base::DictionaryValue* contents);

  
  
  void LoadBackgroundContentsFromManifests(Profile* profile);

  
  
  
  void LoadBackgroundContents(Profile* profile,
                              const GURL& url,
                              const base::string16& frame_name,
                              const base::string16& appid);

  
  void BackgroundContentsOpened(BackgroundContentsOpenedDetails* details);

  
  void BackgroundContentsShutdown(BackgroundContents* contents);

  
  
  
  
  void RegisterBackgroundContents(BackgroundContents* contents);

  
  void UnregisterBackgroundContents(BackgroundContents* contents);

  
  
  void ShutdownAssociatedBackgroundContents(const base::string16& appid);

  
  bool IsTracked(BackgroundContents* contents) const;

  
  
  
  void SendChangeNotification(Profile* profile);

  
  static int restart_delay_in_ms_;

  
  
  PrefService* prefs_;
  content::NotificationRegistrar registrar_;

  
  struct BackgroundContentsInfo {
    
    BackgroundContents* contents;
    
    base::string16 frame_name;
  };

  
  
  
  
  typedef std::map<base::string16, BackgroundContentsInfo>
      BackgroundContentsMap;
  BackgroundContentsMap contents_map_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundContentsService);
};

#endif  

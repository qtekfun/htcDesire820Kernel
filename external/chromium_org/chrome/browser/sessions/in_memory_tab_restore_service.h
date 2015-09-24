// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_IN_MEMORY_TAB_RESTORE_SERVICE_H_
#define CHROME_BROWSER_SESSIONS_IN_MEMORY_TAB_RESTORE_SERVICE_H_

#include <vector>

#include "chrome/browser/sessions/tab_restore_service.h"
#include "chrome/browser/sessions/tab_restore_service_helper.h"

class InMemoryTabRestoreService : public TabRestoreService {
 public:
  
  
  
  InMemoryTabRestoreService(Profile* profile,
                            TimeFactory* time_factory);

  virtual ~InMemoryTabRestoreService();

  
  virtual void AddObserver(TabRestoreServiceObserver* observer) OVERRIDE;
  virtual void RemoveObserver(TabRestoreServiceObserver* observer) OVERRIDE;
  virtual void CreateHistoricalTab(content::WebContents* contents,
                                   int index) OVERRIDE;
  virtual void BrowserClosing(TabRestoreServiceDelegate* delegate) OVERRIDE;
  virtual void BrowserClosed(TabRestoreServiceDelegate* delegate) OVERRIDE;
  virtual void ClearEntries() OVERRIDE;
  virtual const Entries& entries() const OVERRIDE;
  virtual std::vector<content::WebContents*> RestoreMostRecentEntry(
      TabRestoreServiceDelegate* delegate,
      chrome::HostDesktopType host_desktop_type) OVERRIDE;
  virtual Tab* RemoveTabEntryById(SessionID::id_type id) OVERRIDE;
  virtual std::vector<content::WebContents*>
    RestoreEntryById(TabRestoreServiceDelegate* delegate,
                     SessionID::id_type id,
                     chrome::HostDesktopType host_desktop_type,
                     WindowOpenDisposition disposition) OVERRIDE;
  virtual void LoadTabsFromLastSession() OVERRIDE;
  virtual bool IsLoaded() const OVERRIDE;
  virtual void DeleteLastSession() OVERRIDE;
  virtual void Shutdown() OVERRIDE;

 private:
  TabRestoreServiceHelper helper_;

  DISALLOW_COPY_AND_ASSIGN(InMemoryTabRestoreService);
};

#endif  

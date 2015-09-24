// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_H_
#define CHROME_BROWSER_SESSIONS_TAB_RESTORE_SERVICE_H_

#include <list>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/browser/sessions/session_types.h"
#include "chrome/browser/ui/host_desktop.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/sessions/serialized_navigation_entry.h"
#include "content/public/browser/session_storage_namespace.h"
#include "ui/base/window_open_disposition.h"

class TabRestoreServiceDelegate;
class TabRestoreServiceObserver;

namespace content {
class SessionStorageNamespace;
class WebContents;
}

class TabRestoreService : public BrowserContextKeyedService {
 public:
  
  class TimeFactory {
   public:
    virtual ~TimeFactory();
    virtual base::Time TimeNow() = 0;
  };

  
  enum Type {
    TAB,
    WINDOW
  };

  struct Entry {
    Entry();
    explicit Entry(Type type);
    virtual ~Entry();

    
    
    SessionID::id_type id;

    
    Type type;

    
    base::Time timestamp;

    
    
    
    bool from_last_session;
  };

  
  struct Tab : public Entry {
    Tab();
    virtual ~Tab();

    bool has_browser() const { return browser_id > 0; }

    
    std::vector<sessions::SerializedNavigationEntry> navigations;

    
    int current_navigation_index;

    
    
    SessionID::id_type browser_id;

    
    int tabstrip_index;

    
    bool pinned;

    
    std::string extension_app_id;

    
    scoped_refptr<content::SessionStorageNamespace> session_storage_namespace;

    
    std::string user_agent_override;
  };

  
  struct Window : public Entry {
    Window();
    virtual ~Window();

    
    std::vector<Tab> tabs;

    
    int selected_tab_index;

    
    std::string app_name;
  };

  typedef std::list<Entry*> Entries;

  virtual ~TabRestoreService();

  
  
  virtual void AddObserver(TabRestoreServiceObserver* observer) = 0;
  virtual void RemoveObserver(TabRestoreServiceObserver* observer) = 0;

  
  
  virtual void CreateHistoricalTab(content::WebContents* contents,
                                   int index) = 0;

  
  
  
  virtual void BrowserClosing(TabRestoreServiceDelegate* delegate) = 0;

  
  virtual void BrowserClosed(TabRestoreServiceDelegate* delegate) = 0;

  
  
  virtual void ClearEntries() = 0;

  
  
  virtual const Entries& entries() const = 0;

  
  
  
  
  
  virtual std::vector<content::WebContents*> RestoreMostRecentEntry(
      TabRestoreServiceDelegate* delegate,
      chrome::HostDesktopType host_desktop_type) = 0;

  
  
  virtual Tab* RemoveTabEntryById(SessionID::id_type id) = 0;

  
  
  
  
  
  
  
  virtual std::vector<content::WebContents*> RestoreEntryById(
      TabRestoreServiceDelegate* delegate,
      SessionID::id_type id,
      chrome::HostDesktopType host_desktop_type,
      WindowOpenDisposition disposition) = 0;

  
  
  virtual void LoadTabsFromLastSession() = 0;

  
  virtual bool IsLoaded() const = 0;

  
  virtual void DeleteLastSession() = 0;
};

#endif  

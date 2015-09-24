// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_TABS_TABS_EVENT_ROUTER_H_
#define CHROME_BROWSER_EXTENSIONS_API_TABS_TABS_EVENT_ROUTER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/extensions/api/tabs/tabs_api.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/event_router.h"

namespace content {
class WebContents;
}

namespace extensions {

class TabsEventRouter : public TabStripModelObserver,
                        public chrome::BrowserListObserver,
                        public content::NotificationObserver {
 public:
  explicit TabsEventRouter(Profile* profile);
  virtual ~TabsEventRouter();

  
  virtual void OnBrowserAdded(Browser* browser) OVERRIDE;
  virtual void OnBrowserRemoved(Browser* browser) OVERRIDE;
  virtual void OnBrowserSetLastActive(Browser* browser) OVERRIDE;

  
  virtual void TabInsertedAt(content::WebContents* contents, int index,
                             bool active) OVERRIDE;
  virtual void TabClosingAt(TabStripModel* tab_strip_model,
                            content::WebContents* contents,
                            int index) OVERRIDE;
  virtual void TabDetachedAt(content::WebContents* contents,
                             int index) OVERRIDE;
  virtual void ActiveTabChanged(content::WebContents* old_contents,
                                content::WebContents* new_contents,
                                int index,
                                int reason) OVERRIDE;
  virtual void TabSelectionChanged(
      TabStripModel* tab_strip_model,
      const ui::ListSelectionModel& old_model) OVERRIDE;
  virtual void TabMoved(content::WebContents* contents,
                        int from_index,
                        int to_index) OVERRIDE;
  virtual void TabChangedAt(content::WebContents* contents,
                            int index,
                            TabChangeType change_type) OVERRIDE;
  virtual void TabReplacedAt(TabStripModel* tab_strip_model,
                             content::WebContents* old_contents,
                             content::WebContents* new_contents,
                             int index) OVERRIDE;
  virtual void TabPinnedStateChanged(content::WebContents* contents,
                                     int index) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;
 private:
  
  void TabCreatedAt(content::WebContents* contents, int index, bool active);

  
  
  void TabUpdated(content::WebContents* contents, bool did_navigate);

  
  void FaviconUrlUpdated(content::WebContents* contents);

  
  
  
  void DispatchEvent(Profile* profile,
                     const std::string& event_name,
                     scoped_ptr<base::ListValue> args,
                     EventRouter::UserGestureState user_gesture);

  void DispatchEventsAcrossIncognito(
      Profile* profile,
      const std::string& event_name,
      scoped_ptr<base::ListValue> event_args,
      scoped_ptr<base::ListValue> cross_incognito_args);

  void DispatchSimpleBrowserEvent(Profile* profile,
                                  const int window_id,
                                  const std::string& event_name);

  
  
  void DispatchTabUpdatedEvent(content::WebContents* contents,
                               scoped_ptr<DictionaryValue> changed_properties);

  
  
  void RegisterForBrowserNotifications(Browser* browser);

  
  
  void RegisterForTabNotifications(content::WebContents* contents);

  
  void UnregisterForTabNotifications(content::WebContents* contents);

  content::NotificationRegistrar registrar_;

  
  
  
  
  
  class TabEntry {
   public:
    
    
    
    TabEntry();

    
    
    
    
    
    
    DictionaryValue* UpdateLoadState(const content::WebContents* contents);

    
    
    
    DictionaryValue* DidNavigate(const content::WebContents* contents);

   private:
    
    
    
    
    bool complete_waiting_on_load_;

    GURL url_;
  };

  
  
  TabEntry* GetTabEntry(const content::WebContents* contents);

  std::map<int, TabEntry> tab_entries_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(TabsEventRouter);
};

}  

#endif  

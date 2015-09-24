// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_SHORTCUTS_BACKEND_H_
#define CHROME_BROWSER_HISTORY_SHORTCUTS_BACKEND_H_

#include <map>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "url/gurl.h"

class Profile;

namespace history {

class ShortcutsDatabase;

class ShortcutsBackend : public RefcountedBrowserContextKeyedService,
                         public content::NotificationObserver {
 public:
  
  struct Shortcut {
    
    struct MatchCore {
      explicit MatchCore(const AutocompleteMatch& match);
      MatchCore(const base::string16& fill_into_edit,
                const GURL& destination_url,
                const base::string16& contents,
                const ACMatchClassifications& contents_class,
                const base::string16& description,
                const ACMatchClassifications& description_class,
                content::PageTransition transition,
                AutocompleteMatch::Type type,
                const base::string16& keyword);
      ~MatchCore();

      AutocompleteMatch ToMatch() const;

      base::string16 fill_into_edit;
      GURL destination_url;
      base::string16 contents;
      
      
      
      ACMatchClassifications contents_class;
      base::string16 description;
      ACMatchClassifications description_class;
      content::PageTransition transition;
      AutocompleteMatch::Type type;
      base::string16 keyword;
    };

    Shortcut(const std::string& id,
             const base::string16& text,
             const MatchCore& match_core,
             const base::Time& last_access_time,
             int number_of_hits);
    
    Shortcut();
    ~Shortcut();

    std::string id;  
    base::string16 text;   
    MatchCore match_core;
    base::Time last_access_time;  
    int number_of_hits;           
  };

  typedef std::multimap<base::string16, const Shortcut> ShortcutMap;

  
  
  
  ShortcutsBackend(Profile* profile, bool suppress_db);

  
  
  class ShortcutsBackendObserver {
   public:
    
    virtual void OnShortcutsLoaded() = 0;
    
    virtual void OnShortcutsChanged() {}

   protected:
    virtual ~ShortcutsBackendObserver() {}
  };

  
  
  bool Init();

  

  bool initialized() const { return current_state_ == INITIALIZED; }
  const ShortcutMap& shortcuts_map() const { return shortcuts_map_; }

  
  bool DeleteShortcutsWithUrl(const GURL& shortcut_url);

  void AddObserver(ShortcutsBackendObserver* obs);
  void RemoveObserver(ShortcutsBackendObserver* obs);

  
  
  void AddOrUpdateShortcut(const base::string16& text,
                           const AutocompleteMatch& match);

 private:
  friend class base::RefCountedThreadSafe<ShortcutsBackend>;
  friend class ShortcutsProviderTest;
  FRIEND_TEST_ALL_PREFIXES(ShortcutsBackendTest, AddAndUpdateShortcut);
  FRIEND_TEST_ALL_PREFIXES(ShortcutsBackendTest, DeleteShortcuts);

  enum CurrentState {
    NOT_INITIALIZED,  
    INITIALIZING,     
    INITIALIZED,      
                      
  };

  typedef std::map<std::string, ShortcutMap::iterator> GuidMap;

  virtual ~ShortcutsBackend();

  
  virtual void ShutdownOnUIThread() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void InitInternal();

  
  void InitCompleted();

  
  bool AddShortcut(const Shortcut& shortcut);

  
  bool UpdateShortcut(const Shortcut& shortcut);

  
  bool DeleteShortcutsWithIds(const std::vector<std::string>& shortcut_ids);

  
  
  bool DeleteShortcutsWithUrl(const GURL& url, bool exact_match);

  
  bool DeleteAllShortcuts();

  CurrentState current_state_;
  ObserverList<ShortcutsBackendObserver> observer_list_;
  scoped_refptr<ShortcutsDatabase> db_;

  
  
  
  scoped_ptr<ShortcutMap> temp_shortcuts_map_;
  scoped_ptr<GuidMap> temp_guid_map_;

  ShortcutMap shortcuts_map_;
  
  GuidMap guid_map_;

  content::NotificationRegistrar notification_registrar_;

  
  bool no_db_access_;

  DISALLOW_COPY_AND_ASSIGN(ShortcutsBackend);
};

}  

#endif  

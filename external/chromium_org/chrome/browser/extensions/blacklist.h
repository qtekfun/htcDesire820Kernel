// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_BLACKLIST_H_
#define CHROME_BROWSER_EXTENSIONS_BLACKLIST_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/safe_browsing/database_manager.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace extensions {

class Extension;
class ExtensionPrefs;

class Blacklist : public content::NotificationObserver,
                  public base::SupportsWeakPtr<Blacklist> {
 public:
  class Observer {
   public:
    
    explicit Observer(Blacklist* blacklist);

    virtual void OnBlacklistUpdated() = 0;

   protected:
    virtual ~Observer();

   private:
    Blacklist* blacklist_;
  };

  class ScopedDatabaseManagerForTest {
   public:
    explicit ScopedDatabaseManagerForTest(
        scoped_refptr<SafeBrowsingDatabaseManager> database_manager);

    ~ScopedDatabaseManagerForTest();

   private:
    scoped_refptr<SafeBrowsingDatabaseManager> original_;

    DISALLOW_COPY_AND_ASSIGN(ScopedDatabaseManagerForTest);
  };

  
  
  enum BlacklistState {
    NOT_BLACKLISTED = 0,
    BLACKLISTED_MALWARE = 1,
    BLACKLISTED_SECURITY_VULNERABILITY = 2,
    BLACKLISTED_CWS_POLICY_VIOLATION = 3,
    BLACKLISTED_POTENTIALLY_UNWANTED = 4
  };

  typedef std::map<std::string, BlacklistState> BlacklistStateMap;

  typedef base::Callback<void(const BlacklistStateMap&)>
      GetBlacklistedIDsCallback;

  typedef base::Callback<void(const std::set<std::string>&)>
      GetMalwareIDsCallback;

  typedef base::Callback<void(BlacklistState)> IsBlacklistedCallback;

  explicit Blacklist(ExtensionPrefs* prefs);

  virtual ~Blacklist();

  
  
  
  
  
  
  
  
  void GetBlacklistedIDs(const std::set<std::string>& ids,
                         const GetBlacklistedIDsCallback& callback);

  
  
  
  
  void GetMalwareIDs(const std::set<std::string>& ids,
                     const GetMalwareIDsCallback& callback);

  
  void IsBlacklisted(const std::string& extension_id,
                     const IsBlacklistedCallback& callback);

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 private:
  
  static void SetDatabaseManager(
      scoped_refptr<SafeBrowsingDatabaseManager> database_manager);
  static scoped_refptr<SafeBrowsingDatabaseManager> GetDatabaseManager();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  void GetBlacklistStateForIDs(const GetBlacklistedIDsCallback& callback,
                               const std::set<std::string>& blacklisted_ids);

  void RequestExtensionsBlacklistState(const std::set<std::string> ids,
                                       base::Callback<void()> callback);

  void ReturnBlacklistStateMap(const GetBlacklistedIDsCallback& callback,
                               const std::set<std::string>& blacklisted_ids);

  ObserverList<Observer> observers_;

  content::NotificationRegistrar registrar_;

  BlacklistStateMap blacklist_state_cache_;

  DISALLOW_COPY_AND_ASSIGN(Blacklist);
};

}  

#endif  

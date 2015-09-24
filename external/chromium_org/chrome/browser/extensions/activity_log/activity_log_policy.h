// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_ACTIVITY_LOG_POLICY_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_ACTIVITY_LOG_POLICY_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "chrome/browser/extensions/activity_log/activity_actions.h"
#include "chrome/browser/extensions/activity_log/activity_database.h"
#include "chrome/common/extensions/api/activity_log_private.h"
#include "content/public/browser/browser_thread.h"
#include "url/gurl.h"

class Profile;
class GURL;

namespace base {
class FilePath;
}

namespace extensions {

class Extension;

class ActivityLogPolicy {
 public:
  enum PolicyType {
    POLICY_FULLSTREAM,
    POLICY_COUNTS,
    POLICY_INVALID,
  };

  
  
  
  
  explicit ActivityLogPolicy(Profile* profile);

  
  
  
  virtual void Close() = 0;

  
  
  
  virtual void ProcessAction(scoped_refptr<Action> action) = 0;

  
  void SetClockForTesting(scoped_ptr<base::Clock> clock);

  
  
  
  
  class Util {
   public:
    
    
    typedef std::set<std::pair<Action::ActionType, std::string> > ApiSet;

    
    
    static std::string Serialize(const base::Value* value);

    
    
    
    
    
    static void StripPrivacySensitiveFields(scoped_refptr<Action> action);

    
    
    static void StripArguments(const ApiSet& api_whitelist,
                               scoped_refptr<Action> action);

    
    
    static base::Time AddDays(const base::Time& base_date, int days);

    
    
    
    static void ComputeDatabaseTimeBounds(const base::Time& now,
                                          int days_ago,
                                          int64* early_bound,
                                          int64* late_bound);

    
    
    
    
    static bool DropObsoleteTables(sql::Connection* db);

   private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(Util);
  };

 protected:
  
  
  virtual ~ActivityLogPolicy();

  
  
  
  base::Time Now() const;

 private:
  
  
  
  scoped_ptr<base::Clock> testing_clock_;

  DISALLOW_COPY_AND_ASSIGN(ActivityLogPolicy);
};

class ActivityLogDatabasePolicy : public ActivityLogPolicy,
                                  public ActivityDatabase::Delegate {
 public:
  ActivityLogDatabasePolicy(Profile* profile,
                            const base::FilePath& database_name);

  
  
  void Init();

  // Requests that in-memory state be written to the database.  This method can
  
  
  virtual void Flush();

  
  
  
  
  virtual void ReadFilteredData(
      const std::string& extension_id,
      const Action::ActionType type,
      const std::string& api_name,
      const std::string& page_url,
      const std::string& arg_url,
      const int days_ago,
      const base::Callback
         <void(scoped_ptr<Action::ActionVector>)>& callback) = 0;

  
  
  virtual void RemoveURLs(const std::vector<GURL>& restrict_urls) = 0;

  
  virtual void RemoveExtensionData(const std::string& extension_id) = 0;

  
  virtual void DeleteDatabase() = 0;

 protected:
  
  
  template<typename DatabaseType, typename DatabaseFunc>
  void ScheduleAndForget(DatabaseType db, DatabaseFunc func) {
    content::BrowserThread::PostTask(
        content::BrowserThread::DB,
        FROM_HERE,
        base::Bind(func, base::Unretained(db)));
  }

  template<typename DatabaseType, typename DatabaseFunc, typename ArgA>
  void ScheduleAndForget(DatabaseType db, DatabaseFunc func, ArgA a) {
    content::BrowserThread::PostTask(
        content::BrowserThread::DB,
        FROM_HERE,
        base::Bind(func, base::Unretained(db), a));
  }

  template<typename DatabaseType, typename DatabaseFunc,
      typename ArgA, typename ArgB>
  void ScheduleAndForget(DatabaseType db, DatabaseFunc func, ArgA a, ArgB b) {
    content::BrowserThread::PostTask(
        content::BrowserThread::DB,
        FROM_HERE,
        base::Bind(func, base::Unretained(db), a, b));
  }

  
  ActivityDatabase* activity_database() const { return db_; }

  
  
  
  sql::Connection* GetDatabaseConnection() const;

 private:
  
  
  ActivityDatabase* db_;
  base::FilePath database_path_;
};

}  

#endif  

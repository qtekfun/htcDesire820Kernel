// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_COUNTING_POLICY_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_COUNTING_POLICY_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "chrome/browser/extensions/activity_log/activity_database.h"
#include "chrome/browser/extensions/activity_log/activity_log_policy.h"
#include "chrome/browser/extensions/activity_log/database_string_table.h"

namespace extensions {

class CountingPolicy : public ActivityLogDatabasePolicy {
 public:
  explicit CountingPolicy(Profile* profile);
  virtual ~CountingPolicy();

  virtual void ProcessAction(scoped_refptr<Action> action) OVERRIDE;

  virtual void ReadFilteredData(
      const std::string& extension_id,
      const Action::ActionType type,
      const std::string& api_name,
      const std::string& page_url,
      const std::string& arg_url,
      const int days_ago,
      const base::Callback
          <void(scoped_ptr<Action::ActionVector>)>& callback) OVERRIDE;

  virtual void Close() OVERRIDE;

  
  const base::TimeDelta& retention_time() const { return retention_time_; }
  void set_retention_time(const base::TimeDelta& delta) {
    retention_time_ = delta;
  }

  
  virtual void RemoveURLs(const std::vector<GURL>&) OVERRIDE;

  
  virtual void RemoveExtensionData(const std::string& extension_id) OVERRIDE;

  
  virtual void DeleteDatabase() OVERRIDE;

  
  
  static const char* kTableName;
  static const char* kReadViewName;

 protected:
  
  
  virtual bool InitDatabase(sql::Connection* db) OVERRIDE;
  virtual bool FlushDatabase(sql::Connection* db) OVERRIDE;
  virtual void OnDatabaseFailure() OVERRIDE;
  virtual void OnDatabaseClose() OVERRIDE;

 private:
  
  
  
  typedef std::map<scoped_refptr<Action>, int, ActionComparatorExcludingTime>
      ActionQueue;

  // Adds an Action to those to be written out; this is an internal method used
  
  void QueueAction(scoped_refptr<Action> action);

  
  
  scoped_ptr<Action::ActionVector> DoReadFilteredData(
      const std::string& extension_id,
      const Action::ActionType type,
      const std::string& api_name,
      const std::string& page_url,
      const std::string& arg_url,
      const int days_ago);

  
  
  void DoRemoveURLs(const std::vector<GURL>& restrict_urls);

  
  
  void DoRemoveExtensionData(const std::string& extension_id);

  
  void DoDeleteDatabase();

  
  bool CleanOlderThan(sql::Connection* db, const base::Time& cutoff);

  
  
  bool CleanStringTables(sql::Connection* db);

  
  Util::ApiSet api_arg_whitelist_;

  
  
  
  DatabaseStringTable string_table_;
  DatabaseStringTable url_table_;

  // Tracks any pending updates to be written to the database, if write
  
  ActionQueue queued_actions_;

  
  
  
  
  base::Time queued_actions_date_;

  
  
  
  
  base::TimeDelta retention_time_;

  
  
  
  base::Time last_database_cleaning_time_;

  friend class CountingPolicyTest;
  FRIEND_TEST_ALL_PREFIXES(CountingPolicyTest, EarlyFlush);
  FRIEND_TEST_ALL_PREFIXES(CountingPolicyTest, MergingAndExpiring);
  FRIEND_TEST_ALL_PREFIXES(CountingPolicyTest, StringTableCleaning);
};

}  

#endif  

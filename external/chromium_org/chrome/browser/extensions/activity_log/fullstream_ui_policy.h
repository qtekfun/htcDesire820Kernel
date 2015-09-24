// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_FULLSTREAM_UI_POLICY_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_FULLSTREAM_UI_POLICY_H_

#include <string>

#include "chrome/browser/extensions/activity_log/activity_database.h"
#include "chrome/browser/extensions/activity_log/activity_log_policy.h"

class GURL;

namespace extensions {

class FullStreamUIPolicy : public ActivityLogDatabasePolicy {
 public:
  
  explicit FullStreamUIPolicy(Profile* profile);

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

  
  virtual void RemoveURLs(const std::vector<GURL>& restrict_urls) OVERRIDE;

  
  virtual void RemoveExtensionData(const std::string& extension_id) OVERRIDE;

  
  virtual void DeleteDatabase() OVERRIDE;

  
  static const char* kTableName;
  static const char* kTableContentFields[];
  static const char* kTableFieldTypes[];
  static const int kTableFieldCount;

 protected:
  
  
  virtual ~FullStreamUIPolicy();

  
  
  virtual bool InitDatabase(sql::Connection* db) OVERRIDE;
  virtual bool FlushDatabase(sql::Connection* db) OVERRIDE;
  virtual void OnDatabaseFailure() OVERRIDE;
  virtual void OnDatabaseClose() OVERRIDE;

  
  
  
  virtual scoped_refptr<Action> ProcessArguments(
      scoped_refptr<Action> action) const;

  
  
  void DoRemoveURLs(const std::vector<GURL>& restrict_urls);

  
  
  void DoRemoveExtensionData(const std::string& extension_id);

  
  
  void DoDeleteDatabase();

  // Tracks any pending updates to be written to the database, if write
  
  Action::ActionVector queued_actions_;

 private:
  
  
  void QueueAction(scoped_refptr<Action> action);

  
  
  scoped_ptr<Action::ActionVector> DoReadFilteredData(
      const std::string& extension_id,
      const Action::ActionType type,
      const std::string& api_name,
      const std::string& page_url,
      const std::string& arg_url,
      const int days_ago);
};

}  

#endif  

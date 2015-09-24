// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_WEBDATA_LOGINS_TABLE_H_
#define CHROME_BROWSER_WEBDATA_LOGINS_TABLE_H_
#pragma once

#include <vector>

#include "chrome/browser/webdata/web_database_table.h"

namespace base {
class Time;
}

namespace webkit_glue {
struct PasswordForm;
}

#if defined(OS_WIN)
struct IE7PasswordInfo;
#endif

class LoginsTable : public WebDatabaseTable {
 public:
  LoginsTable(sql::Connection* db, sql::MetaTable* meta_table)
      : WebDatabaseTable(db, meta_table) {}
  virtual ~LoginsTable() {}
  virtual bool Init();
  virtual bool IsSyncable();

  
  bool AddLogin(const webkit_glue::PasswordForm& form);

#if defined(OS_WIN)
  
  bool AddIE7Login(const IE7PasswordInfo& info);

  
  bool RemoveIE7Login(const IE7PasswordInfo& info);

  
  bool GetIE7Login(const IE7PasswordInfo& info, IE7PasswordInfo* result);
#endif

  
  bool UpdateLogin(const webkit_glue::PasswordForm& form);

  
  bool RemoveLogin(const webkit_glue::PasswordForm& form);

  
  
  
  bool RemoveLoginsCreatedBetween(base::Time delete_begin,
                                  base::Time delete_end);

  
  
  
  bool GetLogins(const webkit_glue::PasswordForm& form,
                 std::vector<webkit_glue::PasswordForm*>* forms);

  
  
  
  
  bool GetAllLogins(std::vector<webkit_glue::PasswordForm*>* forms,
                    bool include_blacklisted);

 private:
  DISALLOW_COPY_AND_ASSIGN(LoginsTable);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_LOGIN_DATABASE_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_LOGIN_DATABASE_H_
#pragma once

#include <string>
#include <vector>

#include "app/sql/connection.h"
#include "app/sql/meta_table.h"
#include "base/file_path.h"
#include "base/string16.h"
#include "webkit/glue/password_form.h"

class LoginDatabase {
 public:
  LoginDatabase();
  virtual ~LoginDatabase();

  
  
  bool Init(const FilePath& db_path);

  
  void ReportMetrics();

  
  bool AddLogin(const webkit_glue::PasswordForm& form);

  
  
  bool UpdateLogin(const webkit_glue::PasswordForm& form, int* items_changed);

  
  bool RemoveLogin(const webkit_glue::PasswordForm& form);

  
  
  
  bool RemoveLoginsCreatedBetween(const base::Time delete_begin,
                                  const base::Time delete_end);

  
  
  
  bool GetLogins(const webkit_glue::PasswordForm& form,
                 std::vector<webkit_glue::PasswordForm*>* forms) const;

  
  
  
  bool GetLoginsCreatedBetween(
      const base::Time begin,
      const base::Time end,
      std::vector<webkit_glue::PasswordForm*>* forms) const;

  
  
  bool GetAutofillableLogins(
      std::vector<webkit_glue::PasswordForm*>* forms) const;

  
  bool GetBlacklistLogins(std::vector<webkit_glue::PasswordForm*>* forms) const;

  
  
  
  
  
  bool DeleteAndRecreateDatabaseFile();

 private:
  
  std::string EncryptedString(const string16& plain_text) const;

  
  string16 DecryptedString(const std::string& cipher_text) const;

  bool InitLoginsTable();
  void MigrateOldVersionsAsNeeded();

  
  
  void InitPasswordFormFromStatement(webkit_glue::PasswordForm* form,
                                     sql::Statement& s) const;

  
  
  bool GetAllLoginsWithBlacklistSetting(
      bool blacklisted, std::vector<webkit_glue::PasswordForm*>* forms) const;

  FilePath db_path_;
  mutable sql::Connection db_;
  sql::MetaTable meta_table_;

  DISALLOW_COPY_AND_ASSIGN(LoginDatabase);
};

#endif  

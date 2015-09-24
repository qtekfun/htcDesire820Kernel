// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_LOGIN_DATABASE_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_LOGIN_DATABASE_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/pickle.h"
#include "base/strings/string16.h"
#include "chrome/browser/password_manager/psl_matching_helper.h"
#include "sql/connection.h"
#include "sql/meta_table.h"

namespace autofill {
struct PasswordForm;
}  

class LoginDatabase {
 public:
  LoginDatabase();
  virtual ~LoginDatabase();

  
  
  bool Init(const base::FilePath& db_path);

  
  void ReportMetrics();

  
  bool AddLogin(const autofill::PasswordForm& form);

  
  
  bool UpdateLogin(const autofill::PasswordForm& form, int* items_changed);

  
  bool RemoveLogin(const autofill::PasswordForm& form);

  
  
  
  bool RemoveLoginsCreatedBetween(const base::Time delete_begin,
                                  const base::Time delete_end);

  
  
  
  bool GetLogins(const autofill::PasswordForm& form,
                 std::vector<autofill::PasswordForm*>* forms) const;

  
  
  
  bool GetLoginsCreatedBetween(
      const base::Time begin,
      const base::Time end,
      std::vector<autofill::PasswordForm*>* forms) const;

  
  
  bool GetAutofillableLogins(
      std::vector<autofill::PasswordForm*>* forms) const;

  
  bool GetBlacklistLogins(
      std::vector<autofill::PasswordForm*>* forms) const;

  
  
  
  
  
  bool DeleteAndRecreateDatabaseFile();

 private:
  friend class LoginDatabaseTest;

  
  enum EncryptionResult {
    
    ENCRYPTION_RESULT_SUCCESS,
    
    
    
    ENCRYPTION_RESULT_ITEM_FAILURE,
    
    
    
    ENCRYPTION_RESULT_SERVICE_FAILURE,
  };

  
  
  
  
  EncryptionResult EncryptedString(const base::string16& plain_text,
                                   std::string* cipher_text) const;

  
  
  
  
  EncryptionResult DecryptedString(const std::string& cipher_text,
                                   base::string16* plain_text) const;

  bool InitLoginsTable();
  bool MigrateOldVersionsAsNeeded();

  
  
  
  
  EncryptionResult InitPasswordFormFromStatement(autofill::PasswordForm* form,
                                                 sql::Statement& s) const;

  
  
  bool GetAllLoginsWithBlacklistSetting(
      bool blacklisted, std::vector<autofill::PasswordForm*>* forms) const;

  
  Pickle SerializeVector(const std::vector<base::string16>& vec) const;
  std::vector<base::string16> DeserializeVector(const Pickle& pickle) const;

  base::FilePath db_path_;
  mutable sql::Connection db_;
  sql::MetaTable meta_table_;

  PSLMatchingHelper psl_helper_;

  DISALLOW_COPY_AND_ASSIGN(LoginDatabase);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_MAC_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_MAC_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/threading/thread.h"
#include "chrome/browser/password_manager/login_database.h"
#include "chrome/browser/password_manager/password_store.h"

class MacKeychain;
class NotificationService;

class PasswordStoreMac : public PasswordStore {
 public:
  
  
  PasswordStoreMac(MacKeychain* keychain, LoginDatabase* login_db);

  
  virtual bool Init();

 protected:
  virtual ~PasswordStoreMac();

  
  virtual void ScheduleTask(Task* task);

 private:
  virtual void ReportMetricsImpl();
  virtual void AddLoginImpl(const webkit_glue::PasswordForm& form);
  virtual void UpdateLoginImpl(const webkit_glue::PasswordForm& form);
  virtual void RemoveLoginImpl(const webkit_glue::PasswordForm& form);
  virtual void RemoveLoginsCreatedBetweenImpl(const base::Time& delete_begin,
                                              const base::Time& delete_end);
  virtual void GetLoginsImpl(GetLoginsRequest* request,
                             const webkit_glue::PasswordForm& form);
  virtual void GetAutofillableLoginsImpl(GetLoginsRequest* request);
  virtual void GetBlacklistLoginsImpl(GetLoginsRequest* request);
  virtual bool FillAutofillableLogins(
      std::vector<webkit_glue::PasswordForm*>* forms);
  virtual bool FillBlacklistLogins(
      std::vector<webkit_glue::PasswordForm*>* forms);

  
  
  
  bool AddToKeychainIfNecessary(const webkit_glue::PasswordForm& form);

  
  
  bool DatabaseHasFormMatchingKeychainForm(
      const webkit_glue::PasswordForm& form);

  
  
  
  std::vector<webkit_glue::PasswordForm*> GetUnusedKeychainForms();

  
  void RemoveDatabaseForms(
      const std::vector<webkit_glue::PasswordForm*>& forms);

  
  void RemoveKeychainForms(
      const std::vector<webkit_glue::PasswordForm*>& forms);

  
  
  void CreateNotificationService();

  scoped_ptr<MacKeychain> keychain_;
  scoped_ptr<LoginDatabase> login_metadata_db_;

  
  scoped_ptr<base::Thread> thread_;

  
  
  scoped_ptr<NotificationService> notification_service_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreMac);
};

#endif  

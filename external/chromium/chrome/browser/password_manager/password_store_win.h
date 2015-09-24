// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_WIN_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_WIN_H_
#pragma once

#include "base/scoped_ptr.h"
#include "chrome/browser/password_manager/password_store_default.h"

class LoginDatabase;
class Profile;
class WebDataService;

namespace webkit_glue {
struct PasswordForm;
}

class PasswordStoreWin : public PasswordStoreDefault {
 public:
  
  PasswordStoreWin(LoginDatabase* login_database,
                   Profile* profile,
                   WebDataService* web_data_service);

 private:
  class DBHandler;

  virtual ~PasswordStoreWin();

  
  void ShutdownOnDBThread();

  virtual GetLoginsRequest* NewGetLoginsRequest(
      GetLoginsCallback* callback) OVERRIDE;

  
  virtual void Shutdown() OVERRIDE;
  virtual void ForwardLoginsResult(GetLoginsRequest* request) OVERRIDE;

  
  virtual void GetLoginsImpl(GetLoginsRequest* request,
                             const webkit_glue::PasswordForm& form) OVERRIDE;

  scoped_ptr<DBHandler> db_handler_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreWin);
};

#endif  

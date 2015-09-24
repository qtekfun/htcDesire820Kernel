// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_DELEGATE_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_MANAGER_DELEGATE_H_
#pragma once

namespace webkit_glue {
struct PasswordFormFillData;
}  

class PasswordFormManager;
class Profile;

class PasswordManagerDelegate {
 public:
  PasswordManagerDelegate() {}
  virtual ~PasswordManagerDelegate() {}

  
  
  
  virtual void FillPasswordForm(
      const webkit_glue::PasswordFormFillData& form_data) = 0;

  
  virtual void AddSavePasswordInfoBar(PasswordFormManager* form_to_save) = 0;

  
  virtual Profile* GetProfileForPasswordManager() = 0;

  
  
  virtual bool DidLastPageLoadEncounterSSLErrors() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(PasswordManagerDelegate);
};


#endif  

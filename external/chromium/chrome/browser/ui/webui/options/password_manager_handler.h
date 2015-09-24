// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_PASSWORD_MANAGER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_PASSWORD_MANAGER_HANDLER_H_

#include <string>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "chrome/browser/password_manager/password_store.h"
#include "chrome/browser/password_manager/password_store_consumer.h"
#include "chrome/browser/prefs/pref_member.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class PasswordManagerHandler : public OptionsPageUIHandler,
                               public PasswordStore::Observer {
 public:
  PasswordManagerHandler();
  virtual ~PasswordManagerHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void Initialize();
  virtual void RegisterMessages();

  
  virtual void OnLoginsChanged();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  
  PasswordStore* GetPasswordStore();

  
  void UpdatePasswordLists(const ListValue* args);

  
  
  void RemoveSavedPassword(const ListValue* args);

  
  
  void RemovePasswordException(const ListValue* args);

  
  void RemoveAllSavedPasswords(const ListValue* args);

  
  void RemoveAllPasswordExceptions(const ListValue* args);

  
  
  void ShowSelectedPassword(const ListValue* args);

  
  
  void SetPasswordList();
  void SetPasswordExceptionList();

  
  class ListPopulater : public PasswordStoreConsumer {
   public:
    explicit ListPopulater(PasswordManagerHandler* page);
    virtual ~ListPopulater();

    
    virtual void Populate() = 0;

    
    virtual void OnPasswordStoreRequestDone(
        CancelableRequestProvider::Handle handle,
        const std::vector<webkit_glue::PasswordForm*>& result) = 0;

   protected:
    PasswordManagerHandler* page_;
    CancelableRequestProvider::Handle pending_login_query_;
  };

  
  class PasswordListPopulater : public ListPopulater {
   public:
    explicit PasswordListPopulater(PasswordManagerHandler* page);

    
    virtual void Populate();

    
    virtual void OnPasswordStoreRequestDone(
        CancelableRequestProvider::Handle handle,
        const std::vector<webkit_glue::PasswordForm*>& result);
  };

  
  class PasswordExceptionListPopulater : public ListPopulater {
   public:
    explicit PasswordExceptionListPopulater(PasswordManagerHandler* page);

    
    virtual void Populate();

    
    virtual void OnPasswordStoreRequestDone(
        CancelableRequestProvider::Handle handle,
        const std::vector<webkit_glue::PasswordForm*>& result);
  };

  
  PasswordListPopulater populater_;
  PasswordExceptionListPopulater exception_populater_;

  ScopedVector<webkit_glue::PasswordForm> password_list_;
  ScopedVector<webkit_glue::PasswordForm> password_exception_list_;

  
  std::string languages_;

  
  BooleanPrefMember show_passwords_;

  DISALLOW_COPY_AND_ASSIGN(PasswordManagerHandler);
};

#endif  

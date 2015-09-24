// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_MANAGED_USER_IMPORT_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_MANAGED_USER_IMPORT_HANDLER_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace base {
class DictionaryValue;
class ListValue;
}

namespace options {

class ManagedUserImportHandler : public OptionsPageUIHandler {
 public:
  ManagedUserImportHandler();
  virtual ~ManagedUserImportHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void InitializeHandler() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;
 private:
  
  
  
  
  void RequestManagedUserImportUpdate(const base::ListValue* args);

  
  
  
  
  
  
  
  
  
  
  void SendExistingManagedUsers(const base::DictionaryValue* dict);

  
  
  void ClearManagedUsersAndShowError();

  bool IsAccountConnected() const;
  bool HasAuthError() const;

  base::WeakPtrFactory<ManagedUserImportHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ManagedUserImportHandler);
};

}  

#endif  

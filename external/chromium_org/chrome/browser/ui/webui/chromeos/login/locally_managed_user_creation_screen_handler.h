// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_LOCALLY_MANAGED_USER_CREATION_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_LOCALLY_MANAGED_USER_CREATION_SCREEN_HANDLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "chrome/browser/chromeos/login/default_user_images.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"
#include "content/public/browser/web_ui.h"

namespace base {
class ListValue;
}

namespace chromeos {

class LocallyManagedUserCreationScreenHandler : public BaseScreenHandler {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    virtual void OnActorDestroyed(
        LocallyManagedUserCreationScreenHandler* actor) = 0;

    
    
    virtual void AuthenticateManager(const std::string& manager_id,
                                     const std::string& manager_password) = 0;

    
    
    virtual void CreateManagedUser(
        const base::string16& display_name,
        const std::string& managed_user_password) = 0;

    
    
    
    virtual bool FindUserByDisplayName(const base::string16& display_name,
                                       std::string *out_id) const = 0;

    
    virtual void ImportManagedUser(const std::string& user_id) = 0;
    
    
    virtual void ImportManagedUserWithPassword(const std::string& user_id,
                                               const std::string& password) = 0;

    virtual void AbortFlow() = 0;
    virtual void FinishFlow() = 0;

    virtual void CheckCameraPresence() = 0;
    virtual void OnPhotoTaken(const std::string& raw_data) = 0;
    virtual void OnImageSelected(const std::string& image_url,
                                 const std::string& image_type) = 0;
    virtual void OnImageAccepted() = 0;
    virtual void OnPageSelected(const std::string& page) = 0;
  };

  LocallyManagedUserCreationScreenHandler();
  virtual ~LocallyManagedUserCreationScreenHandler();

  virtual void PrepareToShow();
  virtual void Show();
  virtual void Hide();
  virtual void SetDelegate(Delegate* delegate);

  void ShowManagerPasswordError();

  void ShowIntroPage();
  void ShowManagerSelectionPage();
  void ShowUsernamePage();

  
  
  void ShowStatusMessage(bool is_progress, const base::string16& message);
  void ShowTutorialPage();

  void ShowErrorPage(const base::string16& title,
                     const base::string16& message,
                     const base::string16& button_text);

  
  void ShowPage(const std::string& page);

  void SetCameraPresent(bool enabled);

  void ShowExistingManagedUsers(const base::ListValue* users);

  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;
  virtual void Initialize() OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  void HandleCheckLocallyManagedUserName(const base::string16& name);

  void HandleManagerSelected(const std::string& manager_id);
  void HandleImportUserSelected(const std::string& user_id);

  void HandleFinishLocalManagedUserCreation();
  void HandleAbortLocalManagedUserCreation();
  void HandleRetryLocalManagedUserCreation(const base::ListValue* args);
  void HandleCurrentSupervisedUserPage(const std::string& current_page);

  void HandleAuthenticateManager(const std::string& raw_manager_username,
                                 const std::string& manager_password);
  void HandleCreateManagedUser(const base::string16& new_raw_user_name,
                               const std::string& new_user_password);
  void HandleImportSupervisedUser(const std::string& user_id);
  void HandleImportSupervisedUserWithPassword(const std::string& user_id,
                                              const std::string& password);

  void HandleGetImages();
  void HandlePhotoTaken(const std::string& image_url);
  void HandleCheckCameraPresence();
  void HandleSelectImage(const std::string& image_url,
                         const std::string& image_type);

  void UpdateText(const std::string& element_id, const base::string16& text);

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(LocallyManagedUserCreationScreenHandler);
};

}  

#endif  

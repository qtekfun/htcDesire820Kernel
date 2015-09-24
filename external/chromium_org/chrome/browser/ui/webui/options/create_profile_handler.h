// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_CREATE_PROFILE_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_CREATE_PROFILE_HANDLER_H_

#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_window.h"
#include "chrome/browser/ui/host_desktop.h"
#include "chrome/browser/ui/webui/options/options_ui.h"
#include "google_apis/gaia/google_service_auth_error.h"


namespace base {
class DictionaryValue;
class ListValue;
}

class ManagedUserRegistrationUtility;

namespace options {

class CreateProfileHandler: public OptionsPageUIHandler {
 public:
  CreateProfileHandler();
  virtual ~CreateProfileHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;

  
  virtual void RegisterMessages() OVERRIDE;

 private:
  
  
  enum ProfileCreationStatus {
    PROFILE_CREATION_SUCCESS,
    PROFILE_CREATION_ERROR,
  };

  
  
  
  enum ProfileCreationErrorType {
    REMOTE_ERROR,
    LOCAL_ERROR,
    SIGNIN_ERROR
  };

  
  
  
  enum ProfileCreationOperationType {
    SUPERVISED_PROFILE_CREATION,
    SUPERVISED_PROFILE_IMPORT,
    NON_SUPERVISED_PROFILE_CREATION,
    NO_CREATION_IN_PROGRESS
  };

  
  
  
  
  
  
  
  
  void CreateProfile(const base::ListValue* args);

  
  
  
  
  
  void OnProfileCreated(bool create_shortcut,
                        chrome::HostDesktopType desktop_type,
                        const std::string& managed_user_id,
                        Profile* profile,
                        Profile::CreateStatus status);

  void HandleProfileCreationSuccess(bool create_shortcut,
                                    chrome::HostDesktopType desktop_type,
                                    const std::string& managed_user_id,
                                    Profile* profile);

  
  
  void RegisterManagedUser(bool create_shortcut,
                           chrome::HostDesktopType desktop_type,
                           const std::string& managed_user_id,
                           Profile* new_profile);

  
  void OnManagedUserRegistered(bool create_shortcut,
                               chrome::HostDesktopType desktop_type,
                               Profile* profile,
                               const GoogleServiceAuthError& error);

  
  
  void CreateShortcutAndShowSuccess(bool create_shortcut,
                                    chrome::HostDesktopType desktop_type,
                                    Profile* profile);

  
  void ShowProfileCreationError(Profile* profile, const base::string16& error);

  
  void ShowProfileCreationWarning(const base::string16& warning);

  
  
  
  
  void HandleCancelProfileCreation(const base::ListValue* args);

  
  
  
  
  void CancelProfileRegistration(bool user_initiated);

  
  void RecordProfileCreationMetrics(Profile::CreateStatus status);

  
  
  void RecordSupervisedProfileCreationMetrics(
      GoogleServiceAuthError::State error_state);

  base::string16 GetProfileCreationErrorMessage(
      ProfileCreationErrorType error) const;
  std::string GetJavascriptMethodName(ProfileCreationStatus status) const;

  bool IsValidExistingManagedUserId(
      const std::string& existing_managed_user_id) const;

  
  
  
  base::FilePath profile_path_being_created_;

  
  base::TimeTicks profile_creation_start_time_;

  scoped_ptr<ManagedUserRegistrationUtility> managed_user_registration_utility_;

  
  
  ProfileCreationOperationType profile_creation_type_;

  base::WeakPtrFactory<CreateProfileHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(CreateProfileHandler);
};

}  

#endif  

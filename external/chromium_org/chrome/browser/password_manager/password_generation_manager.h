// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_GENERATION_MANAGER_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_GENERATION_MANAGER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace autofill {
struct FormData;
class FormStructure;
class PasswordGenerator;
struct PasswordForm;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class PasswordGenerationManager
    : public content::WebContentsObserver,
      public content::WebContentsUserData<PasswordGenerationManager> {
 public:
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
  virtual ~PasswordGenerationManager();

  
  
  
  void DetectAccountCreationForms(
      const std::vector<autofill::FormStructure*>& forms);

 protected:
  explicit PasswordGenerationManager(content::WebContents* contents);

 private:
  friend class content::WebContentsUserData<PasswordGenerationManager>;
  friend class PasswordGenerationManagerTest;

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  bool IsGenerationEnabled() const;

  
  
  virtual void SendAccountCreationFormsToRenderer(
      content::RenderViewHost* host,
      const std::vector<autofill::FormData>& forms);

  
  
  
  void OnShowPasswordGenerationPopup(const gfx::Rect& icon_bounds,
                                     int max_length,
                                     const autofill::PasswordForm& form);

  
  scoped_ptr<autofill::PasswordGenerator> password_generator_;

  DISALLOW_COPY_AND_ASSIGN(PasswordGenerationManager);
};

#endif  

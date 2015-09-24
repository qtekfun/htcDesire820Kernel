// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_AUTOMATIC_PROFILE_RESETTER_DELEGATE_H_
#define CHROME_BROWSER_PROFILE_RESETTER_AUTOMATIC_PROFILE_RESETTER_DELEGATE_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/profile_resetter/profile_resetter.h"
#include "chrome/browser/search_engines/template_url_service_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/one_shot_event.h"

class BrandcodeConfigFetcher;
class GlobalErrorService;
class Profile;
class ResettableSettingsSnapshot;
class TemplateURLService;

namespace base {
class DictionaryValue;
class ListValue;
}

class AutomaticProfileResetterDelegate {
 public:
  virtual ~AutomaticProfileResetterDelegate() {}

  
  
  virtual void EnumerateLoadedModulesIfNeeded() = 0;

  
  
  virtual void RequestCallbackWhenLoadedModulesAreEnumerated(
      const base::Closure& ready_callback) const = 0;

  
  virtual void LoadTemplateURLServiceIfNeeded() = 0;

  
  
  virtual void RequestCallbackWhenTemplateURLServiceIsLoaded(
      const base::Closure& ready_callback) const = 0;

  
  
  
  virtual void FetchBrandcodedDefaultSettingsIfNeeded() = 0;

  
  
  
  virtual void RequestCallbackWhenBrandcodedDefaultsAreFetched(
      const base::Closure& ready_callback) const = 0;

  
  virtual scoped_ptr<base::ListValue> GetLoadedModuleNameDigests() const = 0;

  
  
  
  
  
  
  
  
  
  
  
  virtual scoped_ptr<base::DictionaryValue>
      GetDefaultSearchProviderDetails() const = 0;

  
  virtual bool IsDefaultSearchProviderManaged() const = 0;

  
  
  virtual scoped_ptr<base::ListValue>
      GetPrepopulatedSearchProvidersDetails() const = 0;

  
  
  
  
  
  
  
  virtual bool TriggerPrompt() = 0;

  
  
  
  
  
  virtual void TriggerProfileSettingsReset(bool send_feedback,
                                           const base::Closure& completion) = 0;

  
  
  virtual void DismissPrompt() = 0;
};

class AutomaticProfileResetterDelegateImpl
    : public AutomaticProfileResetterDelegate,
      public base::SupportsWeakPtr<AutomaticProfileResetterDelegateImpl>,
      public TemplateURLServiceObserver,
      public content::NotificationObserver {
 public:
  explicit AutomaticProfileResetterDelegateImpl(
      Profile* profile,
      ProfileResetter::ResettableFlags resettable_aspects);
  virtual ~AutomaticProfileResetterDelegateImpl();

  
  
  
  const BrandcodedDefaultSettings* brandcoded_defaults() const {
    return brandcoded_defaults_.get();
  }

  
  virtual void EnumerateLoadedModulesIfNeeded() OVERRIDE;
  virtual void RequestCallbackWhenLoadedModulesAreEnumerated(
      const base::Closure& ready_callback) const OVERRIDE;
  virtual void LoadTemplateURLServiceIfNeeded() OVERRIDE;
  virtual void RequestCallbackWhenTemplateURLServiceIsLoaded(
      const base::Closure& ready_callback) const OVERRIDE;
  virtual void FetchBrandcodedDefaultSettingsIfNeeded() OVERRIDE;
  virtual void RequestCallbackWhenBrandcodedDefaultsAreFetched(
      const base::Closure& ready_callback) const OVERRIDE;
  virtual scoped_ptr<base::ListValue>
      GetLoadedModuleNameDigests() const OVERRIDE;
  virtual scoped_ptr<base::DictionaryValue>
      GetDefaultSearchProviderDetails() const OVERRIDE;
  virtual bool IsDefaultSearchProviderManaged() const OVERRIDE;
  virtual scoped_ptr<base::ListValue>
      GetPrepopulatedSearchProvidersDetails() const OVERRIDE;
  virtual bool TriggerPrompt() OVERRIDE;
  virtual void TriggerProfileSettingsReset(
      bool send_feedback,
      const base::Closure& completion) OVERRIDE;
  virtual void DismissPrompt() OVERRIDE;

  
  virtual void OnTemplateURLServiceChanged() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  
  
  virtual void SendFeedback(const std::string& report) const;

  
  
  
  void RunProfileSettingsReset(bool send_feedback,
                               const base::Closure& completion);

  
  
  void OnBrandcodedDefaultsFetched();

  
  
  
  
  void OnProfileSettingsResetCompleted(
      const base::Closure& user_callback,
      scoped_ptr<ResettableSettingsSnapshot> old_settings_snapshot);

  Profile* profile_;
  GlobalErrorService* global_error_service_;
  TemplateURLService* template_url_service_;

  scoped_ptr<BrandcodeConfigFetcher> brandcoded_config_fetcher_;
  scoped_ptr<BrandcodedDefaultSettings> brandcoded_defaults_;

  const ProfileResetter::ResettableFlags resettable_aspects_;
  scoped_ptr<ProfileResetter> profile_resetter_;

  content::NotificationRegistrar registrar_;

  
  
  scoped_ptr<base::ListValue> module_list_;

  
  
  
  
  
  
  extensions::OneShotEvent modules_have_been_enumerated_event_;

  
  
  
  extensions::OneShotEvent template_url_service_ready_event_;

  
  
  extensions::OneShotEvent brandcoded_defaults_fetched_event_;

  DISALLOW_COPY_AND_ASSIGN(AutomaticProfileResetterDelegateImpl);
};

#endif  

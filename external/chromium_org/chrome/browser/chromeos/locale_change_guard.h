// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOCALE_CHANGE_GUARD_H_
#define CHROME_BROWSER_CHROMEOS_LOCALE_CHANGE_GUARD_H_

#include <string>

#include "ash/system/locale/locale_observer.h"
#include "base/compiler_specific.h"
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_types.h"

class Profile;

namespace base {
class ListValue;
}

namespace chromeos {

class LocaleChangeGuard : public content::NotificationObserver,
                          public ash::LocaleObserver::Delegate,
                          public base::SupportsWeakPtr<LocaleChangeGuard> {
 public:
  explicit LocaleChangeGuard(Profile* profile);
  virtual ~LocaleChangeGuard();

  
  virtual void AcceptLocaleChange() OVERRIDE;
  virtual void RevertLocaleChange() OVERRIDE;

  
  void PrepareChangingLocale(
      const std::string& from_locale, const std::string& to_locale);

  
  void OnLogin();

 private:
  class Delegate;

  void RevertLocaleChangeCallback(const base::ListValue* list);
  void Check();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  std::string from_locale_;
  std::string to_locale_;
  Profile* profile_;
  bool reverted_;
  bool session_started_;
  bool main_frame_loaded_;
  content::NotificationRegistrar registrar_;

  
  
  
  base::string16 title_text_;
  base::string16 message_text_;
  base::string16 revert_link_text_;
};

}  

#endif  

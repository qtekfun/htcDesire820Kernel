// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_ACCEPT_LANGUAGES_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_ACCEPT_LANGUAGES_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class PrefChangeRegistrar;
class PrefService;
class Profile;

class TranslateAcceptLanguages : public content::NotificationObserver {
 public:
  TranslateAcceptLanguages();
  virtual ~TranslateAcceptLanguages();

  
  
  static bool CanBeAcceptLanguage(const std::string& language);

  
  
  
  bool IsAcceptLanguage(Profile* profile,
                        const std::string& language);

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void InitAcceptLanguages(PrefService* prefs);

  
  typedef std::set<std::string> LanguageSet;
  typedef std::map<PrefService*, LanguageSet> PrefServiceLanguagesMap;
  PrefServiceLanguagesMap accept_languages_;

  
  
  typedef std::map<PrefService*, PrefChangeRegistrar*> PrefServiceRegistrarMap;
  PrefServiceRegistrarMap pref_change_registrars_;

  content::NotificationRegistrar notification_registrar_;

  DISALLOW_COPY_AND_ASSIGN(TranslateAcceptLanguages);
};

#endif  

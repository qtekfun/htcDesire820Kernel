// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_MANAGER_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_MANAGER_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/lazy_instance.h"
#include "base/task.h"
#include "chrome/browser/prefs/pref_change_registrar.h"
#include "chrome/common/net/url_fetcher.h"
#include "chrome/common/translate_errors.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

template <typename T> struct DefaultSingletonTraits;
class GURL;
struct PageTranslatedDetails;
class PrefService;
class TabContents;
class TranslateInfoBarDelegate;


class TranslateManager : public NotificationObserver,
                         public URLFetcher::Delegate {
 public:
  
  static TranslateManager* GetInstance();

  virtual ~TranslateManager();

  
  
  
  void TranslatePage(TabContents* tab_contents,
                     const std::string& source_lang,
                     const std::string& target_lang);

  
  
  void RevertTranslation(TabContents* tab_contents);

  
  
  
  void ReportLanguageDetectionError(TabContents* tab_contents);

  
  void ClearTranslateScript() { translate_script_.clear(); }

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  
  void set_translate_script_expiration_delay(int delay_ms) {
    translate_script_expiration_delay_ = delay_ms;
  }

  
  static bool IsShowingTranslateInfobar(TabContents* tab);

  
  static bool IsTranslatableURL(const GURL& url);

  
  
  static void GetSupportedLanguages(std::vector<std::string>* languages);

  
  
  static std::string GetLanguageCode(const std::string& chrome_locale);

  
  static bool IsSupportedLanguage(const std::string& language);

 protected:
  TranslateManager();

 private:
  friend struct DefaultSingletonTraits<TranslateManager>;

  
  
  
  struct PendingRequest {
    int render_process_id;
    int render_view_id;
    int page_id;
    std::string source_lang;
    std::string target_lang;
  };

  
  
  void InitiateTranslation(TabContents* tab, const std::string& page_lang);

  
  
  void InitiateTranslationPosted(int process_id,
                                 int render_id,
                                 const std::string& page_lang);

  
  void DoTranslatePage(TabContents* tab_contents,
                       const std::string& translate_script,
                       const std::string& source_lang,
                       const std::string& target_lang);

   
   void PageTranslated(TabContents* tab, PageTranslatedDetails* details);

  
  
  bool IsAcceptLanguage(TabContents* tab, const std::string& language);

  
  
  void InitAcceptLanguages(PrefService* prefs);

  
  
  void RequestTranslateScript();

  
  
  void ShowInfoBar(TabContents* tab, TranslateInfoBarDelegate* infobar);

  
  
  
  static std::string GetTargetLanguage();

  
  static TranslateInfoBarDelegate* GetTranslateInfoBarDelegate(
      TabContents* tab);

  NotificationRegistrar notification_registrar_;
  PrefChangeRegistrar pref_change_registrar_;

  
  typedef std::set<std::string> LanguageSet;
  typedef std::map<PrefService*, LanguageSet> PrefServiceLanguagesMap;
  PrefServiceLanguagesMap accept_languages_;

  ScopedRunnableMethodFactory<TranslateManager> method_factory_;

  
  std::string translate_script_;

  
  
  int translate_script_expiration_delay_;

  
  bool translate_script_request_pending_;

  
  
  
  std::vector<PendingRequest> pending_requests_;

  
  static base::LazyInstance<std::set<std::string> > supported_languages_;

  DISALLOW_COPY_AND_ASSIGN(TranslateManager);
};

#endif  

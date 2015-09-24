// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRANSLATE_TRANSLATE_MANAGER_H_
#define CHROME_BROWSER_TRANSLATE_TRANSLATE_MANAGER_H_

#include <map>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "chrome/browser/ui/translate/translate_bubble_model.h"
#include "chrome/common/translate/translate_errors.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

template <typename T> struct DefaultSingletonTraits;
class GURL;
struct LanguageDetectionDetails;
struct PageTranslatedDetails;
class PrefService;
class Profile;
struct ShortcutConfiguration;
class TranslateAcceptLanguages;
struct TranslateErrorDetails;
struct TranslateEventDetails;
class TranslateInfoBarDelegate;
class TranslateLanguageList;
class TranslateScript;

namespace content {
class WebContents;
}

namespace net {
class URLFetcher;
}


class TranslateManager : public content::NotificationObserver {
 public:
  
  static TranslateManager* GetInstance();

  virtual ~TranslateManager();

  
  static bool IsTranslatableURL(const GURL& url);

  
  
  static void GetSupportedLanguages(std::vector<std::string>* languages);

  
  
  static base::Time GetSupportedLanguagesLastUpdated();

  
  
  static std::string GetLanguageCode(const std::string& chrome_locale);

  
  static bool IsSupportedLanguage(const std::string& language);

  
  
  static bool IsAlphaLanguage(const std::string& language);

  
  static bool IsAcceptLanguage(Profile* profile, const std::string& language);

  
  
  
  
  
  
  static std::string GetTargetLanguage(PrefService* prefs);

  
  
  static std::string GetAutoTargetLanguage(const std::string& original_language,
                                           PrefService* prefs);

  
  static bool IsTranslateBubbleEnabled();

  
  
  
  void FetchLanguageListFromTranslateServer(PrefService* prefs);

  
  
  void CleanupPendingUlrFetcher();

  
  
  
  void TranslatePage(content::WebContents* web_contents,
                     const std::string& source_lang,
                     const std::string& target_lang);

  
  
  void RevertTranslation(content::WebContents* web_contents);

  
  
  
  void ReportLanguageDetectionError(content::WebContents* web_contents);

  
  void ClearTranslateScript();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  void SetTranslateScriptExpirationDelay(int delay_ms);

  
  void set_translate_max_reload_attemps(int attempts) {
    max_reload_check_attempts_ = attempts;
  }

  
  class Observer {
   public:
    virtual void OnLanguageDetection(
        const LanguageDetectionDetails& details) = 0;
    virtual void OnTranslateError(
        const TranslateErrorDetails& details) = 0;
    virtual void OnTranslateEvent(
        const TranslateEventDetails& details) = 0;
  };

  
  void AddObserver(Observer* obs);
  void RemoveObserver(Observer* obs);

  
  void NotifyTranslateEvent(const TranslateEventDetails& details);

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

  
  
  void InitiateTranslation(content::WebContents* web_contents,
                           const std::string& page_lang);

  
  
  void InitiateTranslationPosted(int process_id, int render_id,
                                 const std::string& page_lang, int attempt);

  
  void DoTranslatePage(content::WebContents* web_contents,
                       const std::string& translate_script,
                       const std::string& source_lang,
                       const std::string& target_lang);

  
  void PageTranslated(content::WebContents* web_contents,
                      PageTranslatedDetails* details);

  void OnTranslateScriptFetchComplete(bool success, const std::string& data);

  
  void NotifyLanguageDetection(const LanguageDetectionDetails& details);

  
  void NotifyTranslateError(const TranslateErrorDetails& details);

  
  void ShowBubble(content::WebContents* web_contents,
                  TranslateBubbleModel::ViewState view_state,
                  TranslateErrors::Type error_type);

  
  
  static ShortcutConfiguration ShortcutConfig();

  content::NotificationRegistrar notification_registrar_;

  
  int max_reload_check_attempts_;

  
  
  
  std::vector<PendingRequest> pending_requests_;

  
  ObserverList<Observer> observer_list_;

  
  scoped_ptr<TranslateLanguageList> language_list_;

  
  
  scoped_ptr<TranslateScript> script_;

  
  
  scoped_ptr<TranslateAcceptLanguages> accept_languages_;

  base::WeakPtrFactory<TranslateManager> weak_method_factory_;

  DISALLOW_COPY_AND_ASSIGN(TranslateManager);
};

#endif  

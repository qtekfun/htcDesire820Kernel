// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_TRANSLATE_TRANSLATE_HELPER_H_
#define CHROME_RENDERER_TRANSLATE_TRANSLATE_HELPER_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/common/translate/translate_errors.h"
#include "content/public/renderer/render_view_observer.h"

namespace blink {
class WebDocument;
class WebFrame;
}


class TranslateHelper : public content::RenderViewObserver {
 public:
  explicit TranslateHelper(content::RenderView* render_view);
  virtual ~TranslateHelper();

  
  void PageCaptured(int page_id, const base::string16& contents);

 protected:
  
  
  void OnTranslatePage(int page_id,
                       const std::string& translate_script,
                       const std::string& source_lang,
                       const std::string& target_lang);
  void OnRevertTranslation(int page_id);

  
  
  virtual bool IsTranslateLibAvailable();

  
  virtual bool IsTranslateLibReady();

  
  virtual bool HasTranslationFinished();

  
  
  virtual bool HasTranslationFailed();

  
  
  
  virtual bool StartTranslation();

  
  
  
  virtual std::string GetOriginalPageLanguage();

  
  
  virtual base::TimeDelta AdjustDelay(int delayInMs);

  
  virtual void ExecuteScript(const std::string& script);

  
  
  
  virtual bool ExecuteScriptAndGetBoolResult(const std::string& script,
                                             bool fallback);

  
  
  
  virtual std::string ExecuteScriptAndGetStringResult(
      const std::string& script);

  
  
  
  virtual double ExecuteScriptAndGetDoubleResult(const std::string& script);

 private:
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest, AdoptHtmlLang);
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest,
                           CLDAgreeWithLanguageCodeHavingCountryCode);
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest,
                           CLDDisagreeWithWrongLanguageCode);
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest,
                           InvalidLanguageMetaTagProviding);
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest, LanguageCodeTypoCorrection);
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest, LanguageCodeSynonyms);
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest, ResetInvalidLanguageCode);
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest, SimilarLanguageCode);
  FRIEND_TEST_ALL_PREFIXES(TranslateHelperTest, WellKnownWrongConfiguration);

  
  static void ConvertLanguageCodeSynonym(std::string* code);

  
  
  
  static bool IsTranslationAllowed(blink::WebDocument* document);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  
  void CancelPendingTranslation();

  
  
  
  void CheckTranslateStatus();

  
  
  void TranslatePageImpl(int count);

  
  
  void NotifyBrowserTranslationFailed(TranslateErrors::Type error);

  
  
  blink::WebFrame* GetMainFrame();

  
  
  int page_id_;

  
  bool translation_pending_;
  std::string source_lang_;
  std::string target_lang_;

  
  
  base::TimeTicks language_determined_time_;

  
  base::WeakPtrFactory<TranslateHelper> weak_method_factory_;

  DISALLOW_COPY_AND_ASSIGN(TranslateHelper);
};

#endif  

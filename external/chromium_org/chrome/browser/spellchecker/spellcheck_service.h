// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_SERVICE_H_
#define CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_SERVICE_H_

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/spellchecker/feedback_sender.h"
#include "chrome/browser/spellchecker/spellcheck_custom_dictionary.h"
#include "chrome/browser/spellchecker/spellcheck_hunspell_dictionary.h"
#include "chrome/common/spellcheck_common.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class PrefService;
class SpellCheckHostMetrics;

namespace base {
class WaitableEvent;
}

namespace content {
class RenderProcessHost;
class BrowserContext;
}

class SpellcheckService : public BrowserContextKeyedService,
                          public content::NotificationObserver,
                          public SpellcheckCustomDictionary::Observer,
                          public SpellcheckHunspellDictionary::Observer {
 public:
  
  
  enum EventType {
    BDICT_NOTINITIALIZED,
    BDICT_CORRUPTED,
  };

  
  enum DictionaryFormat {
    DICT_HUNSPELL,
    DICT_TEXT,
    DICT_UNKNOWN,
  };

  explicit SpellcheckService(content::BrowserContext* context);
  virtual ~SpellcheckService();

  
  
  
  
  
  static int GetSpellCheckLanguages(content::BrowserContext* context,
                                    std::vector<std::string>* languages);

  
  
  static void GetSpellCheckLanguagesFromAcceptLanguages(
      const std::vector<std::string>& accept_languages,
      const std::string& dictionary_language,
      std::vector<std::string>* languages);

  
  
  
  
  static bool SignalStatusEvent(EventType type);

  
  
  void StartRecordingMetrics(bool spellcheck_enabled);

  
  
  void InitForRenderer(content::RenderProcessHost* process);

  
  
  SpellCheckHostMetrics* GetMetrics() const;

  
  SpellcheckCustomDictionary* GetCustomDictionary();

  
  SpellcheckHunspellDictionary* GetHunspellDictionary();

  
  spellcheck::FeedbackSender* GetFeedbackSender();

  
  
  bool LoadExternalDictionary(std::string language,
                              std::string locale,
                              std::string path,
                              DictionaryFormat format);

  
  
  bool UnloadExternalDictionary(std::string path);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnCustomDictionaryLoaded() OVERRIDE;
  virtual void OnCustomDictionaryChanged(
      const SpellcheckCustomDictionary::Change& dictionary_change) OVERRIDE;

  
  virtual void OnHunspellDictionaryInitialized() OVERRIDE;
  virtual void OnHunspellDictionaryDownloadBegin() OVERRIDE;
  virtual void OnHunspellDictionaryDownloadSuccess() OVERRIDE;
  virtual void OnHunspellDictionaryDownloadFailure() OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(SpellcheckServiceBrowserTest, DeleteCorruptedBDICT);

  
  static void AttachStatusEvent(base::WaitableEvent* status_event);

  
  static EventType GetStatusEvent();

  
  void InitForAllRenderers();

  
  
  void OnEnableAutoSpellCorrectChanged();

  
  
  void OnSpellCheckDictionaryChanged();

  
  void OnUseSpellingServiceChanged();

  
  
  void UpdateFeedbackSenderState();

  PrefChangeRegistrar pref_change_registrar_;
  content::NotificationRegistrar registrar_;

  
  content::BrowserContext* context_;

  scoped_ptr<SpellCheckHostMetrics> metrics_;

  scoped_ptr<SpellcheckCustomDictionary> custom_dictionary_;

  scoped_ptr<SpellcheckHunspellDictionary> hunspell_dictionary_;

  scoped_ptr<spellcheck::FeedbackSender> feedback_sender_;

  base::WeakPtrFactory<SpellcheckService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SpellcheckService);
};

#endif  

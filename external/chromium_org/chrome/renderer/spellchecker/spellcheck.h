// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_H_
#define CHROME_RENDERER_SPELLCHECKER_SPELLCHECK_H_

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "base/strings/string16.h"
#include "chrome/renderer/spellchecker/custom_dictionary_engine.h"
#include "chrome/renderer/spellchecker/spellcheck_language.h"
#include "content/public/renderer/render_process_observer.h"
#include "ipc/ipc_platform_file.h"
#include "third_party/WebKit/public/platform/WebVector.h"

struct SpellCheckResult;

namespace blink {
class WebTextCheckingCompletion;
struct WebTextCheckingResult;
}

class SpellCheck : public content::RenderProcessObserver,
                   public base::SupportsWeakPtr<SpellCheck> {
 public:
  
  class SpellcheckRequest;
  enum ResultFilter {
    DO_NOT_MODIFY = 1,  
    USE_NATIVE_CHECKER,  
  };

  SpellCheck();
  virtual ~SpellCheck();

  
  void Init(base::PlatformFile file,
            const std::set<std::string>& custom_words,
            const std::string& language);

  
  
  
  
  
  
  bool InitializeIfNeeded();

  
  
  
  
  
  
  
  
  
  
  bool SpellCheckWord(const char16* in_word,
                      int in_word_len,
                      int tag,
                      int* misspelling_start,
                      int* misspelling_len,
                      std::vector<base::string16>* optional_suggestions);

  
  
  
  bool SpellCheckParagraph(
      const base::string16& text,
      blink::WebVector<blink::WebTextCheckingResult>* results);

  
  
  
  
  
  
  base::string16 GetAutoCorrectionWord(const base::string16& word, int tag);

  
  
#if !defined (OS_MACOSX)
  void RequestTextChecking(const base::string16& text,
                           blink::WebTextCheckingCompletion* completion);
#endif

  
  
  
  
  void CreateTextCheckingResults(
      ResultFilter filter,
      int line_offset,
      const base::string16& line_text,
      const std::vector<SpellCheckResult>& spellcheck_results,
      blink::WebVector<blink::WebTextCheckingResult>* textcheck_results);

  bool is_spellcheck_enabled() { return spellcheck_enabled_; }

 private:
   friend class SpellCheckTest;
   FRIEND_TEST_ALL_PREFIXES(SpellCheckTest, GetAutoCorrectionWord_EN_US);
   FRIEND_TEST_ALL_PREFIXES(SpellCheckTest,
       RequestSpellCheckMultipleTimesWithoutInitialization);

  
  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;

  
  void OnInit(IPC::PlatformFileForTransit bdict_file,
              const std::set<std::string>& custom_words,
              const std::string& language,
              bool auto_spell_correct);
  void OnCustomDictionaryChanged(
      const std::vector<std::string>& words_added,
      const std::vector<std::string>& words_removed);
  void OnEnableAutoSpellCorrect(bool enable);
  void OnEnableSpellCheck(bool enable);
  void OnRequestDocumentMarkers();

#if !defined (OS_MACOSX)
  
  
  void PostDelayedSpellCheckTask(SpellcheckRequest* request);

  
  void PerformSpellCheck(SpellcheckRequest* request);

  
  
  
  
  
  scoped_ptr<SpellcheckRequest> pending_request_param_;
#endif

  SpellcheckLanguage spellcheck_;  

  
  CustomDictionaryEngine custom_dictionary_;

  
  bool auto_spell_correct_turned_on_;

  
  bool spellcheck_enabled_;

  DISALLOW_COPY_AND_ASSIGN(SpellCheck);
};

#endif  

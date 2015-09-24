// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_MESSAGE_FILTER_H_
#define CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_MESSAGE_FILTER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/spellchecker/spelling_service_client.h"
#include "content/public/browser/browser_message_filter.h"

class SpellCheckMarker;
class SpellcheckService;
struct SpellCheckResult;

class SpellCheckMessageFilter : public content::BrowserMessageFilter {
 public:
  explicit SpellCheckMessageFilter(int render_process_id);

  
  virtual void OverrideThreadForMessage(
      const IPC::Message& message,
      content::BrowserThread::ID* thread) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;

 private:
  friend class TestingSpellCheckMessageFilter;

  virtual ~SpellCheckMessageFilter();

  void OnSpellCheckerRequestDictionary();
  void OnNotifyChecked(const base::string16& word, bool misspelled);
  void OnRespondDocumentMarkers(const std::vector<uint32>& markers);
#if !defined(OS_MACOSX)
  void OnCallSpellingService(int route_id,
                             int identifier,
                             const base::string16& text,
                             std::vector<SpellCheckMarker> markers);

  
  
  void OnTextCheckComplete(
      int route_id,
      int identifier,
      const std::vector<SpellCheckMarker>& markers,
      bool success,
      const base::string16& text,
      const std::vector<SpellCheckResult>& results);

  
  
  
  
  
  
  void CallSpellingService(
      const base::string16& text,
      int route_id,
      int identifier,
      const std::vector<SpellCheckMarker>& markers);
#endif

  
  virtual SpellcheckService* GetSpellcheckService() const;

  int render_process_id_;

  
  scoped_ptr<SpellingServiceClient> client_;
};

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_FEEDBACK_SENDER_H_
#define CHROME_BROWSER_SPELLCHECKER_FEEDBACK_SENDER_H_

#include <map>
#include <set>
#include <vector>

#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/spellchecker/feedback.h"
#include "chrome/browser/spellchecker/misspelling.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "url/gurl.h"

class SpellCheckMarker;
struct SpellCheckResult;

namespace net {
class URLFetcher;
class URLRequestContextGetter;
}

namespace spellcheck {

namespace {

const char kFeedbackFieldTrialName[] = "SpellingServiceFeedback";
const char kFeedbackFieldTrialEnabledGroupName[] = "Enabled";

}  

class FeedbackSender : public base::SupportsWeakPtr<FeedbackSender>,
                       public net::URLFetcherDelegate {
 public:
  
  
  FeedbackSender(net::URLRequestContextGetter* request_context,
                 const std::string& language,
                 const std::string& country);
  virtual ~FeedbackSender();

  
  
  void SelectedSuggestion(uint32 hash, int suggestion_index);

  
  
  void AddedToDictionary(uint32 hash);

  
  
  void IgnoredSuggestions(uint32 hash);

  
  
  
  void ManuallyCorrected(uint32 hash, const base::string16& correction);

  
  
  void RecordInDictionary(uint32 hash);

  
  
  
  
  
  
  void OnReceiveDocumentMarkers(int renderer_process_id,
                                const std::vector<uint32>& markers);

  
  
  
  
  void OnSpellcheckResults(int renderer_process_id,
                           const base::string16& text,
                           const std::vector<SpellCheckMarker>& markers,
                           std::vector<SpellCheckResult>* results);

  
  
  void OnLanguageCountryChange(const std::string& language,
                               const std::string& country);

  
  void StartFeedbackCollection();

  
  
  void StopFeedbackCollection();

 private:
  friend class FeedbackSenderTest;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  void RequestDocumentMarkers();

  
  
  void FlushFeedback();

  
  void SendFeedback(const std::vector<Misspelling>& feedback_data,
                    bool is_first_feedback_batch);

  
  scoped_refptr<net::URLRequestContextGetter> request_context_;

  
  const std::string api_version_;

  
  std::string language_;

  
  std::string country_;

  
  
  size_t misspelling_counter_;

  
  Feedback feedback_;

  
  
  std::set<int> renderers_sent_feedback_;

  
  base::Time session_start_;

  
  GURL feedback_service_url_;

  
  
  
  
  base::RepeatingTimer<FeedbackSender> timer_;

  
  
  
  ScopedVector<net::URLFetcher> senders_;

  DISALLOW_COPY_AND_ASSIGN(FeedbackSender);
};

}  

#endif  

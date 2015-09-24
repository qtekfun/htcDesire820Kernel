// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SAFE_BROWSING_PHISHING_TERM_FEATURE_EXTRACTOR_H_
#define CHROME_RENDERER_SAFE_BROWSING_PHISHING_TERM_FEATURE_EXTRACTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/containers/mru_cache.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/strings/string_piece.h"

namespace safe_browsing {
class FeatureExtractorClock;
class FeatureMap;

class PhishingTermFeatureExtractor {
 public:
  
  
  typedef base::Callback<void(bool)> DoneCallback;

  
  
  
  
  
  
  
  
  
  
  
  
  PhishingTermFeatureExtractor(
      const base::hash_set<std::string>* page_term_hashes,
      const base::hash_set<uint32>* page_word_hashes,
      size_t max_words_per_term,
      uint32 murmurhash3_seed,
      FeatureExtractorClock* clock);
  ~PhishingTermFeatureExtractor();

  
  
  
  
  
  
  
  
  
  
  
  
  
  void ExtractFeatures(const base::string16* page_text,
                       FeatureMap* features,
                       const DoneCallback& done_callback);

  
  
  
  void CancelPendingExtraction();

 private:
  struct ExtractionState;

  
  
  static const int kMaxTimePerChunkMs;

  
  
  
  static const int kClockCheckGranularity;

  
  
  static const int kMaxTotalTimeMs;

  
  
  static const int kMaxNegativeWordCacheSize;

  
  
  
  
  void ExtractFeaturesWithTimeout();

  
  void HandleWord(const base::StringPiece16& word);

  
  
  
  void CheckNoPendingExtraction();

  
  void RunCallback(bool success);

  
  void Clear();

  
  const base::hash_set<std::string>* page_term_hashes_;

  
  
  
  
  
  const base::hash_set<uint32>* page_word_hashes_;

  
  const size_t max_words_per_term_;

  
  const uint32 murmurhash3_seed_;

  
  
  
  
  typedef base::HashingMRUCache<base::StringPiece16, bool> WordCache;
  WordCache negative_word_cache_;

  
  FeatureExtractorClock* clock_;

  
  const base::string16* page_text_;  
  FeatureMap* features_;  
  DoneCallback done_callback_;

  
  scoped_ptr<ExtractionState> state_;

  
  
  base::WeakPtrFactory<PhishingTermFeatureExtractor> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PhishingTermFeatureExtractor);
};

}  

#endif  

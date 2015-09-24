// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_HOST_METRICS_H_
#define CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_HOST_METRICS_H_

#include <string>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

class SpellCheckHostMetrics {
 public:
  SpellCheckHostMetrics();
  ~SpellCheckHostMetrics();

  
  
  static void RecordCustomWordCountStats(size_t count);

  
  
  void RecordEnabledStats(bool enabled);

  
  
  void RecordDictionaryCorruptionStats(bool corrupted);

  
  
  void RecordCheckedWordStats(const base::string16& word, bool misspell);

  
  
  void RecordReplacedWordStats(int delta);

  
  
  void RecordSuggestionStats(int delta);

  
  void RecordSpellingServiceStats(bool enabled);

 private:
  friend class SpellcheckHostMetricsTest;
  void OnHistogramTimerExpired();

  
  void RecordWordCounts();

  
  int misspelled_word_count_;
  int last_misspelled_word_count_;

  
  int spellchecked_word_count_;
  int last_spellchecked_word_count_;

  
  int suggestion_show_count_;
  int last_suggestion_show_count_;

  
  int replaced_word_count_;
  int last_replaced_word_count_;

  
  int last_unique_word_count_;

  
  base::TimeTicks start_time_;
  
  base::hash_set<std::string> checked_word_hashes_;
  base::RepeatingTimer<SpellCheckHostMetrics> recording_timer_;
};

#endif  

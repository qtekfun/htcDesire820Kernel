// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef ENCODINGS_COMPACT_LANG_DET_COMPACT_LANG_DET_H_
#define ENCODINGS_COMPACT_LANG_DET_COMPACT_LANG_DET_H_

#include "languages/public/languages.h"
#include "encodings/compact_lang_det/win/cld_utf8statetable.h"

namespace cld {
  struct CLDTableSummary;
}  

namespace CompactLangDet {
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  struct DetectionTables {
    const cld::CLDTableSummary* quadgram_obj;
    const UTF8PropObj* unigram_obj;
  };

  
  Language DetectLanguage(const DetectionTables* tables,
                          const char* buffer,
                          int buffer_length,
                          bool is_plain_text,
                          bool* is_reliable);

  
  
  Language DetectLanguageSummary(
                          const DetectionTables* tables,
                          const char* buffer,
                          int buffer_length,
                          bool is_plain_text,
                          Language* language3,
                          int* percent3,
                          int* text_bytes,
                          bool* is_reliable);

  
  
  
  Language DetectLanguageSummary(
                          const DetectionTables* tables,
                          const char* buffer,
                          int buffer_length,
                          bool is_plain_text,
                          const char* tld_hint,       
                          int encoding_hint,          
                          Language language_hint,     
                          Language* language3,
                          int* percent3,
                          int* text_bytes,
                          bool* is_reliable);

  
  
  
  
  
  
  
  
  Language ExtDetectLanguageSummary(
                          const DetectionTables* tables,
                          const char* buffer,
                          int buffer_length,
                          bool is_plain_text,
                          Language* language3,
                          int* percent3,
                          int* text_bytes,
                          bool* is_reliable);

  
  
  
  
  
  
  
  
  
  Language ExtDetectLanguageSummary(
                          const DetectionTables* tables,
                          const char* buffer,
                          int buffer_length,
                          bool is_plain_text,
                          const char* tld_hint,       
                          int encoding_hint,          
                          Language language_hint,     
                          Language* language3,
                          int* percent3,
                          int* text_bytes,
                          bool* is_reliable);

  
  
  
  
  
  Language ExtDetectLanguageSummary(
                          const DetectionTables* tables,
                          const char* buffer,
                          int buffer_length,
                          bool is_plain_text,
                          const char* tld_hint,       
                          int encoding_hint,          
                          Language language_hint,     
                          Language* language3,
                          int* percent3,
                          double* normalized_score3,
                          int* text_bytes,
                          bool* is_reliable);

  
  
  const char* DetectLanguageVersion();
};      

#endif  

// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ENCODINGS_COMPACT_LANG_DET_CLDUTIL_DBG_H_
#define ENCODINGS_COMPACT_LANG_DET_CLDUTIL_DBG_H_

#include "encodings/compact_lang_det/cldutil.h"
#include <string>
#include "encodings/compact_lang_det/ext_lang_enc.h"
#include "encodings/compact_lang_det/tote.h"
#include "encodings/compact_lang_det/win/cld_basictypes.h"
#include "encodings/compact_lang_det/win/cld_commandlineflags.h"

DECLARE_bool(dbgscore);
DECLARE_bool(dbglookup);
DECLARE_bool(dbgreli);

using std::string;

namespace cld {



  void DbgScoreInit(const char* src, int len);

  
  void DbgLangName3(Language lang, char* temp);

  
  void DbgScoreState();

  void DbgScoreTop(const char* src, int srclen, Tote* chunk_tote);

  void DbgScoreFlush();

  
  void DbgScoreRecord(const char* src, uint32 probs, int len);

  void DbgScoreRecordUni(const char* src, int propval, int len);

  
  void PrintLang(FILE* f, const Tote* chunk_tote,
                 const Language cur_lang, const bool cur_unreliable,
                 Language prior_lang, bool prior_unreliable);

  
  void PrintLang2(FILE* f,
                  const Language lang1, const Language lang2, bool diff_prior);

  
  void PrintText(FILE* f, Language cur_lang, const string& str);

  
  void PrintTextSpeculative(FILE* f, Language cur_lang, const string& str);

  
  void PrintSkippedText(FILE* f, const string& str);

  void DbgProbsToStderr(uint32 probs);
  void DbgUniTermToStderr(int propval, const uint8* usrc, int len);
  
  void DbgBiTermToStderr(uint32 bihash, uint32 probs,
                          const char* src, int len);
  void DbgQuadTermToStderr(uint32 quadhash, uint32 probs,
                          const char* src, int len);
  void DbgWordTermToStderr(uint64 wordhash, uint32 probs,
                          const char* src, int len);

}       


#endif  

// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ENCODINGS_COMPACT_LANG_DET_GETONESCRIPTSPAN_H_
#define ENCODINGS_COMPACT_LANG_DET_GETONESCRIPTSPAN_H_

#include "encodings/compact_lang_det/letterscript_enum.h"
#include "encodings/compact_lang_det/compact_lang_det_impl.h"

namespace getone {
  static const int kMaxScriptBuffer = 4096;
  static const int kMaxScriptLowerBuffer = (kMaxScriptBuffer * 3) / 2;
  static const int kMaxScriptBytes = kMaxScriptBuffer- 8;   
  static const int kMaxAnswerBuffer = 256;

  typedef enum UnicodeLScript ULScript;

  typedef struct {
    char* text;             
    int text_bytes;         
    int offset;             
    ULScript script;        
    Language lang;          
    bool truncated;         
                            
  } LangSpan;


  static inline bool IsContinuationByte(char c) {
    return static_cast<signed char>(c) < -64;
  }

  
  
  int GetUTF8LetterScriptNum(const char* src);


  
  
  const char* AdvanceQuad(const char* src);
}     






class ScriptScanner {
 public:
  ScriptScanner(const char* buffer, int buffer_length, bool is_plain_text);
  ~ScriptScanner();

  
  bool GetOneScriptSpan(getone::LangSpan* span);

  
  void LowerScriptSpan(getone::LangSpan* span);

  
  
  bool GetOneScriptSpanLower(getone::LangSpan* span);

 private:
  int SkipToFrontOfSpan(const char* src, int len, int* script);

  const char* start_byte_;
  const char* next_byte_;
  const char* next_byte_limit_;
  int byte_length_;
  bool is_plain_text_;
  char* script_buffer_;           
  char* script_buffer_lower_;     
};


class LangScanner {
 public:
  LangScanner(const CompactLangDetImpl::LangDetObj* langdetobj,
              getone::LangSpan* spn, int smoothwidth, int smoothcandidates,
              int maxlangs, int minlangspan);
  ~LangScanner();


  int script() {return script_;}

  
  
  void NewText(getone::LangSpan* spn);

  bool GetOneShortLangSpanBoot(getone::LangSpan* span);  
  bool GetOneLangSpanBoot(getone::LangSpan* span);       

  
  bool GetOneShortLangSpan(const CompactLangDetImpl::LangDetObj* langdetobj,
                           getone::LangSpan* span);
  bool GetOneLangSpan(const CompactLangDetImpl::LangDetObj* langdetobj,
                      getone::LangSpan* span);

  
  void SetLanguageBias(const CompactLangDetImpl::LangDetObj* langdetobj,
                       Language key, int delta);

  
  int next_answer_;
  char answer_buffer_[getone::kMaxAnswerBuffer];
  char answer_buffer2_[getone::kMaxAnswerBuffer];
  char answer_buffer3_[getone::kMaxAnswerBuffer];
  char answer_buffer4_[getone::kMaxAnswerBuffer];

 private:
  const char* start_byte_;
  const char* next_byte_limit_;
  const char* next_byte_;
  const char* onelangspan_begin_;
  int byte_length_;
  int script_;
  Language spanlang_;
  int smoothwidth_;
  int smoothwidth_2_;
  int smoothcandidates_;
  int maxlangs_;
  int minlangspan_;
  int rb_size_;
  int next_rb_;
  int rb_mask_;
  uint32* rb_;
  int* offset_rb_;
};

#endif  

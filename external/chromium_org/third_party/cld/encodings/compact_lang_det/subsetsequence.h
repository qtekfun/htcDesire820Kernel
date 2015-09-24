// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef ENCODINGS_COMPACT_LANG_DET_SUBSETSEQUENCE_H_
#define ENCODINGS_COMPACT_LANG_DET_SUBSETSEQUENCE_H_

#include "encodings/compact_lang_det/win/cld_basictypes.h"
#include "encodings/compact_lang_det/win/cld_google.h"


class SubsetSequence {
 public:
   void Init();
   void Add(uint8 e);
   void Extract(int n, uint8* dst);
   SubsetSequence() {Init();}
   ~SubsetSequence() {};

 private:
   uint8 Median3(int sub);
   void NewLevel();
   void DoCarries();
   void Flush();

   static const int kMaxLevel_ = 16;    
   static const int kMaxSeq_ = 128;

   int k_;
   int next_e_;
   int limit_e_;
   int level_limit_e_;
   uint8 seq_[kMaxSeq_];
   uint8 count_[kMaxLevel_ + 1];        

   DISALLOW_EVIL_CONSTRUCTORS(SubsetSequence);

   
   COMPILE_ASSERT(kMaxSeq_ >= (kMaxLevel_ * 2 + 40), kMaxSeq__is_too_small);
};

#endif  

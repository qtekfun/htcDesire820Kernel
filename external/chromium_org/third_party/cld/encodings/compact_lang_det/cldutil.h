// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ENCODINGS_COMPACT_LANG_DET_CLDUTIL_H_
#define ENCODINGS_COMPACT_LANG_DET_CLDUTIL_H_

#include <string>
#include "encodings/compact_lang_det/ext_lang_enc.h"
#include "encodings/compact_lang_det/tote.h"
#include "encodings/compact_lang_det/win/cld_basictypes.h"
#include "encodings/compact_lang_det/win/cld_commandlineflags.h"
#include "encodings/compact_lang_det/win/cld_utf8statetable.h"

namespace cld {

  
  
  typedef struct {
    uint32 key[4];        
    uint32 value[4];      
  } SmallWordProbBucket4;

  
  
  typedef struct {
    uint16 key[4];        
                          
    uint32 value[4];      
  } LargeQuadProbBucket4;

  
  
  typedef struct {
    uint32 keyvalue[4];   
  } IndirectProbBucket4;


  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct CLDTableSummary {
    const IndirectProbBucket4* kCLDTable;
                                        
                                        
    const uint32* kCLDTableInd;         
    const int kCLDTableSize;            
    const int kCLDTableIndSize;         
    const int kCLDTableKeyMask;         
    const int kCLDTableBuildDate;       
  };


  
  
  
  typedef struct {
    uint8 probs[8];
  } UnigramProbArray;

  
  
  
  static const int kTargetCTJKVZProbsSize = 242;
  static const UnigramProbArray kTargetCTJKVZProbs[kTargetCTJKVZProbsSize] = {
    {{0,0,0,0,0,0,0,0}},
    {{0,0,0,0,0,12,0,0}},
    {{0,0,0,0,12,0,0,0}},
    {{0,0,0,12,0,0,0,0}},
    {{0,0,12,0,0,0,0,0}},
    {{0,12,0,0,0,0,0,0}},
    {{12,0,0,0,0,0,0,0}},

    {{8,0,0,0,4,0,0,0}},
    {{8,0,0,4,0,0,0,0}},
    {{8,0,4,0,0,0,0,0}},
    {{8,4,0,0,0,0,0,0}},
    {{8,2,0,2,0,0,0,0}},
    {{0,0,0,0,0,8,0,0}},
    {{0,4,8,0,0,0,0,0}},
    {{4,0,0,0,0,8,0,0}},
    {{0,0,8,0,0,0,0,0}},
    {{8,2,2,0,0,0,0,0}},
    {{0,8,4,0,0,0,0,0}},
    {{8,0,0,0,0,4,0,0}},
    {{0,8,2,0,0,0,0,0}},
    {{4,8,0,0,0,0,0,0}},
    {{2,8,0,2,0,0,0,0}},
    {{2,2,8,0,0,0,0,0}},
    {{0,8,0,0,0,0,0,0}},
    {{0,2,8,0,0,0,0,0}},
    {{2,8,2,0,0,0,0,0}},
    {{8,0,0,0,0,0,0,0}},
    {{2,8,0,0,0,0,0,0}},
    {{8,2,0,0,0,0,0,0}},

    {{0,6,2,0,2,0,0,0}},
    {{2,0,0,0,6,0,0,0}},
    {{4,0,0,0,6,0,0,0}},
    {{4,6,0,0,4,0,0,0}},
    {{4,6,2,0,2,0,0,0}},
    {{4,6,4,0,2,0,0,0}},
    {{5,4,6,0,0,0,0,0}},
    {{6,0,0,0,4,0,0,0}},
    {{6,0,2,0,4,0,0,0}},
    {{6,0,4,0,4,0,0,0}},
    {{6,2,0,0,4,0,0,0}},
    {{6,2,2,0,4,0,0,0}},
    {{6,2,4,0,2,0,0,0}},
    {{6,4,0,0,2,0,0,0}},
    {{6,4,2,0,2,0,0,0}},
    {{0,0,6,2,0,0,0,0}},
    {{0,6,2,0,0,2,0,0}},
    {{2,2,2,0,0,6,0,0}},
    {{2,2,6,4,0,0,0,0}},
    {{2,4,0,0,0,6,0,0}},
    {{2,6,0,4,0,0,0,0}},
    {{2,6,2,4,0,0,0,0}},
    {{2,6,4,4,0,0,0,0}},
    {{4,0,2,0,0,6,0,0}},
    {{4,2,6,2,0,0,0,0}},
    {{4,4,2,0,0,6,0,0}},
    {{4,6,4,0,0,2,0,0}},
    {{6,0,2,0,0,2,0,0}},
    {{6,2,0,0,0,2,0,0}},
    {{6,2,2,0,0,4,0,0}},
    {{6,2,4,0,0,2,0,0}},
    {{4,6,2,0,0,4,0,0}},
    {{6,4,2,0,0,4,0,0}},
    {{2,0,0,0,0,6,0,0}},
    {{6,2,0,2,0,0,0,0}},
    {{2,2,0,0,0,6,0,0}},
    {{6,2,6,0,0,0,0,0}},
    {{6,4,2,0,0,2,0,0}},
    {{6,4,2,2,0,0,0,0}},
    {{4,6,4,2,0,0,0,0}},
    {{6,0,2,0,0,4,0,0}},
    {{6,0,4,0,0,2,0,0}},
    {{6,0,6,0,0,0,0,0}},
    {{6,2,2,0,0,0,0,0}},
    {{6,4,0,0,0,2,0,0}},
    {{6,4,5,0,0,0,0,0}},
    {{0,6,0,2,0,0,0,0}},
    {{0,6,2,2,0,0,0,0}},
    {{2,6,0,2,0,0,0,0}},
    {{2,6,2,2,0,0,0,0}},
    {{4,2,0,0,0,6,0,0}},
    {{6,4,0,0,0,4,0,0}},
    {{6,4,0,2,0,0,0,0}},
    {{6,6,0,2,0,0,0,0}},
    {{6,0,4,0,0,4,0,0}},
    {{6,2,0,0,0,4,0,0}},
    {{6,6,2,2,0,0,0,0}},
    {{4,6,0,0,0,2,0,0}},
    {{2,6,6,0,0,0,0,0}},
    {{4,5,6,0,0,0,0,0}},
    {{4,6,0,2,0,0,0,0}},
    {{6,2,0,0,0,6,0,0}},
    {{0,6,4,2,0,0,0,0}},
    {{4,0,6,0,0,0,0,0}},
    {{2,6,4,2,0,0,0,0}},
    {{4,6,0,0,0,4,0,0}},
    {{6,2,2,0,0,0,0,0}},
    {{4,6,2,2,0,0,0,0}},
    {{4,6,5,0,0,0,0,0}},
    {{6,0,2,0,0,0,0,0}},
    {{6,4,4,0,0,0,0,0}},
    {{4,2,6,0,0,0,0,0}},
    {{2,0,6,0,0,0,0,0}},
    {{4,4,0,0,0,6,0,0}},
    {{4,4,6,0,0,0,0,0}},
    {{4,6,2,0,0,2,0,0}},
    {{2,2,6,0,0,0,0,0}},
    {{2,4,6,0,0,0,0,0}},
    {{0,6,6,0,0,0,0,0}},
    {{6,2,4,0,0,0,0,0}},
    {{0,4,6,0,0,0,0,0}},
    {{4,0,0,0,0,6,0,0}},
    {{4,6,4,0,0,0,0,0}},
    {{6,0,0,0,0,6,0,0}},
    {{6,0,0,0,0,2,0,0}},
    {{6,0,4,0,0,0,0,0}},
    {{6,5,4,0,0,0,0,0}},
    {{0,2,6,0,0,0,0,0}},
    {{0,0,6,0,0,0,0,0}},
    {{6,6,2,0,0,0,0,0}},
    {{2,6,4,0,0,0,0,0}},
    {{6,4,2,0,0,0,0,0}},
    {{2,6,2,0,0,0,0,0}},
    {{2,6,0,0,0,0,0,0}},
    {{6,0,0,0,0,4,0,0}},
    {{6,4,0,0,0,0,0,0}},
    {{6,6,0,0,0,0,0,0}},
    {{5,6,4,0,0,0,0,0}},
    {{0,6,0,0,0,0,0,0}},
    {{6,2,0,0,0,0,0,0}},
    {{0,6,2,0,0,0,0,0}},
    {{4,6,2,0,0,0,0,0}},
    {{0,6,4,0,0,0,0,0}},
    {{4,6,0,0,0,0,0,0}},
    {{6,0,0,0,0,0,0,0}},
    {{6,6,5,0,0,0,0,0}},
    {{6,5,6,0,0,0,0,0}},
    {{5,6,6,0,0,0,0,0}},
    {{5,5,6,0,0,0,0,0}},
    {{5,6,5,0,0,0,0,0}},
    {{6,5,5,0,0,0,0,0}},
    {{6,6,6,0,0,0,0,0}},
    {{6,5,0,0,0,0,0,0}},
    {{6,0,5,0,0,0,0,0}},
    {{0,6,5,0,0,0,0,0}},
    {{5,6,0,0,0,0,0,0}},
    {{5,0,6,0,0,0,0,0}},
    {{0,5,6,0,0,0,0,0}},

    {{0,0,0,0,4,0,0,0}},
    {{0,0,0,4,0,0,0,0}},
    {{2,2,0,0,4,0,0,0}},
    {{2,2,2,0,4,0,0,0}},
    {{2,4,0,0,2,0,0,0}},
    {{2,4,2,0,2,0,0,0}},
    {{2,4,4,0,2,0,0,0}},
    {{4,0,2,0,4,0,0,0}},
    {{4,0,4,0,2,0,0,0}},
    {{4,2,0,0,2,0,0,0}},
    {{4,2,2,0,2,0,0,0}},
    {{4,4,0,0,2,0,0,0}},
    {{4,4,2,0,2,0,0,0}},
    {{4,4,4,0,2,0,0,0}},
    {{0,2,2,4,0,0,0,0}},
    {{2,2,4,2,0,0,0,0}},
    {{2,4,4,0,0,2,0,0}},
    {{2,4,4,2,0,0,0,0}},
    {{4,0,4,0,0,2,0,0}},
    {{4,0,4,0,0,4,0,0}},
    {{4,2,2,4,0,0,0,0}},
    {{4,4,0,2,0,0,0,0}},
    {{2,2,0,4,0,0,0,0}},
    {{2,4,2,2,0,0,0,0}},
    {{4,4,2,2,0,0,0,0}},
    {{4,0,4,0,0,0,0,0}},
    {{4,4,4,0,0,4,0,0}},
    {{0,4,0,2,0,0,0,0}},
    {{0,4,2,2,0,0,0,0}},
    {{4,0,2,0,0,2,0,0}},
    {{4,2,0,0,0,4,0,0}},
    {{2,2,2,0,0,4,0,0}},
    {{4,0,0,2,0,0,0,0}},
    {{4,4,4,0,0,2,0,0}},
    {{4,0,0,0,0,4,0,0}},
    {{4,0,2,0,0,4,0,0}},
    {{4,2,0,0,0,2,0,0}},
    {{4,2,2,0,0,2,0,0}},
    {{2,4,0,2,0,0,0,0}},
    {{2,2,0,0,0,4,0,0}},
    {{2,4,0,0,0,4,0,0}},
    {{2,4,2,0,0,4,0,0}},
    {{4,2,4,0,0,0,0,0}},
    {{2,0,4,0,0,0,0,0}},
    {{4,0,2,0,0,0,0,0}},
    {{4,4,0,0,0,4,0,0}},
    {{4,4,2,0,0,4,0,0}},
    {{0,4,4,0,0,0,0,0}},
    {{4,4,0,0,0,2,0,0}},
    {{2,4,0,0,0,2,0,0}},
    {{2,2,4,0,0,0,0,0}},
    {{0,2,4,0,0,0,0,0}},
    {{4,2,2,0,0,0,0,0}},
    {{2,4,2,0,0,2,0,0}},
    {{4,4,4,0,0,0,0,0}},
    {{2,4,4,0,0,0,0,0}},
    {{0,0,4,0,0,0,0,0}},
    {{0,4,2,0,0,0,0,0}},
    {{4,4,2,0,0,2,0,0}},
    {{2,4,2,0,0,0,0,0}},
    {{4,2,0,0,0,0,0,0}},
    {{4,4,0,0,0,0,0,0}},
    {{4,4,2,0,0,0,0,0}},
    {{2,4,0,0,0,0,0,0}},
    {{0,4,0,0,0,0,0,0}},
    {{4,0,0,0,0,0,0,0}},
    {{0,0,0,4,4,0,0,0}},
    {{0,0,4,0,4,0,0,0}},
    {{0,0,4,4,0,0,0,0}},
    {{0,4,0,0,4,0,0,0}},
    {{0,4,0,4,0,0,0,0}},
    {{4,0,0,0,4,0,0,0}},
    {{4,0,0,4,0,0,0,0}},

    {{2,0,0,0,0,0,0,0}},
    {{0,2,0,0,0,0,0,0}},
    {{0,2,0,2,2,0,0,0}},
    {{0,2,2,0,2,0,0,0}},
    {{2,0,0,2,2,0,0,0}},
    {{2,0,2,0,2,0,0,0}},
    {{2,0,2,2,0,0,0,0}},
    {{2,2,0,0,2,0,0,0}},
    {{2,2,2,2,0,0,0,0}},
    {{2,2,0,2,0,0,0,0}},
    {{2,2,0,0,0,0,0,0}},
    {{0,0,2,0,0,0,0,0}},
    {{0,2,2,0,0,0,0,0}},
    {{2,2,2,0,0,0,0,0}},
    {{0,0,0,2,0,0,0,0}},
    {{2,0,2,0,0,0,0,0}},
    {{0,2,0,2,0,0,0,0}},
    {{0,0,2,2,0,0,0,0}},
    {{0,2,2,2,0,0,0,0}},
  };




  
  static const uint8 kSkipSpaceVowelContinue[256] = {
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,1,0,0,0,1,0,0, 0,1,0,0,0,0,0,1, 0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,
    0,1,0,0,0,1,0,0, 0,1,0,0,0,0,0,1, 0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,

    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
  };

  
  static const uint8 kSkipSpaceContinue[256] = {
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
  };


  
  static const Language kOnlyLanguagePerLScript[] = {
    ENGLISH,            
    UNKNOWN_LANGUAGE,   
    
    GREEK,              
    UNKNOWN_LANGUAGE,   
    ARMENIAN,           
    UNKNOWN_LANGUAGE,   
    UNKNOWN_LANGUAGE,   
    SYRIAC,             
    DHIVEHI,            
    UNKNOWN_LANGUAGE,   
    UNKNOWN_LANGUAGE,   
    PUNJABI,            
    GUJARATI,           
    ORIYA,              
    TAMIL,              
    TELUGU,             
    KANNADA,            
    MALAYALAM,          
    SINHALESE,          
    THAI,               
    LAOTHIAN,           
    UNKNOWN_LANGUAGE,   
    BURMESE,            
    GEORGIAN,           
    UNKNOWN_LANGUAGE,   
    UNKNOWN_LANGUAGE,   
    CHEROKEE,           
    INUKTITUT,          
    X_OGHAM,            
    X_RUNIC,            
    KHMER,              
    MONGOLIAN,          
    X_YI,               
    X_OLD_ITALIC,       
    X_GOTHIC,           
    X_DESERET,          
    ENGLISH,            
    TAGALOG,            
    X_HANUNOO,          
    X_BUHID,            
    X_TAGBANWA,         
    LIMBU,              
    X_TAI_LE,           
    X_LINEAR_B,         
    X_UGARITIC,         
    X_SHAVIAN,          
    X_OSMANYA,          
    X_CYPRIOT,          
    X_BUGINESE,         
    X_COPTIC,           
    X_NEW_TAI_LUE,      
    X_GLAGOLITIC,       
    X_TIFINAGH,         
    X_SYLOTI_NAGRI,     
    X_OLD_PERSIAN,      
    X_KHAROSHTHI,       
    X_BALINESE,         
    X_CUNEIFORM,        
    X_PHOENICIAN,       
    X_PHAGS_PA,         
    X_NKO,              

    
    X_SUDANESE,         
    X_LEPCHA,           
    X_OL_CHIKI,         
    X_VAI,              
    X_SAURASHTRA,       
    X_KAYAH_LI,         
    X_REJANG,           
    X_LYCIAN,           
    X_CARIAN,           
    X_LYDIAN,           
    X_CHAM,             
  };

  COMPILE_ASSERT(arraysize(kOnlyLanguagePerLScript) == ULScript_NUM_SCRIPTS,
       kOnlyLanguagePerLScript_has_incorrect_length);


  
  
  static const Language kDefaultLanguagePerLScript[] = {
    UNKNOWN_LANGUAGE,            
    ENGLISH,   
    UNKNOWN_LANGUAGE,              
    RUSSIAN,   
    UNKNOWN_LANGUAGE,           
    HEBREW,   
    ARABIC,   
    UNKNOWN_LANGUAGE,             
    UNKNOWN_LANGUAGE,            
    HINDI,   
    BENGALI,   
    UNKNOWN_LANGUAGE,            
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,              
    UNKNOWN_LANGUAGE,              
    UNKNOWN_LANGUAGE,             
    UNKNOWN_LANGUAGE,            
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,               
    UNKNOWN_LANGUAGE,           
    TIBETAN,   
    UNKNOWN_LANGUAGE,            
    UNKNOWN_LANGUAGE,           
    CHINESE,   
    AMHARIC,   
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,            
    UNKNOWN_LANGUAGE,            
    UNKNOWN_LANGUAGE,              
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,               
    UNKNOWN_LANGUAGE,       
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,            
    UNKNOWN_LANGUAGE,            
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,            
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,              
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,          
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,      
    UNKNOWN_LANGUAGE,       
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,     
    UNKNOWN_LANGUAGE,      
    UNKNOWN_LANGUAGE,       
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,        
    UNKNOWN_LANGUAGE,       
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,              

    
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,              
    UNKNOWN_LANGUAGE,       
    UNKNOWN_LANGUAGE,         
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,           
    UNKNOWN_LANGUAGE,             
  };

  COMPILE_ASSERT(arraysize(kDefaultLanguagePerLScript) == ULScript_NUM_SCRIPTS,
       kDefaultLanguagePerLScript_has_incorrect_length);


  
  
  
  static const uint8 kIsStandaloneLang[EXT_NUM_LANGUAGES + 1] = {
     0,
     0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,0,    
     0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
     0,1,0,0,1, 0,1,0,0,0, 0,0,1,1,0, 0,0,0,0,1,    
     1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,1,    
     0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0,    

     0,0,0,0,1, 0,1,1,1,0, 1,0,0,0,0, 0,0,0,0,0,    
     0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
     0,1,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,    

     0,0,0,0,0,                                     
    
     0,0,0,0,0, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
     1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

     1,1,1,1,
   };

   
   
   
   static const uint8 kIsUnigramLang[EXT_NUM_LANGUAGES + 1] = {
      0,
      0,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,1,0,0,0,    
      0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,    
      0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,    
      0,0,0,0,0, 0,1,0,0,1, 0,0,0,0,0, 0,0,0,0,0,    
      0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,    

      0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,    
      0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,    
      0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,0,    

      0,0,0,0,0,                                     
     

      0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,    
      0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,    

      0,0,0,0,
   };


  
  
  static const uint8 kScoreUniPerLScript[] = {
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
  };

  COMPILE_ASSERT(arraysize(kScoreUniPerLScript) == ULScript_NUM_SCRIPTS,
       kScoreUniPerLScript_has_incorrect_length);


  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  static const uint8 kIsPackedTop40[EXT_NUM_LANGUAGES + 1] = {
    0,
    1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,0,
    1,1,1,1,0, 1,0,1,0,0, 0,0,1,1,1, 1,0,0,1,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,0,0,0,0,
    0,0,0,1,0, 0,1,0,1,1, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0,

    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

    0,0,0,0,0,                                    
    

    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

    0,0,0,0,
  };



  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static const int kLgProbV2TblSize = 234;
  static const uint8 kLgProbV2Tbl[kLgProbV2TblSize * 8] = {
    1,1,1,1,1, 1,1,1,     
    2,2,2,1,1, 2,2,1,     
    2,2,2,2,2, 2,2,2,
    3,3,2,2,1, 3,2,1,     
    3,3,3,2,2, 3,3,2,
    3,3,3,3,3, 3,3,3,
    4,3,3,2,1, 4,3,1,     
    4,4,3,3,2, 4,3,2,
    4,4,4,3,3, 4,4,3,
    4,4,4,4,4, 4,4,4,
    5,4,3,2,1, 5,3,1,     
    5,4,4,3,2, 5,4,2,
    5,5,4,4,3, 5,4,3,
    5,5,5,4,4, 5,5,4,
    5,5,5,5,5, 5,5,5,
    6,5,4,2,1, 6,4,1,     
    6,5,4,3,2, 6,4,2,
    6,5,5,4,3, 6,5,3,
    6,6,5,5,4, 6,5,4,
    6,6,6,5,5, 6,6,5,
    6,6,6,6,6, 6,6,6,
    7,6,4,3,1, 7,4,1,     
    7,6,5,3,2, 7,5,2,
    7,6,5,4,3, 7,5,3,
    7,6,6,5,4, 7,6,4,
    7,7,6,6,5, 7,6,5,
    7,7,7,6,6, 7,7,6,
    7,7,7,7,7, 7,7,7,
    8,6,5,3,1, 8,5,1,     
    8,7,5,4,2, 8,5,2,
    8,7,6,4,3, 8,6,3,
    8,7,6,5,4, 8,6,4,
    8,7,7,6,5, 8,7,5,
    8,8,7,7,6, 8,7,6,
    8,8,8,7,7, 8,8,7,
    8,8,8,8,8, 8,8,8,
    9,7,5,3,1, 9,5,1,     
    9,7,6,4,2, 9,6,2,
    9,8,6,5,3, 9,6,3,
    9,8,7,5,4, 9,7,4,
    9,8,7,6,5, 9,7,5,
    9,8,8,7,6, 9,8,6,
    9,9,8,8,7, 9,8,7,
    9,9,9,8,8, 9,9,8,
    9,9,9,9,9, 9,9,9,
    10,8,6,3,1, 10,6,1,   
    10,8,6,4,2, 10,6,2,
    10,8,7,5,3, 10,7,3,
    10,9,7,6,4, 10,7,4,
    10,9,8,6,5, 10,8,5,
    10,9,8,7,6, 10,8,6,
    10,9,9,8,7, 10,9,7,
    10,10,9,9,8, 10,9,8,
    10,10,10,9,9, 10,10,9,
    10,10,10,10,10, 10,10,10,
    11,9,6,4,1, 11,6,1,   
    11,9,7,4,2, 11,7,2,
    11,9,7,5,3, 11,7,3,
    11,9,8,6,4, 11,8,4,
    11,10,8,7,5, 11,8,5,
    11,10,9,7,6, 11,9,6,
    11,10,9,8,7, 11,9,7,
    11,10,10,9,8, 11,10,8,
    11,11,10,10,9, 11,10,9,
    11,11,11,10,10, 11,11,10,
    11,11,11,11,11, 11,11,11,
    12,9,7,4,1, 12,7,1,   
    12,10,7,5,2, 12,7,2,
    12,10,8,5,3, 12,8,3,
    12,10,8,6,4, 12,8,4,
    12,10,9,7,5, 12,9,5,
    12,11,9,8,6, 12,9,6,
    12,11,10,8,7, 12,10,7,
    12,11,10,9,8, 12,10,8,
    12,11,11,10,9, 12,11,9,
    12,12,11,11,10, 12,11,10,
    12,12,12,11,11, 12,12,11,
    12,12,12,12,12, 12,12,12,

    1,1,1,1,1, 1,1,1,
    2,2,2,1,1, 2,2,1,
    2,2,2,2,2, 2,2,2,
    3,3,2,2,1, 3,3,1,
    3,3,3,2,2, 3,3,2,
    3,3,3,3,3, 3,3,3,
    4,3,3,2,1, 4,3,1,
    4,4,3,3,2, 4,4,2,
    4,4,4,3,3, 4,4,3,
    4,4,4,4,4, 4,4,4,
    5,4,3,2,1, 5,4,1,
    5,4,4,3,2, 5,4,2,
    5,5,4,4,3, 5,5,3,
    5,5,5,4,4, 5,5,4,
    5,5,5,5,5, 5,5,5,
    6,5,4,2,1, 6,5,1,
    6,5,4,3,2, 6,5,2,
    6,5,5,4,3, 6,5,3,
    6,6,5,5,4, 6,6,4,
    6,6,6,5,5, 6,6,5,
    6,6,6,6,6, 6,6,6,
    7,6,4,3,1, 7,6,1,
    7,6,5,3,2, 7,6,2,
    7,6,5,4,3, 7,6,3,
    7,6,6,5,4, 7,6,4,
    7,7,6,6,5, 7,7,5,
    7,7,7,6,6, 7,7,6,
    7,7,7,7,7, 7,7,7,
    8,6,5,3,1, 8,6,1,
    8,7,5,4,2, 8,7,2,
    8,7,6,4,3, 8,7,3,
    8,7,6,5,4, 8,7,4,
    8,7,7,6,5, 8,7,5,
    8,8,7,7,6, 8,8,6,
    8,8,8,7,7, 8,8,7,
    8,8,8,8,8, 8,8,8,
    9,7,5,3,1, 9,7,1,
    9,7,6,4,2, 9,7,2,
    9,8,6,5,3, 9,8,3,
    9,8,7,5,4, 9,8,4,
    9,8,7,6,5, 9,8,5,
    9,8,8,7,6, 9,8,6,
    9,9,8,8,7, 9,9,7,
    9,9,9,8,8, 9,9,8,
    9,9,9,9,9, 9,9,9,
    10,8,6,3,1, 10,8,1,
    10,8,6,4,2, 10,8,2,
    10,8,7,5,3, 10,8,3,
    10,9,7,6,4, 10,9,4,
    10,9,8,6,5, 10,9,5,
    10,9,8,7,6, 10,9,6,
    10,9,9,8,7, 10,9,7,
    10,10,9,9,8, 10,10,8,
    10,10,10,9,9, 10,10,9,
    10,10,10,10,10, 10,10,10,
    11,9,6,4,1, 11,9,1,
    11,9,7,4,2, 11,9,2,
    11,9,7,5,3, 11,9,3,
    11,9,8,6,4, 11,9,4,
    11,10,8,7,5, 11,10,5,
    11,10,9,7,6, 11,10,6,
    11,10,9,8,7, 11,10,7,
    11,10,10,9,8, 11,10,8,
    11,11,10,10,9, 11,11,9,
    11,11,11,10,10, 11,11,10,
    11,11,11,11,11, 11,11,11,
    12,9,7,4,1, 12,9,1,
    12,10,7,5,2, 12,10,2,
    12,10,8,5,3, 12,10,3,
    12,10,8,6,4, 12,10,4,
    12,10,9,7,5, 12,10,5,
    12,11,9,8,6, 12,11,6,
    12,11,10,8,7, 12,11,7,
    12,11,10,9,8, 12,11,8,
    12,11,11,10,9, 12,11,9,
    12,12,11,11,10, 12,12,10,
    12,12,12,11,11, 12,12,11,
    12,12,12,12,12, 12,12,12,

    1,1,1,1,1, 1,1,1,
    2,2,2,1,1, 2,1,1,
    2,2,2,2,2, 2,2,2,
    3,3,2,2,1, 3,2,1,
    3,3,3,2,2, 3,2,2,
    3,3,3,3,3, 3,3,3,
    4,3,3,2,1, 4,2,1,
    4,4,3,3,2, 4,3,2,
    4,4,4,3,3, 4,3,3,
    4,4,4,4,4, 4,4,4,
    5,4,3,2,1, 5,2,1,
    5,4,4,3,2, 5,3,2,
    5,5,4,4,3, 5,4,3,
    5,5,5,4,4, 5,4,4,
    5,5,5,5,5, 5,5,5,
    6,5,4,2,1, 6,2,1,
    6,5,4,3,2, 6,3,2,
    6,5,5,4,3, 6,4,3,
    6,6,5,5,4, 6,5,4,
    6,6,6,5,5, 6,5,5,
    6,6,6,6,6, 6,6,6,
    7,6,4,3,1, 7,3,1,
    7,6,5,3,2, 7,3,2,
    7,6,5,4,3, 7,4,3,
    7,6,6,5,4, 7,5,4,
    7,7,6,6,5, 7,6,5,
    7,7,7,6,6, 7,6,6,
    7,7,7,7,7, 7,7,7,
    8,6,5,3,1, 8,3,1,
    8,7,5,4,2, 8,4,2,
    8,7,6,4,3, 8,4,3,
    8,7,6,5,4, 8,5,4,
    8,7,7,6,5, 8,6,5,
    8,8,7,7,6, 8,7,6,
    8,8,8,7,7, 8,7,7,
    8,8,8,8,8, 8,8,8,
    9,7,5,3,1, 9,3,1,
    9,7,6,4,2, 9,4,2,
    9,8,6,5,3, 9,5,3,
    9,8,7,5,4, 9,5,4,
    9,8,7,6,5, 9,6,5,
    9,8,8,7,6, 9,7,6,
    9,9,8,8,7, 9,8,7,
    9,9,9,8,8, 9,8,8,
    9,9,9,9,9, 9,9,9,
    10,8,6,3,1, 10,3,1,
    10,8,6,4,2, 10,4,2,
    10,8,7,5,3, 10,5,3,
    10,9,7,6,4, 10,6,4,
    10,9,8,6,5, 10,6,5,
    10,9,8,7,6, 10,7,6,
    10,9,9,8,7, 10,8,7,
    10,10,9,9,8, 10,9,8,
    10,10,10,9,9, 10,9,9,
    10,10,10,10,10, 10,10,10,
    11,9,6,4,1, 11,4,1,
    11,9,7,4,2, 11,4,2,
    11,9,7,5,3, 11,5,3,
    11,9,8,6,4, 11,6,4,
    11,10,8,7,5, 11,7,5,
    11,10,9,7,6, 11,7,6,
    11,10,9,8,7, 11,8,7,
    11,10,10,9,8, 11,9,8,
    11,11,10,10,9, 11,10,9,
    11,11,11,10,10, 11,10,10,
    11,11,11,11,11, 11,11,11,
    12,9,7,4,1, 12,4,1,
    12,10,7,5,2, 12,5,2,
    12,10,8,5,3, 12,5,3,
    12,10,8,6,4, 12,6,4,
    12,10,9,7,5, 12,7,5,
    12,11,9,8,6, 12,8,6,
    12,11,10,8,7, 12,8,7,
    12,11,10,9,8, 12,9,8,
    12,11,11,10,9, 12,10,9,
    12,12,11,11,10, 12,11,10,
    12,12,12,11,11, 12,11,11,
    12,12,12,12,12, 12,12,12,
  };

  
  static const uint8 kLgProbV2TblBackmap[13] = {
    0,
    0, 1, 3, 6,   10, 15, 21, 28,   36, 45, 55, 66,
  };


  
  static const uint8 kAdvanceOneChar[256] = {
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3, 3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4,
  };

  
  static const uint8 kAdvanceOneCharButSpace[256] = {
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3, 3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4,
  };

  
  static const uint8 kAdvanceOneCharSpaceVowel[256] = {
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,1,0,0,0,1,0,0, 0,1,0,0,0,0,0,1, 0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,
    0,1,0,0,0,1,0,0, 0,1,0,0,0,0,0,1, 0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,

    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
  };

  
  static const uint8 kAdvanceOneCharSpace[256] = {
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
  };


  static inline int minint(int a, int b) {return (a < b) ? a: b;}
  static inline int maxint(int a, int b) {return (a > b) ? a: b;}

  
  int ReliabilityDelta(int value1, int value2, int count);
  int ReliabilityMainstream(int topscore, int len, int mean_score);

  
  inline const char* MyExtLanguageCode(Language lang) {
    return ExtLanguageCode(lang);
  }

  
  
  inline int LScript4(UnicodeLScript lscript) {
    if (lscript == ULScript_Latin) {return 0;}
    if (lscript == ULScript_Cyrillic) {return 1;}
    if (lscript == ULScript_Arabic) {return 2;}
    return 3;
  }


  

  
  inline const uint8* LgProb2TblEntry(int i) {
    return &kLgProbV2Tbl[i * 8];
  }

  
  
  inline uint8 LgProb5(const uint8* entry, int j) {
    return entry[j];
  }

  
  inline uint8 LgProb3(const uint8* entry, int j) {
    return entry[j + 5];
  }




  
  
  uint32 BiHashV25(const char* word_ptr, int bytecount);

  
  
  uint32 QuadHashV25(const char* word_ptr, int bytecount);

  
  
  uint32 QuadHashV25Underscore(const char* word_ptr, int bytecount);


  
  
  
  uint64 OctaHash40(const char* word_ptr, int bytecount);

  uint64 OctaHash40underscore(const char* word_ptr, int bytecount);


  
  inline void QuadFPJustHash(uint32 quadhash,
                                  uint32 keymask,
                                  int bucketcount,
                                  uint32* subscr, uint32* hashkey) {
    *subscr = (quadhash + (quadhash >> 12)) & (bucketcount - 1);
    *hashkey = quadhash & keymask;
  }

  
  
  
  inline const uint32 QuadHashV3Lookup4(const cld::CLDTableSummary* gram_obj,
                                        uint32 quadhash) {

    uint32 subscr, hashkey;
    const IndirectProbBucket4* quadtable = gram_obj->kCLDTable;
    uint32 keymask = gram_obj->kCLDTableKeyMask;
    int bucketcount = gram_obj->kCLDTableSize;
    QuadFPJustHash(quadhash, keymask, bucketcount, &subscr, &hashkey);
    const IndirectProbBucket4* bucket_ptr = &quadtable[subscr];
    
    if (((hashkey ^ bucket_ptr->keyvalue[0]) & keymask) == 0) {
      return bucket_ptr->keyvalue[0];
    }
    if (((hashkey ^ bucket_ptr->keyvalue[1]) & keymask) == 0) {
      return bucket_ptr->keyvalue[1];
    }
    if (((hashkey ^ bucket_ptr->keyvalue[2]) & keymask) == 0) {
      return bucket_ptr->keyvalue[2];
    }
    if (((hashkey ^ bucket_ptr->keyvalue[3]) & keymask) == 0) {
      return bucket_ptr->keyvalue[3];
    }
    return 0;
  }


  
  
  
  
  
  
  
  

  
  inline void OctaFPJustHash(uint64 longwordhash,
                                    uint32 keymask,
                                    int bucketcount,
                                    uint32* subscr, uint32* hashkey) {
    uint32 temp = (longwordhash + (longwordhash >> 12)) & (bucketcount - 1);
    *subscr = temp;
    temp = longwordhash >> 4;
    *hashkey = temp & keymask;
  }

  
  
  
  
  inline const uint32 OctaHashV3Lookup4(const cld::CLDTableSummary* gram_obj,
                                            uint64 longwordhash) {
    uint32 subscr, hashkey;
    const IndirectProbBucket4* octatable = gram_obj->kCLDTable;
    uint32 keymask = gram_obj->kCLDTableKeyMask;
    int bucketcount = gram_obj->kCLDTableSize;
    OctaFPJustHash(longwordhash, keymask, bucketcount,
                          &subscr, &hashkey);
    const IndirectProbBucket4* bucket_ptr = &octatable[subscr];
    
    if (((hashkey ^ bucket_ptr->keyvalue[0]) & keymask) == 0) {
      return bucket_ptr->keyvalue[0];
    }
    if (((hashkey ^ bucket_ptr->keyvalue[1]) & keymask) == 0) {
      return bucket_ptr->keyvalue[1];
    }
    if (((hashkey ^ bucket_ptr->keyvalue[2]) & keymask) == 0) {
      return bucket_ptr->keyvalue[2];
    }
    if (((hashkey ^ bucket_ptr->keyvalue[3]) & keymask) == 0) {
      return bucket_ptr->keyvalue[3];
    }
    return 0;
  }




  
  
  
  
  void ProcessProbV25UniTote(int propval, Tote* tote);

  
  
  
  
  void ProcessProbV25Tote(uint32 probs, Tote* tote);



  
  
  int DoUniScoreV3(const UTF8PropObj* unigram_obj,
                   const char* isrc, int srclen, int advance_by,
                   int* tote_grams, int gram_limit, Tote* chunk_tote);


  
  
  
  int DoBigramScoreV3(const cld::CLDTableSummary* bigram_obj,
                      const char* isrc, int srclen, Tote* chunk_tote);


  
  
  int DoQuadScoreV3(const cld::CLDTableSummary* quadgram_obj,
                    const char* isrc, int srclen, int advance_by,
                    int* tote_grams, int gram_limit, Tote* chunk_tote);

  
  
  
  int DoOctaScoreV3(const cld::CLDTableSummary* octagram_obj,
                    const char* isrc, int srclen, Tote* chunk_tote);


  
  static const int kMinReliable = 75;

  
  double GetNormalizedScore(Language lang, UnicodeLScript lscript,
                          int bytes, int score);

  
  int GetReliability(int len, UnicodeLScript lscript, const Tote* chunk_tote);



  
  
  
  uint8 inline PackLanguage(Language lang) {
    return static_cast<uint8>(lang + 1);}

  Language inline UnpackLanguage(int ilang) {
    return static_cast<Language>(ilang - 1);}

  
  bool inline IsUTF8ContinueByte(char c) {
    return static_cast<signed char>(c) < -64;}
  bool inline IsUTF8HighByte(char c) {
    return static_cast<signed char>(c) < 0;}


  
  
  void DemoteNotTop40(Tote* chunk_tote, int packed_plus_one);

}       


#endif  

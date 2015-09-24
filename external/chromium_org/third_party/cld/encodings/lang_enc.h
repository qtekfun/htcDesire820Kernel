// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef ENCODINGS_LANG_ENC_H__
#define ENCODINGS_LANG_ENC_H__

#include "languages/public/languages.h"
#include "encodings/public/encodings.h"


const Encoding* EncodingsForLanguage(Language lang);


bool DefaultEncodingForLanguage(Language lang,
                                Encoding *encoding);

const Language* LanguagesForEncoding(Encoding enc);

bool DefaultLanguageForEncoding(Encoding enc, Language* language);

bool IsLangEncCompatible(Language lang, Encoding enc);

Language DominantLanguageFromEncoding(Encoding enc);


const char* LanguageCode(Language lang, Encoding enc);

bool IsEncodingWithSupportedLanguage(Encoding enc);



bool LangsFromCountryCode(const char* country_code,
                          const Language** lang_arry,
                          int* num_langs);


bool EncFromCountryCode(const char* country_code, Encoding* enc);



enum VisualType {
  NOT_VISUAL_DOCUMENT = 0,
  VISUAL_HEBREW_HTML,  
  CONVERTED_RTL_PDF,   
};

VisualType default_visualtype();

const char* VisualTypeName(VisualType visualtype);



void InitLangEnc();

#endif  

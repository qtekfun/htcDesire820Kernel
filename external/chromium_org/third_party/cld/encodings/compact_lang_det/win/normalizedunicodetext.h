// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ENCODINGS_COMPACT_LANG_DET_WIN_NORMALIZEDUNICODETEXT_H_
#define ENCODINGS_COMPACT_LANG_DET_WIN_NORMALIZEDUNICODETEXT_H_

#include <tchar.h>
#include <windows.h>

#include "encodings/compact_lang_det/win/cld_scopedptr.h"


#if (WINVER < 0x0600)
typedef enum _NORM_FORM {
  NormalizationOther  = 0,
  NormalizationC = 0x1,
  NormalizationD = 0x2,
  NormalizationKC = 0x5,
  NormalizationKD = 0x6
} NORM_FORM;
#endif


class NormalizedUnicodeText {
 public:
  
  NormalizedUnicodeText();

  
  
  
  
  
  DWORD Normalize(NORM_FORM normalization_form, const WCHAR* text);

  
  const WCHAR* get() const { return normalized_text_; }

 private:
  
  
  
  
  const WCHAR* TryToNormalizeText(NORM_FORM normalization_form,
                                  const WCHAR* text, DWORD *error_code);

  
  const WCHAR* normalized_text_;
  
  
  
  
  
  
  
  scoped_array<WCHAR> text_;

  DISALLOW_COPY_AND_ASSIGN(NormalizedUnicodeText);
};


#endif  

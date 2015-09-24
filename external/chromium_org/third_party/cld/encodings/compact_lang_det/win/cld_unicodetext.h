// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ENCODINGS_COMPACT_LANG_DET_WIN_CLD_UNICODETEXT_H_
#define ENCODINGS_COMPACT_LANG_DET_WIN_CLD_UNICODETEXT_H_

#include "languages/public/languages.h"
#include "unicode/utypes.h"

namespace CompactLangDet {
  struct DetectionTables;
}  

Language DetectLanguageOfUnicodeText(
    const CompactLangDet::DetectionTables* detection_tables,
    const UChar* text, bool is_plain_text,
    bool* is_reliable, int* num_languages,
    int* error_code, int* text_bytes);


#endif  

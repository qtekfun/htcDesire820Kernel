// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_WORD_TRIMMER_H_
#define CHROME_BROWSER_SPELLCHECKER_WORD_TRIMMER_H_

#include "base/strings/string16.h"

base::string16 TrimWords(size_t* start,
                         size_t end,
                         const base::string16& text,
                         size_t keep);

#endif  

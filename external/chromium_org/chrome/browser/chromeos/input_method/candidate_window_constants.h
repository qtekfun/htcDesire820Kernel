// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_WINDOW_CONSTANTS_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_WINDOW_CONSTANTS_H_

namespace chromeos {
namespace input_method {

const int kFontSizeDelta = 2;

#if defined(GOOGLE_CHROME_BUILD)
const char kJapaneseFontName[] = "MotoyaG04Gothic";
#else
const char kJapaneseFontName[] = "IPAPGothic";
#endif

const int kMinCandidateLabelWidth = 100;
const int kMaxCandidateLabelWidth = 500;
const int kMinPreeditAreaWidth = 134;

const int kInfolistIndicatorIconWidth = 4;
const int kInfolistIndicatorIconPadding = 2;

}  
}  

#endif  

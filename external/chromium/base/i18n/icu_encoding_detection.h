// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_I18N_ICU_ENCODING_DETECTION_H_
#define BASE_I18N_ICU_ENCODING_DETECTION_H_
#pragma once

#include <string>
#include <vector>

namespace base {

bool DetectEncoding(const std::string& text, std::string* encoding);

bool DetectAllEncodings(const std::string& text,
                        std::vector<std::string>* encodings);

}  

#endif  

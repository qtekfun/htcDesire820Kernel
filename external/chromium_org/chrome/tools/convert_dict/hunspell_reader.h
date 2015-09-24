// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TOOLS_CONVERT_DICT_HUNSPELL_READER_H_
#define CHROME_TOOLS_CONVERT_DICT_HUNSPELL_READER_H_

#include <string>

namespace convert_dict {

std::string ReadLine(FILE* file);

void TrimLine(std::string* line);

void StripComment(std::string* line);

}  

#endif  

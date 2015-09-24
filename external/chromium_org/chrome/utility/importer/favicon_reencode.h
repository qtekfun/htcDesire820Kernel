// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMPORTER_FAVICON_REENCODE_H_
#define CHROME_UTILITY_IMPORTER_FAVICON_REENCODE_H_

#include <vector>

#include "base/basictypes.h"

namespace importer {

bool ReencodeFavicon(const unsigned char* src_data,
                     size_t src_len,
                     std::vector<unsigned char>* png_data);

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_ADDRESS_PARSER_H_
#define CONTENT_COMMON_ADDRESS_PARSER_H_

#include "base/strings/string16.h"
#include "content/common/content_export.h"

namespace content {

namespace address_parser {

bool FindAddress(const base::string16& text, base::string16* address);

CONTENT_EXPORT bool FindAddress(const base::string16::const_iterator& begin,
                                const base::string16::const_iterator& end,
                                size_t* start_pos,
                                size_t* end_pos);

}  

}  

#endif  

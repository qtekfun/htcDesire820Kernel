// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_QUOTED_PRINTABLE_H_
#define CHROME_BROWSER_NET_QUOTED_PRINTABLE_H_
#pragma once

#include <string>


namespace chrome {
namespace browser {
namespace net {

void QuotedPrintableEncode(const std::string& input, std::string* output);

bool QuotedPrintableDecode(const std::string& input, std::string* output);

int IsEOL(const std::string::const_iterator& iter, const std::string& input);

}  
}  
}  

#endif  

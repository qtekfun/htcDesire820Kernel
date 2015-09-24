// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef I18N_ADDRESSINPUT_UTIL_MD5_H_
#define I18N_ADDRESSINPUT_UTIL_MD5_H_

#include <string>

namespace i18n {
namespace addressinput {



struct MD5Digest {
  unsigned char a[16];
};

typedef char MD5Context[88];

void MD5Sum(const void* data, size_t length, MD5Digest* digest);

void MD5Init(MD5Context* context);

void MD5Update(MD5Context* context, const std::string& data);

void MD5Final(MD5Digest* digest, MD5Context* context);

std::string MD5DigestToBase16(const MD5Digest& digest);

std::string MD5String(const std::string& str);

}  
}  

#endif  

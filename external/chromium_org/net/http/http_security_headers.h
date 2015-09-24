// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_SECURITY_HEADERS_H_
#define NET_HTTP_HTTP_SECURITY_HEADERS_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "base/values.h"
#include "net/base/hash_value.h"
#include "net/base/net_export.h"

namespace net {

const int64 kMaxHSTSAgeSecs = 86400 * 365;  

bool NET_EXPORT_PRIVATE ParseHSTSHeader(const std::string& value,
                                        base::TimeDelta* max_age,
                                        bool* include_subdomains);

bool NET_EXPORT_PRIVATE ParseHPKPHeader(const std::string& value,
                                        const HashValueVector& chain_hashes,
                                        base::TimeDelta* max_age,
                                        bool* include_subdomains,
                                        HashValueVector* hashes);

}  

#endif  

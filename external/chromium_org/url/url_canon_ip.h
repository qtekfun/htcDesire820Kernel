// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_CANON_IP_H_
#define URL_URL_CANON_IP_H_

#include "base/strings/string16.h"
#include "url/url_canon.h"
#include "url/url_export.h"
#include "url/url_parse.h"

namespace url_canon {

URL_EXPORT void AppendIPv4Address(const unsigned char address[4],
                                  CanonOutput* output);

URL_EXPORT void AppendIPv6Address(const unsigned char address[16],
                                  CanonOutput* output);

URL_EXPORT bool FindIPv4Components(const char* spec,
                                   const url_parse::Component& host,
                                   url_parse::Component components[4]);
URL_EXPORT bool FindIPv4Components(const base::char16* spec,
                                   const url_parse::Component& host,
                                   url_parse::Component components[4]);

URL_EXPORT CanonHostInfo::Family IPv4AddressToNumber(
    const char* spec,
    const url_parse::Component& host,
    unsigned char address[4],
    int* num_ipv4_components);
URL_EXPORT CanonHostInfo::Family IPv4AddressToNumber(
    const base::char16* spec,
    const url_parse::Component& host,
    unsigned char address[4],
    int* num_ipv4_components);

URL_EXPORT bool IPv6AddressToNumber(const char* spec,
                                    const url_parse::Component& host,
                                    unsigned char address[16]);
URL_EXPORT bool IPv6AddressToNumber(const base::char16* spec,
                                    const url_parse::Component& host,
                                    unsigned char address[16]);

}  

#endif  

// Copyright 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GOOGLEURL_SRC_URL_CANON_IP_H__
#define GOOGLEURL_SRC_URL_CANON_IP_H__

#include "base/string16.h"
#include "googleurl/src/url_canon.h"
#include "googleurl/src/url_common.h"
#include "googleurl/src/url_parse.h"

namespace url_canon {

GURL_API bool FindIPv4Components(const char* spec,
                                 const url_parse::Component& host,
                                 url_parse::Component components[4]);
GURL_API bool FindIPv4Components(const char16* spec,
                                 const url_parse::Component& host,
                                 url_parse::Component components[4]);

GURL_API CanonHostInfo::Family IPv4AddressToNumber(
    const char* spec,
    const url_parse::Component& host,
    unsigned char address[4],
    int* num_ipv4_components);
GURL_API CanonHostInfo::Family IPv4AddressToNumber(
    const char16* spec,
    const url_parse::Component& host,
    unsigned char address[4],
    int* num_ipv4_components);

GURL_API bool IPv6AddressToNumber(const char* spec,
                                  const url_parse::Component& host,
                                  unsigned char address[16]);
GURL_API bool IPv6AddressToNumber(const char16* spec,
                                  const url_parse::Component& host,
                                  unsigned char address[16]);

}  

#endif  

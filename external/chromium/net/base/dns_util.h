// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_DNS_UTIL_H_
#define NET_BASE_DNS_UTIL_H_
#pragma once

#include <string>

#include "base/basictypes.h"

namespace net {

bool DNSDomainFromDot(const std::string& dotted, std::string* out);

std::string DNSDomainToString(const std::string& domain);

bool IsSTD3ASCIIValidCharacter(char c);

std::string TrimEndingDot(const std::string& host);


static const uint16 kDNS_CNAME = 5;
static const uint16 kDNS_TXT = 16;
static const uint16 kDNS_CERT = 37;
static const uint16 kDNS_DS = 43;
static const uint16 kDNS_RRSIG = 46;
static const uint16 kDNS_DNSKEY = 48;
static const uint16 kDNS_ANY = 0xff;
static const uint16 kDNS_CAA = 13172;  
static const uint16 kDNS_TESTING = 0xfffe;  

static const uint8 kDNSSEC_RSA_SHA1 = 5;
static const uint8 kDNSSEC_RSA_SHA1_NSEC3 = 7;
static const uint8 kDNSSEC_RSA_SHA256 = 8;

static const uint8 kDNSSEC_SHA1 = 1;
static const uint8 kDNSSEC_SHA256 = 2;

}  

#endif  

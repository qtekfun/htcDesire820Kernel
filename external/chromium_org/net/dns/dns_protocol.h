// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_PROTOCOL_H_
#define NET_DNS_DNS_PROTOCOL_H_

#include "base/basictypes.h"
#include "net/base/net_export.h"

namespace net {

namespace dns_protocol {

static const uint16 kDefaultPort = 53;
static const uint16 kDefaultPortMulticast = 5353;





#pragma pack(push)
#pragma pack(1)

struct NET_EXPORT_PRIVATE Header {
  uint16 id;
  uint16 flags;
  uint16 qdcount;
  uint16 ancount;
  uint16 nscount;
  uint16 arcount;
};

#pragma pack(pop)

static const uint8 kLabelMask = 0xc0;
static const uint8 kLabelPointer = 0xc0;
static const uint8 kLabelDirect = 0x0;
static const uint16 kOffsetMask = 0x3fff;

static const uint16 kMDnsClassMask = 0x7FFF;

static const int kMaxNameLength = 255;

static const int kMaxUDPSize = 512;

static const int kMaxMulticastSize = 9000;

static const uint16 kClassIN = 1;

static const uint16 kTypeA = 1;
static const uint16 kTypeCNAME = 5;
static const uint16 kTypePTR = 12;
static const uint16 kTypeTXT = 16;
static const uint16 kTypeAAAA = 28;
static const uint16 kTypeSRV = 33;
static const uint16 kTypeNSEC = 47;


static const uint8 kRcodeMask = 0xf;
static const uint8 kRcodeNOERROR = 0;
static const uint8 kRcodeFORMERR = 1;
static const uint8 kRcodeSERVFAIL = 2;
static const uint8 kRcodeNXDOMAIN = 3;
static const uint8 kRcodeNOTIMP = 4;
static const uint8 kRcodeREFUSED = 5;

static const uint16 kFlagResponse = 0x8000;
static const uint16 kFlagRA = 0x80;
static const uint16 kFlagRD = 0x100;
static const uint16 kFlagTC = 0x200;
static const uint16 kFlagAA = 0x400;

}  

}  

#endif  

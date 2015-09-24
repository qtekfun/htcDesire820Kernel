// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_ASN1_UTIL_H_
#define NET_BASE_ASN1_UTIL_H_
#pragma once

#include "base/string_piece.h"

namespace net {

namespace asn1 {

static const unsigned kINTEGER = 0x02;
static const unsigned kOID = 0x06;
static const unsigned kSEQUENCE = 0x30;

static const unsigned kContextSpecific = 0x80;
static const unsigned kCompound = 0x20;

bool ParseElement(base::StringPiece* in,
                  unsigned tag_value,
                  base::StringPiece* out,
                  unsigned *out_header_len);

bool GetElement(base::StringPiece* in,
                unsigned tag_value,
                base::StringPiece* out);


bool ExtractSPKIFromDERCert(base::StringPiece cert,
                            base::StringPiece* spki_out);

} 

} 

#endif 

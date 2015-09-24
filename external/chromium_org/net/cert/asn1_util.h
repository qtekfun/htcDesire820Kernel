// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_ASN1_UTIL_H_
#define NET_CERT_ASN1_UTIL_H_

#include <vector>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

namespace net {

namespace asn1 {

static const unsigned kBOOLEAN = 0x01;
static const unsigned kINTEGER = 0x02;
static const unsigned kBITSTRING = 0x03;
static const unsigned kOCTETSTRING = 0x04;
static const unsigned kOID = 0x06;
static const unsigned kSEQUENCE = 0x30;

static const unsigned kContextSpecific = 0x80;
static const unsigned kConstructed = 0x20;

static const unsigned kAny = 0x10000;
static const unsigned kOptional = 0x20000;

bool ParseElement(base::StringPiece* in,
                  unsigned tag_value,
                  base::StringPiece* out,
                  unsigned *out_header_len);

bool GetElement(base::StringPiece* in,
                unsigned tag_value,
                base::StringPiece* out);

NET_EXPORT_PRIVATE bool ExtractSPKIFromDERCert(base::StringPiece cert,
                                               base::StringPiece* spki_out);

NET_EXPORT_PRIVATE bool ExtractSubjectPublicKeyFromSPKI(
    base::StringPiece spki,
    base::StringPiece* spk_out);

NET_EXPORT_PRIVATE bool ExtractCRLURLsFromDERCert(
    base::StringPiece cert,
    std::vector<base::StringPiece>* urls_out);

} 

} 

#endif 

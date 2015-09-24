// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CT_SERIALIZATION_H_
#define NET_CERT_CT_SERIALIZATION_H_

#include <string>
#include <vector>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/cert/signed_certificate_timestamp.h"

namespace net {

namespace ct {

NET_EXPORT_PRIVATE bool EncodeDigitallySigned(const DigitallySigned& input,
                                              std::string* output);

NET_EXPORT_PRIVATE bool DecodeDigitallySigned(base::StringPiece* input,
                                              DigitallySigned* output);

NET_EXPORT_PRIVATE bool EncodeLogEntry(const LogEntry& input,
                                       std::string* output);

NET_EXPORT_PRIVATE bool EncodeV1SCTSignedData(
    const base::Time& timestamp,
    const std::string& serialized_log_entry,
    const std::string& extensions,
    std::string* output);

NET_EXPORT_PRIVATE bool DecodeSCTList(base::StringPiece* input,
                                      std::vector<base::StringPiece>* output);

NET_EXPORT_PRIVATE bool DecodeSignedCertificateTimestamp(
    base::StringPiece* input,
    scoped_refptr<ct::SignedCertificateTimestamp>* output);

NET_EXPORT_PRIVATE bool EncodeSCTListForTesting(const base::StringPiece& sct,
                                                std::string* output);
}  

}  

#endif  

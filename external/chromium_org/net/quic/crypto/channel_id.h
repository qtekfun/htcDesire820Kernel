// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_CHANNEL_ID_H_
#define NET_QUIC_CRYPTO_CHANNEL_ID_H_

#include <string>

#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE ChannelIDSigner {
 public:
  virtual ~ChannelIDSigner() { }

  
  
  
  virtual bool Sign(const std::string& hostname,
                    base::StringPiece signed_data,
                    std::string* out_key,
                    std::string* out_signature) = 0;

  
  
  virtual std::string GetKeyForHostname(const std::string& hostname) = 0;
};

class NET_EXPORT_PRIVATE ChannelIDVerifier {
 public:
  
  
  
  static const char kContextStr[];
  
  
  
  static const char kClientToServerStr[];

  
  
  static bool Verify(base::StringPiece key,
                     base::StringPiece signed_data,
                     base::StringPiece signature);

  
  
  
  
  static bool VerifyRaw(base::StringPiece key,
                        base::StringPiece signed_data,
                        base::StringPiece signature,
                        bool is_channel_id_signature);
};

}  

#endif  

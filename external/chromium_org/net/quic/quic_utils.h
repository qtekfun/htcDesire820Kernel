// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_UTILS_H_
#define NET_QUIC_QUIC_UTILS_H_

#include "net/base/int128.h"
#include "net/base/net_export.h"
#include "net/quic/quic_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE QuicUtils {
 public:
  enum Priority {
    LOCAL_PRIORITY,
    PEER_PRIORITY,
  };

  
  
  static uint64 FNV1a_64_Hash(const char* data, int len);

  
  
  static uint128 FNV1a_128_Hash(const char* data, int len);

  
  
  
  
  
  
  
  // match in |their_tags| is written to |out_index|.
  static bool FindMutualTag(const QuicTagVector& our_tags,
                            const QuicTag* their_tags,
                            size_t num_their_tags,
                            Priority priority,
                            QuicTag* out_result,
                            size_t* out_index);

  
  static void SerializeUint128(uint128 v, uint8* out);

  
  
  static void SerializeUint128Short(uint128 v, uint8* out);

  
  static const char* StreamErrorToString(QuicRstStreamErrorCode error);

  
  static const char* ErrorToString(QuicErrorCode error);

  
  static const char* EncryptionLevelToString(EncryptionLevel level);

  
  
  
  
  static std::string TagToString(QuicTag tag);

  
  
  
  
  
  static std::string StringToHexASCIIDump(base::StringPiece in_buffer);

  static char* AsChars(unsigned char* data) {
    return reinterpret_cast<char*>(data);
  }

  static QuicPriority LowestPriority();

  static QuicPriority HighestPriority();
};

inline IOVector MakeIOVector(base::StringPiece str) {
  IOVector iov;
  iov.Append(const_cast<char*>(str.data()), str.size());
  return iov;
}

}  

#endif  

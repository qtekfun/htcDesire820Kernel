// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_BANDWIDTH_H_
#define NET_QUIC_QUIC_BANDWIDTH_H_

#include "base/basictypes.h"
#include "net/quic/quic_time.h"

namespace net {

typedef uint64 QuicByteCount;

class NET_EXPORT_PRIVATE QuicBandwidth {

 public:
  
  static QuicBandwidth Zero();

  
  static QuicBandwidth FromBitsPerSecond(int64 bits_per_second);

  
  static QuicBandwidth FromKBitsPerSecond(int64 k_bits_per_second);

  
  static QuicBandwidth FromBytesPerSecond(int64 bytes_per_second);

  
  static QuicBandwidth FromKBytesPerSecond(int64 k_bytes_per_second);

  
  static QuicBandwidth FromBytesAndTimeDelta(QuicByteCount bytes,
                                             QuicTime::Delta delta);

  int64 ToBitsPerSecond() const;

  int64 ToKBitsPerSecond() const;

  int64 ToBytesPerSecond() const;

  int64 ToKBytesPerSecond() const;

  QuicByteCount ToBytesPerPeriod(QuicTime::Delta time_period) const;

  int64 ToKBytesPerPeriod(QuicTime::Delta time_period) const;

  bool IsZero() const;

  QuicBandwidth Add(const QuicBandwidth& delta) const;

  QuicBandwidth Subtract(const QuicBandwidth& delta) const;

  QuicBandwidth Scale(float scale_factor) const;

  QuicTime::Delta TransferTime(QuicByteCount bytes) const;

 private:
  explicit QuicBandwidth(int64 bits_per_second);
  int64 bits_per_second_;
};

inline bool operator==(QuicBandwidth lhs, QuicBandwidth rhs) {
  return lhs.ToBitsPerSecond() == rhs.ToBitsPerSecond();
}
inline bool operator!=(QuicBandwidth lhs, QuicBandwidth rhs) {
  return !(lhs == rhs);
}
inline bool operator<(QuicBandwidth lhs, QuicBandwidth rhs) {
  return lhs.ToBitsPerSecond() < rhs.ToBitsPerSecond();
}
inline bool operator>(QuicBandwidth lhs, QuicBandwidth rhs) {
  return rhs < lhs;
}
inline bool operator<=(QuicBandwidth lhs, QuicBandwidth rhs) {
  return !(rhs < lhs);
}
inline bool operator>=(QuicBandwidth lhs, QuicBandwidth rhs) {
  return !(lhs < rhs);
}

}  
#endif  

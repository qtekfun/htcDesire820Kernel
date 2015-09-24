// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_NODE_ORDINAL_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_NODE_ORDINAL_H_

#include "base/basictypes.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/ordinal.h"

namespace syncer {


struct NodeOrdinalTraits {
  static const uint8 kZeroDigit = 0;
  static const uint8 kMaxDigit = kuint8max;
  static const size_t kMinLength = 8;
};

typedef Ordinal<NodeOrdinalTraits> NodeOrdinal;

COMPILE_ASSERT(static_cast<char>(NodeOrdinal::kZeroDigit) == '\x00',
               NodeOrdinalHasCorrectZeroDigit);
COMPILE_ASSERT(static_cast<char>(NodeOrdinal::kOneDigit) == '\x01',
               NodeOrdinalHasCorrectOneDigit);
COMPILE_ASSERT(static_cast<char>(NodeOrdinal::kMidDigit) == '\x80',
               NodeOrdinalHasCorrectMidDigit);
COMPILE_ASSERT(static_cast<char>(NodeOrdinal::kMaxDigit) == '\xff',
               NodeOrdinalHasCorrectMaxDigit);
COMPILE_ASSERT(NodeOrdinal::kMidDigitValue == 128,
               NodeOrdinalHasCorrectMidDigitValue);
COMPILE_ASSERT(NodeOrdinal::kMaxDigitValue == 255,
               NodeOrdinalHasCorrectMaxDigitValue);
COMPILE_ASSERT(NodeOrdinal::kRadix == 256,
               NodeOrdinalHasCorrectRadix);

SYNC_EXPORT_PRIVATE NodeOrdinal Int64ToNodeOrdinal(int64 x);

SYNC_EXPORT_PRIVATE int64 NodeOrdinalToInt64(const NodeOrdinal& ordinal);

}  

#endif  

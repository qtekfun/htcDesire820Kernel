// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_UNIQUE_POSITION_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_UNIQUE_POSITION_H_

#include <string>

#include "base/basictypes.h"
#include "sync/base/sync_export.h"

namespace sync_pb {
class UniquePosition;
}

namespace syncer {

class SYNC_EXPORT_PRIVATE UniquePosition {
 public:
  static const size_t kSuffixLength;
  static const size_t kCompressBytesThreshold;

  static bool IsValidSuffix(const std::string& suffix);
  static bool IsValidBytes(const std::string& bytes);

  
  static UniquePosition CreateInvalid();

  
  
  static UniquePosition FromProto(const sync_pb::UniquePosition& proto);

  
  
  
  static UniquePosition FromInt64(int64 i, const std::string& suffix);

  
  static UniquePosition InitialPosition(const std::string& suffix);

  
  
  static UniquePosition Before(const UniquePosition& x,
                               const std::string& suffix);
  static UniquePosition After(const UniquePosition& x,
                              const std::string& suffix);
  static UniquePosition Between(const UniquePosition& before,
                                const UniquePosition& after,
                                const std::string& suffix);

  
  UniquePosition();

  bool LessThan(const UniquePosition& other) const;
  bool Equals(const UniquePosition& other) const;

  
  void ToProto(sync_pb::UniquePosition* proto) const;

  
  void SerializeToString(std::string* blob) const;

  
  std::string ToDebugString() const;

  
  std::string GetSuffixForTest() const;

  
  
  
  int64 ToInt64() const;

  bool IsValid() const;

 private:
  friend class UniquePositionTest;

  
  
  
  static std::string FindSmallerWithSuffix(const std::string& str,
                                           const std::string& suffix);
  
  
  
  static std::string FindGreaterWithSuffix(const std::string& str,
                                           const std::string& suffix);
  
  
  
  static std::string FindBetweenWithSuffix(const std::string& before,
                                           const std::string& after,
                                           const std::string& suffix);

  
  explicit UniquePosition(const std::string& internal_rep);

  
  
  UniquePosition(const std::string& uncompressed, const std::string& suffix);

  
  static std::string Compress(const std::string& input);
  static std::string CompressImpl(const std::string& input);
  static std::string Uncompress(const std::string& compressed);
  static bool IsValidCompressed(const std::string& str);

  
  
  std::string compressed_;
  bool is_valid_;
};

}  

#endif  

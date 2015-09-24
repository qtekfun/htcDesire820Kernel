// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VISITEDLINK_COMMON_VISITEDLINK_COMMON_H_
#define COMPONENTS_VISITEDLINK_COMMON_VISITEDLINK_COMMON_H_

#include <vector>

#include "base/basictypes.h"

class GURL;

namespace visitedlink {

#define LINK_SALT_LENGTH 8

class VisitedLinkCommon {
 public:
  
  typedef uint64 Fingerprint;
  typedef std::vector<Fingerprint> Fingerprints;

  
  typedef int32 Hash;

  
  static const Fingerprint null_fingerprint_;
  static const Hash null_hash_;

  VisitedLinkCommon();
  virtual ~VisitedLinkCommon();

  
  Fingerprint ComputeURLFingerprint(const char* canonical_url,
                                    size_t url_len) const {
    return ComputeURLFingerprint(canonical_url, url_len, salt_);
  }

  
  
  
  bool IsVisited(const char* canonical_url, size_t url_len) const;
  bool IsVisited(const GURL& url) const;
  bool IsVisited(Fingerprint fingerprint) const;

#ifdef UNIT_TEST
  
  void GetUsageStatistics(int32* table_size,
                          VisitedLinkCommon::Fingerprint** fingerprints) {
    *table_size = table_length_;
    *fingerprints = hash_table_;
  }
#endif

 protected:
  
  
  struct SharedHeader {
    
    uint32 length;

    
    uint8 salt[LINK_SALT_LENGTH];
  };

  
  
  
  Fingerprint FingerprintAt(int32 table_offset) const {
    if (!hash_table_)
      return null_fingerprint_;
    return hash_table_[table_offset];
  }

  
  
  
  
  static Fingerprint ComputeURLFingerprint(const char* canonical_url,
                                           size_t url_len,
                                           const uint8 salt[LINK_SALT_LENGTH]);

  
  
  static Hash HashFingerprint(Fingerprint fingerprint, int32 table_length) {
    if (table_length == 0)
      return null_hash_;
    return static_cast<Hash>(fingerprint % table_length);
  }
  
  Hash HashFingerprint(Fingerprint fingerprint) const {
    return HashFingerprint(fingerprint, table_length_);
  }

  
  VisitedLinkCommon::Fingerprint* hash_table_;

  
  int32 table_length_;

  
  uint8 salt_[LINK_SALT_LENGTH];

 private:
  DISALLOW_COPY_AND_ASSIGN(VisitedLinkCommon);
};

}  

#endif  

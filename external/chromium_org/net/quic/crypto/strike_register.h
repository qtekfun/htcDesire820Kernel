// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_STRIKE_REGISTER_H_
#define NET_QUIC_CRYPTO_STRIKE_REGISTER_H_

#include <set>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE StrikeRegister {
 public:
  enum StartupType {
    
    
    
    
    
    
    
    
    DENY_REQUESTS_AT_STARTUP,
    
    
    
    
    NO_STARTUP_PERIOD_NEEDED,
  };

  
  static const uint32 kExternalNodeSize;

  
  
  
  
  static const uint32 kNil;

  
  
  static const uint32 kExternalFlag;

  
  
  
  
  
  
  
  
  StrikeRegister(unsigned max_entries,
                 uint32 current_time_external,
                 uint32 window_secs,
                 const uint8 orbit[8],
                 StartupType startup);

  ~StrikeRegister();

  void Reset();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Insert(const uint8 nonce[32], const uint32 current_time);

  
  
  const uint8* orbit() const;

  
  void Validate();

 private:
  class InternalNode;

  
  static uint32 TimeFromBytes(const uint8 d[4]);

  
  
  uint32 ExternalTimeToInternal(uint32 external_time);

  
  
  uint32 BestMatch(const uint8 v[24]) const;

  
  
  uint32& external_node_next_ptr(unsigned i);

  uint8* external_node(unsigned i);

  uint32 GetFreeExternalNode();

  uint32 GetFreeInternalNode();

  
  
  void DropNode();

  void FreeExternalNode(uint32 index);

  void FreeInternalNode(uint32 index);

  void ValidateTree(uint32 internal_node,
                    int last_bit,
                    const std::vector<std::pair<unsigned, bool> >& bits,
                    const std::set<uint32>& free_internal_nodes,
                    const std::set<uint32>& free_external_nodes,
                    std::set<uint32>* used_internal_nodes,
                    std::set<uint32>* used_external_nodes);

  const uint32 max_entries_;
  const uint32 window_secs_;
  
  
  const uint32 internal_epoch_;
  uint8 orbit_[8];
  uint32 horizon_;
  bool horizon_valid_;

  uint32 internal_node_free_head_;
  uint32 external_node_free_head_;
  uint32 internal_node_head_;
  
  
  InternalNode* internal_nodes_;
  scoped_ptr<uint8[]> external_nodes_;
};

}  

#endif  

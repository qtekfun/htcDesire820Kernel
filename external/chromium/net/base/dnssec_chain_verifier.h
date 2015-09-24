// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_DNSSEC_CHAIN_VERIFIER_H_
#define NET_BASE_DNSSEC_CHAIN_VERIFIER_H_

#include <map>
#include <string>
#include <vector>

#include "base/string_piece.h"

namespace net {

class DNSSECChainVerifier {
 public:
  enum Error {
    OK = 0,
    BAD_DATA,  
    UNKNOWN_ROOT_KEY,  
    UNKNOWN_DIGEST,  
    UNKNOWN_TERMINAL_RRTYPE,  
    BAD_SIGNATURE,  
    NO_DS_LINK,  
    OFF_COURSE,  
    BAD_TARGET,  
  };

  
  
  
  
  DNSSECChainVerifier(const std::string& target,
                      const base::StringPiece& chain);
  ~DNSSECChainVerifier();

  
  
  void IgnoreTimestamps();

  
  Error Verify();

  
  
  uint16 rrtype() const;
  
  
  const std::vector<base::StringPiece>& rrdatas() const;

  
  
  
  
  static std::map<std::string, std::string>
  ParseTLSTXTRecord(base::StringPiece rrdata);

  
  static unsigned MatchingLabels(base::StringPiece a,
                                 base::StringPiece b);

 private:
  struct Zone;

  bool U8(uint8*);
  bool U16(uint16*);
  bool VariableLength16(base::StringPiece*);
  bool ReadName(base::StringPiece*);

  bool ReadAheadEntryKey(base::StringPiece*);
  bool ReadAheadKey(base::StringPiece*, uint8 entry_key);
  bool ReadDNSKEYs(std::vector<base::StringPiece>*, bool is_root);
  bool DigestKey(base::StringPiece* digest,
                 const base::StringPiece& name,
                 const base::StringPiece& dnskey,
                 uint8 digest_type,
                 uint16 keyid,
                 uint8 algorithm);

  Error EnterRoot();
  Error EnterZone(const base::StringPiece& zone);
  Error LeaveZone(base::StringPiece* next_name);
  Error ReadDSSet(std::vector<base::StringPiece>*,
                  const base::StringPiece& next_name);
  Error ReadGenericRRs(std::vector<base::StringPiece>*);
  Error ReadCNAME(std::vector<base::StringPiece>*);

  Zone* current_zone_;
  std::string target_;
  base::StringPiece chain_;
  bool ignore_timestamps_;
  bool valid_;
  
  
  bool already_entered_zone_;
  uint16 rrtype_;
  std::vector<base::StringPiece> rrdatas_;
  
  std::vector<void*> scratch_pool_;
};

}  

#endif  

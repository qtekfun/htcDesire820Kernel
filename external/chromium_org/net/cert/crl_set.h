// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_CERT_CRL_SET_H_
#define NET_CERT_CRL_SET_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/cert/x509_cert_types.h"

namespace base {
class DictionaryValue;
}

namespace net {

class NET_EXPORT CRLSet : public base::RefCountedThreadSafe<CRLSet> {
 public:
  enum Result {
    REVOKED,  
    UNKNOWN,  
    GOOD,     
  };

  
  
  static bool Parse(base::StringPiece data,
                    scoped_refptr<CRLSet>* out_crl_set);

  
  
  Result CheckSPKI(const base::StringPiece& spki_hash) const;

  
  
  
  
  
  Result CheckSerial(
      const base::StringPiece& serial_number,
      const base::StringPiece& issuer_spki_hash) const;

  
  
  bool IsExpired() const;

  
  
  bool ApplyDelta(const base::StringPiece& delta_bytes,
                  scoped_refptr<CRLSet>* out_crl_set);

  
  
  
  static bool GetIsDeltaUpdate(const base::StringPiece& bytes, bool *is_delta);

  
  
  
  std::string Serialize() const;

  
  
  
  uint32 sequence() const;

  
  
  typedef std::vector< std::pair<std::string, std::vector<std::string> > >
      CRLList;

  
  
  const CRLList& crls() const;

  
  static CRLSet* EmptyCRLSetForTesting();

  
  static CRLSet* ExpiredCRLSetForTesting();

  
  
  
  
  
  static CRLSet* ForTesting(bool is_expired,
                            const SHA256HashValue* issuer_spki,
                            const std::string& serial_number);

 private:
  CRLSet();
  ~CRLSet();

  friend class base::RefCountedThreadSafe<CRLSet>;

  
  
  bool CopyBlockedSPKIsFromHeader(base::DictionaryValue* header_dict);

  uint32 sequence_;
  CRLList crls_;
  
  
  uint64 not_after_;
  
  
  
  
  std::map<std::string, size_t> crls_index_by_issuer_;
  
  
  std::vector<std::string> blocked_spkis_;
};

}  

#endif  

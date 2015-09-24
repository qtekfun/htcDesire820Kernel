// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_RECORD_RDATA_H_
#define NET_DNS_RECORD_RDATA_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/big_endian.h"
#include "net/base/net_export.h"
#include "net/base/net_util.h"
#include "net/dns/dns_protocol.h"

namespace net {

class DnsRecordParser;

class NET_EXPORT_PRIVATE RecordRdata {
 public:
  virtual ~RecordRdata() {}

  virtual bool IsEqual(const RecordRdata* other) const = 0;
  virtual uint16 Type() const = 0;

 protected:
  RecordRdata();

  DISALLOW_COPY_AND_ASSIGN(RecordRdata);
};

class NET_EXPORT_PRIVATE SrvRecordRdata : public RecordRdata {
 public:
  static const uint16 kType = dns_protocol::kTypeSRV;

  virtual ~SrvRecordRdata();
  static scoped_ptr<SrvRecordRdata> Create(const base::StringPiece& data,
                                           const DnsRecordParser& parser);

  virtual bool IsEqual(const RecordRdata* other) const OVERRIDE;
  virtual uint16 Type() const OVERRIDE;

  uint16 priority() const { return priority_; }
  uint16 weight() const { return weight_; }
  uint16 port() const { return port_; }

  const std::string& target() const { return target_; }

 private:
  SrvRecordRdata();

  uint16 priority_;
  uint16 weight_;
  uint16 port_;

  std::string target_;

  DISALLOW_COPY_AND_ASSIGN(SrvRecordRdata);
};

class NET_EXPORT_PRIVATE ARecordRdata : public RecordRdata {
 public:
  static const uint16 kType = dns_protocol::kTypeA;

  virtual ~ARecordRdata();
  static scoped_ptr<ARecordRdata> Create(const base::StringPiece& data,
                                         const DnsRecordParser& parser);
  virtual bool IsEqual(const RecordRdata* other) const OVERRIDE;
  virtual uint16 Type() const OVERRIDE;

  const IPAddressNumber& address() const { return address_; }

 private:
  ARecordRdata();

  IPAddressNumber address_;

  DISALLOW_COPY_AND_ASSIGN(ARecordRdata);
};

class NET_EXPORT_PRIVATE AAAARecordRdata : public RecordRdata {
 public:
  static const uint16 kType = dns_protocol::kTypeAAAA;

  virtual ~AAAARecordRdata();
  static scoped_ptr<AAAARecordRdata> Create(const base::StringPiece& data,
                                         const DnsRecordParser& parser);
  virtual bool IsEqual(const RecordRdata* other) const OVERRIDE;
  virtual uint16 Type() const OVERRIDE;

  const IPAddressNumber& address() const { return address_; }

 private:
  AAAARecordRdata();

  IPAddressNumber address_;

  DISALLOW_COPY_AND_ASSIGN(AAAARecordRdata);
};

class NET_EXPORT_PRIVATE CnameRecordRdata : public RecordRdata {
 public:
  static const uint16 kType = dns_protocol::kTypeCNAME;

  virtual ~CnameRecordRdata();
  static scoped_ptr<CnameRecordRdata> Create(const base::StringPiece& data,
                                             const DnsRecordParser& parser);
  virtual bool IsEqual(const RecordRdata* other) const OVERRIDE;
  virtual uint16 Type() const OVERRIDE;

  std::string cname() const { return cname_; }

 private:
  CnameRecordRdata();

  std::string cname_;

  DISALLOW_COPY_AND_ASSIGN(CnameRecordRdata);
};

class NET_EXPORT_PRIVATE PtrRecordRdata : public RecordRdata {
 public:
  static const uint16 kType = dns_protocol::kTypePTR;

  virtual ~PtrRecordRdata();
  static scoped_ptr<PtrRecordRdata> Create(const base::StringPiece& data,
                                           const DnsRecordParser& parser);
  virtual bool IsEqual(const RecordRdata* other) const OVERRIDE;
  virtual uint16 Type() const OVERRIDE;

  std::string ptrdomain() const { return ptrdomain_; }

 private:
  PtrRecordRdata();

  std::string ptrdomain_;

  DISALLOW_COPY_AND_ASSIGN(PtrRecordRdata);
};

class NET_EXPORT_PRIVATE TxtRecordRdata : public RecordRdata {
 public:
  static const uint16 kType = dns_protocol::kTypeTXT;

  virtual ~TxtRecordRdata();
  static scoped_ptr<TxtRecordRdata> Create(const base::StringPiece& data,
                                           const DnsRecordParser& parser);
  virtual bool IsEqual(const RecordRdata* other) const OVERRIDE;
  virtual uint16 Type() const OVERRIDE;

  const std::vector<std::string>& texts() const { return texts_; }

 private:
  TxtRecordRdata();

  std::vector<std::string> texts_;

  DISALLOW_COPY_AND_ASSIGN(TxtRecordRdata);
};

class NET_EXPORT_PRIVATE NsecRecordRdata : public RecordRdata {
 public:
  static const uint16 kType = dns_protocol::kTypeNSEC;

  virtual ~NsecRecordRdata();
  static scoped_ptr<NsecRecordRdata> Create(const base::StringPiece& data,
                                            const DnsRecordParser& parser);
  virtual bool IsEqual(const RecordRdata* other) const OVERRIDE;
  virtual uint16 Type() const OVERRIDE;

  
  unsigned bitmap_length() const { return bitmap_.size() * 8; }

  
  
  
  bool GetBit(unsigned i) const;

 private:
  NsecRecordRdata();

  std::vector<uint8> bitmap_;

  DISALLOW_COPY_AND_ASSIGN(NsecRecordRdata);
};


}  

#endif  

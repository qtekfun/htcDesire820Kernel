// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_DNS_PACKET_PARSER_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_DNS_PACKET_PARSER_H_

#include <string>

#include "net/dns/dns_protocol.h"
#include "net/dns/dns_response.h"

struct DnsQueryRecord {
  DnsQueryRecord() : qtype(0), qclass(0) {}
  ~DnsQueryRecord() {}

  std::string qname;  
  uint16 qtype;
  uint16 qclass;
};

class DnsPacketParser {
 public:
  
  DnsPacketParser(const char* packet, size_t length);

  
  ~DnsPacketParser() {}

  bool IsValid() const { return record_parser_.IsValid() && is_header_read_; }

  
  bool AtEnd() const { return record_parser_.AtEnd(); }

  
  const net::dns_protocol::Header& header() const { return header_; }

  
  bool ReadRecord(DnsQueryRecord* record);

  
  bool ReadRecord(net::DnsResourceRecord* record) {
    return record_parser_.ReadRecord(record);
  }

 private:
  
  size_t GetOffset() const { return record_parser_.GetOffset(); }

  
  
  
  
  
  
  
  unsigned ReadName(std::string* out) const {
    return record_parser_.ReadName(packet_ + GetOffset(), out);
  }

  const char* packet_;
  size_t length_;

  
  net::dns_protocol::Header header_;
  bool is_header_read_;

  
  net::DnsRecordParser record_parser_;

  DISALLOW_COPY_AND_ASSIGN(DnsPacketParser);
};

#endif  


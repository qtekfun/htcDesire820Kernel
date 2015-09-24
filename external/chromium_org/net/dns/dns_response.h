// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_RESPONSE_H_
#define NET_DNS_DNS_RESPONSE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string_piece.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/base/net_util.h"

namespace net {

class AddressList;
class DnsQuery;
class IOBufferWithSize;

namespace dns_protocol {
struct Header;
}

struct NET_EXPORT_PRIVATE DnsResourceRecord {
  DnsResourceRecord();
  ~DnsResourceRecord();

  std::string name;  
  uint16 type;
  uint16 klass;
  uint32 ttl;
  base::StringPiece rdata;  
};

class NET_EXPORT_PRIVATE DnsRecordParser {
 public:
  
  DnsRecordParser();

  
  
  DnsRecordParser(const void* packet, size_t length, size_t offset);

  
  bool IsValid() const { return packet_ != NULL; }

  
  bool AtEnd() const { return cur_ == packet_ + length_; }

  
  size_t GetOffset() const { return cur_ - packet_; }

  
  
  
  
  
  
  
  unsigned ReadName(const void* pos, std::string* out) const;

  
  bool ReadRecord(DnsResourceRecord* record);

  
  bool SkipQuestion();

 private:
  const char* packet_;
  size_t length_;
  
  const char* cur_;
};

class NET_EXPORT_PRIVATE DnsResponse {
 public:
  
  enum Result {
    DNS_PARSE_OK = 0,
    DNS_MALFORMED_RESPONSE,    
                               
    DNS_MALFORMED_CNAME,       
    DNS_NAME_MISMATCH,         
                               
    DNS_SIZE_MISMATCH,         
    DNS_CNAME_AFTER_ADDRESS,   
    DNS_ADDRESS_TTL_MISMATCH,  
    DNS_NO_ADDRESSES,          
    
    DNS_PARSE_RESULT_MAX,      
  };

  
  
  DnsResponse();

  
  explicit DnsResponse(size_t length);

  
  DnsResponse(const void* data, size_t length, size_t answer_offset);

  ~DnsResponse();

  
  
  IOBufferWithSize* io_buffer() { return io_buffer_.get(); }

  
  
  bool InitParse(int nbytes, const DnsQuery& query);

  
  
  bool InitParseWithoutQuery(int nbytes);

  
  bool IsValid() const;

  

  
  uint16 flags() const;  
  uint8 rcode() const;

  unsigned answer_count() const;
  unsigned additional_answer_count() const;

  
  base::StringPiece qname() const;
  uint16 qtype() const;

  
  std::string GetDottedName() const;

  
  
  
  DnsRecordParser Parser() const;

  
  
  Result ParseToAddressList(AddressList* addr_list, base::TimeDelta* ttl) const;

 private:
  
  const dns_protocol::Header* header() const;

  
  scoped_refptr<IOBufferWithSize> io_buffer_;

  
  
  DnsRecordParser parser_;

  DISALLOW_COPY_AND_ASSIGN(DnsResponse);
};

}  

#endif  

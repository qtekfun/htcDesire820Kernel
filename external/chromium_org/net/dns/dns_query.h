// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_QUERY_H_
#define NET_DNS_DNS_QUERY_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

namespace net {

class IOBufferWithSize;

class NET_EXPORT_PRIVATE DnsQuery {
 public:
  
  
  DnsQuery(uint16 id, const base::StringPiece& qname, uint16 qtype);
  ~DnsQuery();

  
  DnsQuery* CloneWithNewId(uint16 id) const;

  
  uint16 id() const;
  base::StringPiece qname() const;
  uint16 qtype() const;

  
  
  base::StringPiece question() const;

  
  IOBufferWithSize* io_buffer() const { return io_buffer_.get(); }

  void set_flags(uint16 flags);

 private:
  DnsQuery(const DnsQuery& orig, uint16 id);

  
  
  size_t qname_size_;

  
  scoped_refptr<IOBufferWithSize> io_buffer_;

  DISALLOW_COPY_AND_ASSIGN(DnsQuery);
};

}  

#endif  

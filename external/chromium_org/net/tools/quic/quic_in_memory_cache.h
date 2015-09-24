// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_IN_MEMORY_CACHE_H_
#define NET_TOOLS_QUIC_QUIC_IN_MEMORY_CACHE_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/memory/singleton.h"
#include "base/strings/string_piece.h"
#include "net/tools/balsa/balsa_frame.h"
#include "net/tools/balsa/balsa_headers.h"
#include "net/tools/balsa/noop_balsa_visitor.h"

template <typename T> struct DefaultSingletonTraits;

namespace net {
namespace tools {

namespace test {
class QuicInMemoryCachePeer;
}  

extern std::string FLAGS_quic_in_memory_cache_dir;

class QuicServer;

class QuicInMemoryCache {
 public:
  
  class Response {
   public:
    Response() {}
    ~Response() {}

    const BalsaHeaders& headers() const { return headers_; }
    const base::StringPiece body() const { return base::StringPiece(body_); }

   private:
    friend class QuicInMemoryCache;

    void set_headers(const BalsaHeaders& headers) {
      headers_.CopyFrom(headers);
    }
    void set_body(base::StringPiece body) {
      body.CopyToString(&body_);
    }

    BalsaHeaders headers_;
    std::string body_;

    DISALLOW_COPY_AND_ASSIGN(Response);
  };

  
  static QuicInMemoryCache* GetInstance();

  
  
  
  const Response* GetResponse(const BalsaHeaders& request_headers) const;

  
  
  void AddSimpleResponse(base::StringPiece method,
                         base::StringPiece path,
                         base::StringPiece version,
                         base::StringPiece response_code,
                         base::StringPiece response_detail,
                         base::StringPiece body);

  
  void AddResponse(const BalsaHeaders& request_headers,
                   const BalsaHeaders& response_headers,
                   base::StringPiece response_body);

 private:
  typedef base::hash_map<std::string, Response*> ResponseMap;
  friend struct DefaultSingletonTraits<QuicInMemoryCache>;
  friend class test::QuicInMemoryCachePeer;

  QuicInMemoryCache();
  ~QuicInMemoryCache();

  void ResetForTests();

  void Initialize();

  std::string GetKey(const BalsaHeaders& response_headers) const;

  
  ResponseMap responses_;

  DISALLOW_COPY_AND_ASSIGN(QuicInMemoryCache);
};

}  
}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_TEST_TOOLS_TEST_TOOLS_HTTP_MESSAGE_TEST_UTILS_H_
#define NET_TOOLS_QUIC_TEST_TOOLS_TEST_TOOLS_HTTP_MESSAGE_TEST_UTILS_H_

#include <string>
#include <vector>

#include "base/strings/string_piece.h"
#include "net/tools/balsa/balsa_enums.h"
#include "net/tools/balsa/balsa_headers.h"

namespace net {
namespace tools {
namespace test {

class HttpConstants {
 public:
  enum Version {
    HTTP_UNKNOWN = 0,
    HTTP_0_9,
    HTTP_1_0,
    HTTP_1_1
  };

  enum Method {
    UNKNOWN_METHOD = 0,
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,

    MKCOL,
    UNLOCK,
  };
};

class HTTPMessage {
 public:
  typedef HttpConstants::Version Version;
  typedef HttpConstants::Method Method;

  
  
  static Version StringToVersion(base::StringPiece str);
  static Method StringToMethod(base::StringPiece str);

  static const char* MethodToString(Method method);
  static const char* VersionToString(Version version);

  
  
  HTTPMessage();

  
  HTTPMessage(Version version, Method request, const std::string& path);

  virtual ~HTTPMessage();

  const std::string& body() const { return body_; }

  
  void AddHeader(const std::string& header, const std::string& value);

  
  void RemoveHeader(const std::string& header);

  
  void ReplaceHeader(const std::string& header, const std::string& value);

  
  
  
  
  
  void AddBody(const std::string& body, bool add_content_length);

  bool has_complete_message() const { return has_complete_message_; }
  void set_has_complete_message(bool value) { has_complete_message_ = value; }

  
  
  
  
  
  void ValidateMessage() const;

  bool skip_message_validation() const { return skip_message_validation_; }
  void set_skip_message_validation(bool value) {
    skip_message_validation_ = value;
  }

  
  
  void set_body(const std::string& body) { body_ = body; }

  const BalsaHeaders* headers() const { return &headers_; }
  BalsaHeaders* headers() { return &headers_; }

 protected:
  BalsaHeaders headers_;

  std::string body_;  

  bool is_request_;

  
  
  
  bool has_complete_message_;

  
  
  bool skip_message_validation_;

 private:
  void InitializeFields();

  DISALLOW_COPY_AND_ASSIGN(HTTPMessage);
};

}  
}  
}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_CHILD_WEBURLRESPONSE_EXTRADATA_IMPL_H_
#define WEBKIT_CHILD_WEBURLRESPONSE_EXTRADATA_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/http/http_response_info.h"
#include "third_party/WebKit/public/platform/WebURLResponse.h"
#include "webkit/child/webkit_child_export.h"

namespace webkit_glue {

class WEBKIT_CHILD_EXPORT WebURLResponseExtraDataImpl :
    public NON_EXPORTED_BASE(blink::WebURLResponse::ExtraData) {
 public:
  explicit WebURLResponseExtraDataImpl(
      const std::string& npn_negotiated_protocol);
  virtual ~WebURLResponseExtraDataImpl();

  const std::string& npn_negotiated_protocol() const {
    return npn_negotiated_protocol_;
  }

  
  
  bool was_fetched_via_proxy() const {
    return was_fetched_via_proxy_;
  }
  void set_was_fetched_via_proxy(bool was_fetched_via_proxy) {
    was_fetched_via_proxy_ = was_fetched_via_proxy;
  }

  
  
  bool was_fetched_via_spdy() const {
    return was_fetched_via_spdy_;
  }
  void set_was_fetched_via_spdy(bool was_fetched_via_spdy) {
    was_fetched_via_spdy_ = was_fetched_via_spdy;
  }

  
  net::HttpResponseInfo::ConnectionInfo connection_info() const {
    return connection_info_;
  }
  void set_connection_info(
      net::HttpResponseInfo::ConnectionInfo connection_info) {
    connection_info_ = connection_info;
  }

  
  
  
  bool was_npn_negotiated() const {
    return was_npn_negotiated_;
  }
  void set_was_npn_negotiated(bool was_npn_negotiated) {
    was_npn_negotiated_ = was_npn_negotiated;
  }

  
  
  bool was_alternate_protocol_available() const {
    return was_alternate_protocol_available_;
  }
  void set_was_alternate_protocol_available(
      bool was_alternate_protocol_available) {
    was_alternate_protocol_available_ = was_alternate_protocol_available;
  }

  bool is_ftp_directory_listing() const { return is_ftp_directory_listing_; }
  void set_is_ftp_directory_listing(bool is_ftp_directory_listing) {
    is_ftp_directory_listing_ = is_ftp_directory_listing;
  }

 private:
  std::string npn_negotiated_protocol_;
  bool is_ftp_directory_listing_;
  bool was_fetched_via_proxy_;
  bool was_fetched_via_spdy_;
  bool was_npn_negotiated_;
  net::HttpResponseInfo::ConnectionInfo connection_info_;
  bool was_alternate_protocol_available_;

  DISALLOW_COPY_AND_ASSIGN(WebURLResponseExtraDataImpl);
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_STATUS_H_
#define NET_URL_REQUEST_URL_REQUEST_STATUS_H_

namespace net {

class URLRequestStatus {
 public:
  enum Status {
    
    SUCCESS = 0,

    
    
    IO_PENDING,

    
    CANCELED,

    
    FAILED,
  };

  URLRequestStatus() : status_(SUCCESS), error_(0) {}
  URLRequestStatus(Status s, int e) : status_(s), error_(e) {}

  Status status() const { return status_; }
  void set_status(Status s) { status_ = s; }

  int error() const { return error_; }
  void set_error(int e) { error_ = e; }

  
  
  bool is_success() const {
    return status_ == SUCCESS || status_ == IO_PENDING;
  }

  
  bool is_io_pending() const {
    return status_ == IO_PENDING;
  }

 private:
  
  Status status_;

  
  int error_;
};

}  

#endif  

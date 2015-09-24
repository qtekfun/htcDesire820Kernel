// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_STATUS_H_
#define NET_URL_REQUEST_URL_REQUEST_STATUS_H_
#pragma once

namespace net {

class URLRequestStatus {
 public:
  enum Status {
    
    SUCCESS = 0,

    
    
    IO_PENDING,

    
    
    
    HANDLED_EXTERNALLY,

    
    CANCELED,

    
    FAILED,
  };

  URLRequestStatus() : status_(SUCCESS), os_error_(0) {}
  URLRequestStatus(Status s, int e) : status_(s), os_error_(e) {}

  Status status() const { return status_; }
  void set_status(Status s) { status_ = s; }

  int os_error() const { return os_error_; }
  void set_os_error(int e) { os_error_ = e; }

  
  
  
  
  bool is_success() const {
    return status_ == SUCCESS || status_ == IO_PENDING;
  }

  
  bool is_io_pending() const {
    return status_ == IO_PENDING;
  }

 private:
  
  Status status_;

  
  
  int os_error_;
};

}  

#endif  

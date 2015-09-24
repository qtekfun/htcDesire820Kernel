// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_COOKIES_COOKIE_OPTIONS_H_
#define NET_COOKIES_COOKIE_OPTIONS_H_

namespace net {

class CookieOptions {
 public:
  
  
  
  CookieOptions()
      : exclude_httponly_(true),
        server_time_() {
  }

  void set_exclude_httponly() { exclude_httponly_ = true; }
  void set_include_httponly() { exclude_httponly_ = false; }
  bool exclude_httponly() const { return exclude_httponly_; }

  
  
  
  void set_server_time(const base::Time& server_time) {
    server_time_ = server_time;
  }
  bool has_server_time() const { return !server_time_.is_null(); }
  base::Time server_time() const { return server_time_; }

 private:
  bool exclude_httponly_;
  base::Time server_time_;
};
}  

#endif  


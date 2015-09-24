// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_BASE_COOKIE_OPTIONS_H_
#define NET_BASE_COOKIE_OPTIONS_H_
#pragma once

namespace net {

class CookieOptions {
 public:
  
  
  
  CookieOptions()
      : exclude_httponly_(true),
        force_session_(false),
        server_time_() {
  }

  void set_exclude_httponly() { exclude_httponly_ = true; }
  void set_include_httponly() { exclude_httponly_ = false; }
  bool exclude_httponly() const { return exclude_httponly_; }

  
  
  
  void set_force_session() { force_session_ = true; }
  bool force_session() const { return force_session_; }

  void set_server_time(const base::Time& server_time) {
     server_time_ = server_time;
  }
  bool has_server_time() const { return !server_time_.is_null(); }
  base::Time server_time() const { return server_time_; }

 private:
  bool exclude_httponly_;
  bool force_session_;
  base::Time server_time_;
};
}  

#endif  


// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_NET_NET_UTIL_H_
#define CHROME_TEST_CHROMEDRIVER_NET_NET_UTIL_H_

#include <string>

class URLRequestContextGetter;

class NetAddress {
 public:
  NetAddress();  
  explicit NetAddress(int port);  
  NetAddress(const std::string& host, int port);
  ~NetAddress();

  bool IsValid() const;

  
  std::string ToString() const;

  const std::string& host() const;
  int port() const;

 private:
  std::string host_;
  int port_;
};

bool FetchUrl(const std::string& url,
              URLRequestContextGetter* getter,
              std::string* response);

#endif  

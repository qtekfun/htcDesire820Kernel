// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_HTTPS_PROBER_H_
#define NET_BASE_HTTPS_PROBER_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/task.h"
#include "net/url_request/url_request.h"

template <typename T> struct DefaultSingletonTraits;

namespace net {

class URLRequestContext;

class HTTPSProberDelegate {
 public:
  virtual void ProbeComplete(bool result) = 0;
 protected:
  virtual ~HTTPSProberDelegate() {}
};

class HTTPSProber : public URLRequest::Delegate {
 public:
  
  static HTTPSProber* GetInstance();

  
  
  bool HaveProbed(const std::string& host) const;

  
  bool InFlight(const std::string& host) const;

  
  
  
  
  
  
  bool ProbeHost(const std::string& host, URLRequestContext* ctx,
                 HTTPSProberDelegate* delegate);

  
  virtual void OnAuthRequired(URLRequest* request,
                              AuthChallengeInfo* auth_info);
  virtual void OnSSLCertificateError(URLRequest* request,
                                     int cert_error,
                                     X509Certificate* cert);
  virtual void OnResponseStarted(URLRequest* request);
  virtual void OnReadCompleted(URLRequest* request, int bytes_read);

 private:
  friend struct DefaultSingletonTraits<HTTPSProber>;

  HTTPSProber();
  ~HTTPSProber();

  void Success(URLRequest* request);
  void Failure(URLRequest* request);
  void DoCallback(URLRequest* request, bool result);

  std::map<std::string, HTTPSProberDelegate*> inflight_probes_;
  std::set<std::string> probed_;

  DISALLOW_COPY_AND_ASSIGN(HTTPSProber);
};

}  

#endif  

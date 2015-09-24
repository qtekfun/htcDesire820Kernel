// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_BASE_NOTIFIER_OPTIONS_H_
#define JINGLE_NOTIFIER_BASE_NOTIFIER_OPTIONS_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "jingle/notifier/base/notification_method.h"
#include "net/base/host_port_pair.h"
#include "net/url_request/url_request_context_getter.h"

namespace notifier {

struct NotifierOptions {
  NotifierOptions();
  ~NotifierOptions();

  
  
  bool try_ssltcp_first;

  
  
  bool allow_insecure_connection;

  
  
  bool invalidate_xmpp_login;

  
  
  net::HostPortPair xmpp_host_port;

  
  
  NotificationMethod notification_method;

  
  std::string auth_mechanism;

  
  scoped_refptr<net::URLRequestContextGetter> request_context_getter;
};

}  

#endif  

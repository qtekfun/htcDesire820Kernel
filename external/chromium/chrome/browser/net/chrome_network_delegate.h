// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CHROME_NETWORK_DELEGATE_H_
#define CHROME_BROWSER_NET_CHROME_NETWORK_DELEGATE_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/profiles/profile.h"
#include "net/base/network_delegate.h"

class ExtensionEventRouterForwarder;
template<class T> class PrefMember;
class ProtocolHandlerRegistry;

typedef PrefMember<bool> BooleanPrefMember;

class ChromeNetworkDelegate : public net::NetworkDelegate {
 public:
  
  
  
  
  
  ChromeNetworkDelegate(
      ExtensionEventRouterForwarder* event_router,
      ProfileId profile_id,
      BooleanPrefMember* enable_referrers,
      ProtocolHandlerRegistry* protocol_handler_registry);
  virtual ~ChromeNetworkDelegate();

  
  
  static void InitializeReferrersEnabled(BooleanPrefMember* enable_referrers,
                                         PrefService* pref_service);
 private:
  
  virtual int OnBeforeURLRequest(net::URLRequest* request,
                                 net::CompletionCallback* callback,
                                 GURL* new_url);
  virtual int OnBeforeSendHeaders(uint64 request_id,
                                  net::CompletionCallback* callback,
                                  net::HttpRequestHeaders* headers);
  virtual void OnResponseStarted(net::URLRequest* request);
  virtual void OnReadCompleted(net::URLRequest* request, int bytes_read);
  virtual void OnURLRequestDestroyed(net::URLRequest* request);
  virtual net::URLRequestJob* OnMaybeCreateURLRequestJob(
      net::URLRequest* request);

  scoped_refptr<ExtensionEventRouterForwarder> event_router_;
  const ProfileId profile_id_;

  
  BooleanPrefMember* enable_referrers_;
  scoped_refptr<ProtocolHandlerRegistry> protocol_handler_registry_;
  DISALLOW_COPY_AND_ASSIGN(ChromeNetworkDelegate);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SSL_SSL_TAB_HELPER_H_
#define CHROME_BROWSER_SSL_SSL_TAB_HELPER_H_

#include <map>

#include "base/callback_forward.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class SSLAddCertHandler;

namespace net {
class HttpNetworkSession;
class SSLCertRequestInfo;
class X509Certificate;
}

class SSLTabHelper : public content::WebContentsObserver,
                     public content::WebContentsUserData<SSLTabHelper> {
 public:
  virtual ~SSLTabHelper();

  
  virtual void DidChangeVisibleSSLState() OVERRIDE;

  
  
  
  
  
  void OnVerifyClientCertificateError(
      scoped_refptr<SSLAddCertHandler> handler, int error_code);

  
  
  void AskToAddClientCertificate(
      scoped_refptr<SSLAddCertHandler> handler);

  
  void OnAddClientCertificateSuccess(
      scoped_refptr<SSLAddCertHandler> handler);

  
  void OnAddClientCertificateError(
      scoped_refptr<SSLAddCertHandler> handler, int error_code);

  
  
  void OnAddClientCertificateFinished(
      scoped_refptr<SSLAddCertHandler> handler);

  
  
  void ShowClientCertificateRequestDialog(
      const net::HttpNetworkSession* network_session,
      net::SSLCertRequestInfo* cert_request_info,
      const base::Callback<void(net::X509Certificate*)>& callback);

 private:
  explicit SSLTabHelper(content::WebContents* contents);
  friend class content::WebContentsUserData<SSLTabHelper>;

  content::WebContents* web_contents_;

  class SSLAddCertData;
  std::map<int, linked_ptr<SSLAddCertData> > request_id_to_add_cert_data_;

  SSLAddCertData* GetAddCertData(SSLAddCertHandler* handler);

  DISALLOW_COPY_AND_ASSIGN(SSLTabHelper);
};

#endif  

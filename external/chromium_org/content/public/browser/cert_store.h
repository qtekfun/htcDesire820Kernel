// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_CERT_STORE_H_
#define CONTENT_PUBLIC_BROWSER_CERT_STORE_H_

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

namespace net {
class X509Certificate;
}

namespace content {

class CertStore  {
 public:
  
  CONTENT_EXPORT static CertStore* GetInstance();

  
  
  
  
  
  virtual int StoreCert(net::X509Certificate* cert,
                        int render_process_host_id) = 0;

  
  
  
  virtual bool RetrieveCert(int cert_id,
                            scoped_refptr<net::X509Certificate>* cert) = 0;

 protected:
   virtual ~CertStore() {}
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_SIGNED_CERTIFICATE_TIMESTAMP_STORE_H_
#define CONTENT_PUBLIC_BROWSER_SIGNED_CERTIFICATE_TIMESTAMP_STORE_H_

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

namespace net {
namespace ct {
struct SignedCertificateTimestamp;
}  
}  

namespace content {

class SignedCertificateTimestampStore {
 public:
  
  CONTENT_EXPORT static SignedCertificateTimestampStore* GetInstance();

  
  
  
  
  
  virtual int Store(net::ct::SignedCertificateTimestamp* sct,
                    int render_process_host_id) = 0;

  
  
  
  virtual bool Retrieve(
      int sct_id, scoped_refptr<net::ct::SignedCertificateTimestamp>* sct) = 0;

 protected:
  virtual ~SignedCertificateTimestampStore() {}
};

}  

#endif  

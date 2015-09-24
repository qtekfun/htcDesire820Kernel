// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_CERTIFICATE_REQUEST_RESULT_TYPE_H_
#define CONTENT_PUBLIC_BROWSER_CERTIFICATE_REQUEST_RESULT_TYPE_H_

namespace content {

enum CertificateRequestResultType {
  
  CERTIFICATE_REQUEST_RESULT_TYPE_CONTINUE,

  
  CERTIFICATE_REQUEST_RESULT_TYPE_CANCEL,

  
  CERTIFICATE_REQUEST_RESULT_TYPE_DENY,
};

}  

#endif  

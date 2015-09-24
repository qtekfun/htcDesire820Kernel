// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CERT_TYPE_H_
#define NET_BASE_CERT_TYPE_H_

namespace net {

enum CertType {
  UNKNOWN_CERT,
  CA_CERT,
  USER_CERT,
  SERVER_CERT,
  NUM_CERT_TYPES
};

}  

#endif  
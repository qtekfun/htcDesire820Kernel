// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_TRANSACTION_DELEGATE_H_
#define NET_HTTP_HTTP_TRANSACTION_DELEGATE_H_

namespace net {

class HttpTransactionDelegate {
 public:
  virtual ~HttpTransactionDelegate() {}
  virtual void OnCacheActionStart() = 0;
  virtual void OnCacheActionFinish() = 0;
  virtual void OnNetworkActionStart() = 0;
  virtual void OnNetworkActionFinish() = 0;
};

}  

#endif  

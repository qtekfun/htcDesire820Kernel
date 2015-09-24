// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_TEST_DATA_STREAM_H_
#define NET_BASE_TEST_DATA_STREAM_H_

#include <string.h>  
#include <algorithm>
#include "net/base/net_export.h"


namespace net {

class NET_EXPORT TestDataStream {
 public:
  TestDataStream();

  
  void GetBytes(char* buffer, int length);

  
  
  bool VerifyBytes(const char *buffer, int length);

  
  void Reset();

 private:
  
  
  void AdvanceIndex();

  
  void Consume(int bytes);

  int index_;
  int bytes_remaining_;
  char buffer_[16];
  char* buffer_ptr_;
};

}  

#endif  

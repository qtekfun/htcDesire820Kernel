// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_LOADER_OFFLINE_POLICY
#define CONTENT_BROWSER_LOADER_OFFLINE_POLICY

#include <map>

#include "base/basictypes.h"
#include "content/common/content_export.h"

struct ResourceHostMsg_Request;

namespace net {
class HttpResponseInfo;
class URLRequest;
}

namespace content {

class CONTENT_EXPORT OfflinePolicy {
 public:
  OfflinePolicy();
  ~OfflinePolicy();

  
  
  
  
  int GetAdditionalLoadFlags(int current_flags, bool reset_state);

  
  void UpdateStateForSuccessfullyStartedRequest(
      const net::HttpResponseInfo& response_info);

private:
  enum State { INIT, ONLINE, OFFLINE };

  void RecordAndResetStats();

  bool enabled_;
  State state_;
  int resource_loads_initiated_;
  int resource_loads_successfully_started_;

  DISALLOW_COPY_AND_ASSIGN(OfflinePolicy);
};

}  

#endif  

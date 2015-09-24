// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_RESPONSE_BODY_DRAINER_H_
#define NET_HTTP_HTTP_RESPONSE_BODY_DRAINER_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer.h"
#include "net/base/completion_callback.h"
#include "net/http/http_network_session.h"

namespace net {

class HttpStream;
class IOBuffer;

class HttpResponseBodyDrainer {
 public:
  
  
  
  
  static const int kDrainBodyBufferSize = 16384;
  static const int kTimeoutInSeconds = 5;

  explicit HttpResponseBodyDrainer(HttpStream* stream);
  ~HttpResponseBodyDrainer();

  
  
  
  void Start(HttpNetworkSession* session);

 private:
  enum State {
    STATE_DRAIN_RESPONSE_BODY,
    STATE_DRAIN_RESPONSE_BODY_COMPLETE,
    STATE_NONE,
  };

  int DoLoop(int result);

  int DoDrainResponseBody();
  int DoDrainResponseBodyComplete(int result);

  void OnIOComplete(int result);
  void OnTimerFired();
  void Finish(int result);

  scoped_refptr<IOBuffer> read_buf_;
  const scoped_ptr<HttpStream> stream_;
  State next_state_;
  int total_read_;
  CompletionCallbackImpl<HttpResponseBodyDrainer> io_callback_;
  CompletionCallback* user_callback_;
  base::OneShotTimer<HttpResponseBodyDrainer> timer_;
  HttpNetworkSession* session_;

  DISALLOW_COPY_AND_ASSIGN(HttpResponseBodyDrainer);
};

}  

#endif 

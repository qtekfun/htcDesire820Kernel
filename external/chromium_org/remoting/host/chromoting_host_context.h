// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_CHROMOTING_HOST_CONTEXT_H_
#define REMOTING_HOST_CHROMOTING_HOST_CONTEXT_H_

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace net {
class URLRequestContextGetter;
}  

namespace remoting {

class AutoThreadTaskRunner;

class ChromotingHostContext {
 public:
  ~ChromotingHostContext();

  
  
  
  
  static scoped_ptr<ChromotingHostContext> Create(
      scoped_refptr<AutoThreadTaskRunner> ui_task_runner);

  
  scoped_refptr<AutoThreadTaskRunner> audio_task_runner();

  
  
  
  scoped_refptr<AutoThreadTaskRunner> file_task_runner();

  
  
  
  
  scoped_refptr<AutoThreadTaskRunner> input_task_runner();

  
  
  
  scoped_refptr<AutoThreadTaskRunner> network_task_runner();

  
  
  scoped_refptr<AutoThreadTaskRunner> ui_task_runner();

  
  
  scoped_refptr<AutoThreadTaskRunner> video_capture_task_runner();

  
  scoped_refptr<AutoThreadTaskRunner> video_encode_task_runner();

  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter();

 private:
  ChromotingHostContext(AutoThreadTaskRunner* ui_task_runner);

  
  scoped_refptr<AutoThreadTaskRunner> audio_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> file_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> input_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> network_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> ui_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> video_capture_task_runner_;

  
  scoped_refptr<AutoThreadTaskRunner> video_encode_task_runner_;

  
  scoped_refptr<net::URLRequestContextGetter> url_request_context_getter_;

  DISALLOW_COPY_AND_ASSIGN(ChromotingHostContext);
};

}  

#endif  

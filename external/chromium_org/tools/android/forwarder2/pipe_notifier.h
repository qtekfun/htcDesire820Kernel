// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_ANDROID_FORWARDER2_PIPE_NOTIFIER_H_
#define TOOLS_ANDROID_FORWARDER2_PIPE_NOTIFIER_H_

#include "base/basictypes.h"

namespace forwarder2 {

class PipeNotifier {
 public:
  PipeNotifier();
  ~PipeNotifier();

  bool Notify();

  int receiver_fd() const { return receiver_fd_; }

 private:
  int sender_fd_;
  int receiver_fd_;

  DISALLOW_COPY_AND_ASSIGN(PipeNotifier);
};

}  

#endif  

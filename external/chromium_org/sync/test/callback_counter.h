// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_CALLBACK_COUNTER_H_
#define SYNC_TEST_CALLBACK_COUNTER_H_

namespace syncer {

class CallbackCounter {
 public:
  CallbackCounter() { Reset(); }
  ~CallbackCounter() {}

  void Reset() { times_called_ = 0; }
  void Callback() { ++times_called_; }
  int times_called() const { return times_called_; }

 private:
  int times_called_;

  DISALLOW_COPY_AND_ASSIGN(CallbackCounter);
};

}  

#endif  

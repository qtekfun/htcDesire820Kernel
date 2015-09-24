// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_NATIVE_MESSAGING_NATIVE_MESSAGING_READER_H_
#define REMOTING_HOST_NATIVE_MESSAGING_NATIVE_MESSAGING_READER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "base/threading/thread.h"

namespace base {
class SequencedTaskRunner;
class Value;
}  

namespace net {
class FileStream;
}  

namespace remoting {

class NativeMessagingReader {
 public:
  typedef base::Callback<void(scoped_ptr<base::Value>)> MessageCallback;

  explicit NativeMessagingReader(base::PlatformFile handle);
  ~NativeMessagingReader();

  
  
  
  
  
  void Start(MessageCallback message_callback, base::Closure eof_callback);

 private:
  class Core;
  friend class Core;

  
  
  
  void InvokeMessageCallback(scoped_ptr<base::Value> message);
  void InvokeEofCallback();

  
  
  
  scoped_ptr<Core> core_;

  
  MessageCallback message_callback_;
  base::Closure eof_callback_;

  
  
  
  base::Thread reader_thread_;
  scoped_refptr<base::SequencedTaskRunner> read_task_runner_;

  
  
  base::WeakPtrFactory<NativeMessagingReader> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NativeMessagingReader);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_NATIVE_MESSAGING_NATIVE_MESSAGING_CHANNEL_H_
#define REMOTING_HOST_NATIVE_MESSAGING_NATIVE_MESSAGING_CHANNEL_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/platform_file.h"
#include "base/threading/non_thread_safe.h"
#include "remoting/host/native_messaging/native_messaging_reader.h"
#include "remoting/host/native_messaging/native_messaging_writer.h"

namespace base {
class DictionaryValue;
class Value;
}  

namespace remoting {

class NativeMessagingChannel : public base::NonThreadSafe {
 public:
  
  typedef base::Callback<void(scoped_ptr<base::DictionaryValue> message)>
      SendMessageCallback;

  
  
  NativeMessagingChannel(base::PlatformFile input, base::PlatformFile output);
  ~NativeMessagingChannel();

  
  void Start(const SendMessageCallback& received_message,
             const base::Closure& quit_closure);

  
  void SendMessage(scoped_ptr<base::DictionaryValue> message);

 private:
  
  void ProcessMessage(scoped_ptr<base::Value> message);

  
  
  void Shutdown();

  base::Closure quit_closure_;

  NativeMessagingReader native_messaging_reader_;
  scoped_ptr<NativeMessagingWriter> native_messaging_writer_;

  
  SendMessageCallback received_message_;

  base::WeakPtr<NativeMessagingChannel> weak_ptr_;
  base::WeakPtrFactory<NativeMessagingChannel> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NativeMessagingChannel);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_RAW_CHANNEL_H_
#define MOJO_SYSTEM_RAW_CHANNEL_H_

#include <vector>

#include "base/basictypes.h"
#include "mojo/system/constants.h"
#include "mojo/system/system_impl_export.h"

namespace base {
class MessageLoop;
}

namespace mojo {
namespace system {

class MessageInTransit;

struct PlatformChannelHandle;

//    view of the caller. If necessary, messages are queued (to be written on
class MOJO_SYSTEM_IMPL_EXPORT RawChannel {
 public:
  virtual ~RawChannel() {}

  
  
  class MOJO_SYSTEM_IMPL_EXPORT Delegate {
   public:
    enum FatalError {
      FATAL_ERROR_UNKNOWN = 0,
      FATAL_ERROR_FAILED_READ,
      FATAL_ERROR_FAILED_WRITE
    };

    
    
    virtual void OnReadMessage(const MessageInTransit& message) = 0;

    
    
    virtual void OnFatalError(FatalError fatal_error) = 0;

   protected:
    virtual ~Delegate() {}
  };

  
  
  
  static RawChannel* Create(const PlatformChannelHandle& handle,
                            Delegate* delegate,
                            base::MessageLoop* message_loop);

  
  
  virtual bool Init() = 0;

  
  virtual void Shutdown() = 0;

  
  
  virtual bool WriteMessage(MessageInTransit* message) = 0;

 protected:
  RawChannel(Delegate* delegate, base::MessageLoop* message_loop)
      : delegate_(delegate), message_loop_(message_loop) {}

  Delegate* delegate() { return delegate_; }
  base::MessageLoop* message_loop() { return message_loop_; }

 private:
  Delegate* const delegate_;
  base::MessageLoop* const message_loop_;

  DISALLOW_COPY_AND_ASSIGN(RawChannel);
};

}  
}  

#endif  

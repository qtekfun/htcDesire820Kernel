// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_CLIPBOARD_STUB_H_
#define REMOTING_PROTOCOL_CLIPBOARD_STUB_H_

#include "base/basictypes.h"

namespace remoting {
namespace protocol {

class ClipboardEvent;

class ClipboardStub {
 public:
  ClipboardStub() {}
  virtual ~ClipboardStub() {}

  
  
  virtual void InjectClipboardEvent(const ClipboardEvent& event) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ClipboardStub);
};

}  
}  

#endif  

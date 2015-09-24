// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_PROTOCOL_INPUT_STUB_H_
#define REMOTING_PROTOCOL_INPUT_STUB_H_

#include "base/basictypes.h"

namespace remoting {
namespace protocol {

class KeyEvent;
class MouseEvent;

class InputStub {
 public:
  InputStub() {}
  virtual ~InputStub() {}

  
  
  virtual void InjectKeyEvent(const KeyEvent& event) = 0;
  virtual void InjectMouseEvent(const MouseEvent& event) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(InputStub);
};

}  
}  

#endif  

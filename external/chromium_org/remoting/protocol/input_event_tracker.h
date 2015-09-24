// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_INPUT_EVENT_TRACKER_H_
#define REMOTING_PROTOCOL_INPUT_EVENT_TRACKER_H_

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "remoting/protocol/input_stub.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"

namespace remoting {
namespace protocol {

class InputEventTracker : public InputStub {
 public:
  explicit InputEventTracker(protocol::InputStub* input_stub);
  virtual ~InputEventTracker();

  
  bool IsKeyPressed(uint32 usb_keycode) const;

  
  int PressedKeyCount() const;

  
  
  void ReleaseAll();

  
  virtual void InjectKeyEvent(const KeyEvent& event) OVERRIDE;
  virtual void InjectMouseEvent(const MouseEvent& event) OVERRIDE;

 private:
  protocol::InputStub* input_stub_;

  std::set<uint32> pressed_keys_;

  webrtc::DesktopVector mouse_pos_;
  uint32 mouse_button_state_;

  DISALLOW_COPY_AND_ASSIGN(InputEventTracker);
};

}  
}  

#endif  

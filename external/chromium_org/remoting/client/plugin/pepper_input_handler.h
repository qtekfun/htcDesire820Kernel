// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_PLUGIN_PEPPER_INPUT_HANDLER_H_
#define REMOTING_CLIENT_PLUGIN_PEPPER_INPUT_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ppapi/cpp/mouse_lock.h"
#include "ppapi/cpp/point.h"
#include "ppapi/utility/completion_callback_factory.h"
#include "remoting/protocol/input_stub.h"

namespace pp {
class ImageData;
class InputEvent;
class Instance;
}  

namespace remoting {

namespace protocol {
class InputStub;
} 

class PepperInputHandler : public pp::MouseLock {
 public:
  
  PepperInputHandler(pp::Instance* instance, protocol::InputStub* input_stub);
  virtual ~PepperInputHandler();

  bool HandleInputEvent(const pp::InputEvent& event);

  
  
  void AllowMouseLock();

  
  void DidChangeFocus(bool has_focus);

  
  
  void SetMouseCursor(scoped_ptr<pp::ImageData> image,
                      const pp::Point& hotspot);

 private:
  enum MouseLockState {
    MouseLockDisallowed,
    MouseLockOff,
    MouseLockRequestPending,
    MouseLockOn,
    MouseLockCancelling
  };

  
  virtual void MouseLockLost() OVERRIDE;

  
  void RequestMouseLock();

  
  
  void CancelMouseLock();

  
  
  void UpdateMouseCursor();

  
  void OnMouseLocked(int error);

  pp::Instance* instance_;
  protocol::InputStub* input_stub_;

  pp::CompletionCallbackFactory<PepperInputHandler> callback_factory_;

  
  
  
  scoped_ptr<pp::ImageData> cursor_image_;

  
  pp::Point cursor_hotspot_;

  
  bool has_focus_;

  MouseLockState mouse_lock_state_;

  
  float wheel_delta_x_;
  float wheel_delta_y_;
  float wheel_ticks_x_;
  float wheel_ticks_y_;

  DISALLOW_COPY_AND_ASSIGN(PepperInputHandler);
};

}  

#endif  

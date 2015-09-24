// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_REMOTE_INPUT_FILTER_H_
#define REMOTING_HOST_REMOTE_INPUT_FILTER_H_

#include <list>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "remoting/protocol/input_event_tracker.h"
#include "remoting/protocol/input_stub.h"

namespace remoting {

class RemoteInputFilter : public protocol::InputStub {
 public:
  
  
  explicit RemoteInputFilter(protocol::InputEventTracker* event_tracker);
  virtual ~RemoteInputFilter();

  
  
  
  void LocalMouseMoved(const webrtc::DesktopVector& mouse_pos);

  
  void SetExpectLocalEcho(bool expect_local_echo);

  
  virtual void InjectKeyEvent(const protocol::KeyEvent& event) OVERRIDE;
  virtual void InjectMouseEvent(const protocol::MouseEvent& event) OVERRIDE;

 private:
  bool ShouldIgnoreInput() const;

  protocol::InputEventTracker* event_tracker_;

  
  
  std::list<webrtc::DesktopVector> injected_mouse_positions_;

  
  base::TimeTicks latest_local_input_time_;

  
  bool expect_local_echo_;

  DISALLOW_COPY_AND_ASSIGN(RemoteInputFilter);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MOUSE_LOCK_DISPATCHER_H_
#define CONTENT_RENDERER_MOUSE_LOCK_DISPATCHER_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace blink {
class WebMouseEvent;
}  

namespace content {

class CONTENT_EXPORT MouseLockDispatcher {
 public:
  MouseLockDispatcher();
  virtual ~MouseLockDispatcher();

  class LockTarget {
   public:
    virtual ~LockTarget() {}
    
    virtual void OnLockMouseACK(bool succeeded) = 0;
    
    virtual void OnMouseLockLost() = 0;
    
    virtual bool HandleMouseLockedInputEvent(
        const blink::WebMouseEvent& event) = 0;
  };

  
  
  bool LockMouse(LockTarget* target);
  
  
  void UnlockMouse(LockTarget* target);
  
  
  void OnLockTargetDestroyed(LockTarget* target);
  bool IsMouseLockedTo(LockTarget* target);

  
  bool WillHandleMouseEvent(const blink::WebMouseEvent& event);

  
  
  void OnLockMouseACK(bool succeeded);
  void OnMouseLockLost();

 protected:
  
  
  virtual void SendLockMouseRequest(bool unlocked_by_target) = 0;
  virtual void SendUnlockMouseRequest() = 0;

 private:
  bool MouseLockedOrPendingAction() const {
    return mouse_locked_ || pending_lock_request_ || pending_unlock_request_;
  }

  bool mouse_locked_;
  
  
  
  
  bool pending_lock_request_;
  bool pending_unlock_request_;

  
  
  
  bool unlocked_by_target_;

  
  
  
  LockTarget* target_;

  DISALLOW_COPY_AND_ASSIGN(MouseLockDispatcher);
};

}  

#endif  

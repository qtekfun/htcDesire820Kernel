// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_CAPS_LOCK_DELEGATE_H_
#define ASH_CAPS_LOCK_DELEGATE_H_

#include "ash/ash_export.h"

namespace ash {

class ASH_EXPORT CapsLockDelegate {
 public:
  virtual ~CapsLockDelegate() {}

  
  virtual bool IsCapsLockEnabled() const = 0;

  
  
  
  virtual void SetCapsLockEnabled(bool enabled) = 0;

  
  
  
  virtual void ToggleCapsLock() = 0;
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_IME_CONTROL_DELEGATE_H_
#define ASH_IME_CONTROL_DELEGATE_H_

namespace ui {
class Accelerator;
}  

namespace ash {

class ImeControlDelegate {
 public:
  virtual ~ImeControlDelegate() {}

  virtual bool HandleNextIme() = 0;
  virtual bool HandlePreviousIme(const ui::Accelerator& accelerator) = 0;
  
  virtual bool HandleSwitchIme(const ui::Accelerator& accelerator) = 0;

  
  
  virtual ui::Accelerator RemapAccelerator(
      const ui::Accelerator& accelerator) = 0;
};

}  

#endif  

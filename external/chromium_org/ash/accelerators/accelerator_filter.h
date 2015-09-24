// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ACCELERATORS_ACCELERATOR_FILTER_H_
#define ASH_ACCELERATORS_ACCELERATOR_FILTER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/events/event_handler.h"

namespace ash {
namespace internal {

class ASH_EXPORT AcceleratorFilter : public ui::EventHandler {
 public:
  AcceleratorFilter();
  virtual ~AcceleratorFilter();

  
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(AcceleratorFilter);
};

}  
}  

#endif  

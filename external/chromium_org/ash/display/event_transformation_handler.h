// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_EVENT_TRANSFORMATION_HANDLER_H_
#define ASH_DISPLAY_EVENT_TRANSFORMATION_HANDLER_H_

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "ui/events/event_handler.h"

namespace ash {

namespace internal {

class ASH_EXPORT EventTransformationHandler : public ui::EventHandler {
 public:
  enum TransformationMode {
    TRANSFORM_AUTO,   
                      
                      
    TRANSFORM_NONE,   
  };

  EventTransformationHandler();
  virtual ~EventTransformationHandler();

  void set_transformation_mode(TransformationMode transformation_mode) {
    transformation_mode_ = transformation_mode;
  }

  
  virtual void OnScrollEvent(ui::ScrollEvent* event) OVERRIDE;
#if defined(OS_CHROMEOS)
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
#endif  

 private:
  TransformationMode transformation_mode_;

  DISALLOW_COPY_AND_ASSIGN(EventTransformationHandler);
};

}  
}  

#endif  

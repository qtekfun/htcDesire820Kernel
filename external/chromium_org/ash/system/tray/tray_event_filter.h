// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_TRAY_EVENT_FILTER_H_
#define ASH_SYSTEM_TRAY_TRAY_EVENT_FILTER_H_

#include <set>

#include "base/basictypes.h"
#include "ui/events/event.h"
#include "ui/events/event_handler.h"

namespace aura {
class Window;
}

namespace ash {
namespace internal {

class TrayBubbleWrapper;


class TrayEventFilter : public ui::EventHandler {
 public:
  explicit TrayEventFilter();
  virtual ~TrayEventFilter();

  void AddWrapper(TrayBubbleWrapper* wrapper);
  void RemoveWrapper(TrayBubbleWrapper* wrapper);

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;

 private:
  
  bool ProcessLocatedEvent(ui::LocatedEvent* event);

  std::set<TrayBubbleWrapper*> wrappers_;

  DISALLOW_COPY_AND_ASSIGN(TrayEventFilter);
};

}  
}  

#endif  

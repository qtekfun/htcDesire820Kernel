// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_EVENT_CLIENT_H_
#define UI_AURA_CLIENT_EVENT_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace ui {
class EventTarget;
}

namespace aura {
class Window;
namespace client {

class AURA_EXPORT EventClient {
 public:
  
  virtual bool CanProcessEventsWithinSubtree(const Window* window) const = 0;

  
  virtual ui::EventTarget* GetToplevelEventTarget() = 0;

 protected:
  virtual ~EventClient() {}
};

AURA_EXPORT void SetEventClient(Window* root_window, EventClient* client);
AURA_EXPORT EventClient* GetEventClient(const Window* root_window);

}  
}  

#endif  

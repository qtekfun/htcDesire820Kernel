// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_EVENT_CONVERSION_H_
#define CONTENT_RENDERER_PEPPER_EVENT_CONVERSION_H_

#include <vector>

#include "base/memory/linked_ptr.h"
#include "ppapi/c/ppb_input_event.h"
#include "third_party/WebKit/public/web/WebInputEvent.h"

struct PP_InputEvent;

namespace ppapi {
struct InputEventData;
}

namespace blink {
class WebGamepads;
class WebInputEvent;
}

namespace content {

void CreateInputEventData(const blink::WebInputEvent& event,
                          std::vector<ppapi::InputEventData >* pp_events);

blink::WebInputEvent* CreateWebInputEvent(const ppapi::InputEventData& event);

std::vector<linked_ptr<blink::WebInputEvent> > CreateSimulatedWebInputEvents(
    const ppapi::InputEventData& event,
    int plugin_x,
    int plugin_y);

PP_InputEvent_Class ClassifyInputEvent(blink::WebInputEvent::Type type);

}  

#endif  

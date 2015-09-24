// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_PLUGIN_WEB_EVENT_CONVERTER_MAC_H_
#define CONTENT_CHILD_NPAPI_PLUGIN_WEB_EVENT_CONVERTER_MAC_H_

#include "third_party/npapi/bindings/npapi.h"

namespace blink {
class WebInputEvent;
class WebKeyboardEvent;
class WebMouseEvent;
class WebMouseWheelEvent;
}

namespace content {

class PluginWebEventConverter {
 public:
  PluginWebEventConverter();
  virtual ~PluginWebEventConverter();

  
  
  bool InitWithEvent(const blink::WebInputEvent& web_event);

  
  
  
  
  NPCocoaEvent* plugin_event() { return &cocoa_event_; }

 private:
  
  
  
  bool ConvertKeyboardEvent(const blink::WebKeyboardEvent& web_event);
  bool ConvertMouseEvent(const blink::WebMouseEvent& web_event);
  bool ConvertMouseWheelEvent(const blink::WebMouseWheelEvent& web_event);

  
  static NSUInteger CocoaModifiers(const blink::WebInputEvent& web_event);

  NPCocoaEvent cocoa_event_;

  DISALLOW_COPY_AND_ASSIGN(PluginWebEventConverter);
};

}  

#endif  

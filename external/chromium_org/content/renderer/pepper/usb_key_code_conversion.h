// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_USB_KEY_CODE_CONVERSION_H_
#define CONTENT_RENDERER_PEPPER_USB_KEY_CODE_CONVERSION_H_

#include "ppapi/c/pp_stdint.h"

namespace blink {
class WebKeyboardEvent;
}  

namespace content {

uint32_t UsbKeyCodeForKeyboardEvent(const blink::WebKeyboardEvent& key_event);

const char* CodeForKeyboardEvent(const blink::WebKeyboardEvent& key_event);

}  

#endif  

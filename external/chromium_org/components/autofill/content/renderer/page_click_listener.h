// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CONTENT_RENDERER_PAGE_CLICK_LISTENER_H_
#define COMPONENTS_AUTOFILL_CONTENT_RENDERER_PAGE_CLICK_LISTENER_H_

namespace blink {
class WebInputElement;
}

namespace autofill {

class PageClickListener {
 public:
  
  
  
  virtual void InputElementClicked(const blink::WebInputElement& element,
                                   bool was_focused,
                                   bool is_focused) = 0;

  
  
  virtual void InputElementLostFocus() = 0;

 protected:
  virtual ~PageClickListener() {}
};

}  

#endif  

// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SEARCHBOX_SEARCHBOX_EXTENSION_H_
#define CHROME_RENDERER_SEARCHBOX_SEARCHBOX_EXTENSION_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace v8 {
class Extension;
}

namespace blink {
class WebFrame;
}

namespace extensions_v8 {

class SearchBoxExtension {
 public:
  
  
  static v8::Extension* Get();

  
  
  static bool PageSupportsInstant(blink::WebFrame* frame);

  
  static void DispatchChromeIdentityCheckResult(blink::WebFrame* frame,
                                                const base::string16& identity,
                                                bool identity_match);
  static void DispatchFocusChange(blink::WebFrame* frame);
  static void DispatchInputCancel(blink::WebFrame* frame);
  static void DispatchInputStart(blink::WebFrame* frame);
  static void DispatchKeyCaptureChange(blink::WebFrame* frame);
  static void DispatchMarginChange(blink::WebFrame* frame);
  static void DispatchMostVisitedChanged(blink::WebFrame* frame);
  static void DispatchSubmit(blink::WebFrame* frame);
  static void DispatchSuggestionChange(blink::WebFrame* frame);
  static void DispatchThemeChange(blink::WebFrame* frame);
  static void DispatchToggleVoiceSearch(blink::WebFrame* frame);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(SearchBoxExtension);
};

}  

#endif  

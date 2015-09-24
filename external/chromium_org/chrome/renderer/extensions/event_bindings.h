// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_EVENT_BINDINGS_H_
#define CHROME_RENDERER_EXTENSIONS_EVENT_BINDINGS_H_

#include "v8/include/v8.h"

namespace extensions {
class ChromeV8Context;
class ChromeV8Extension;
class Dispatcher;
class EventFilter;

class EventBindings {
 public:
  static ChromeV8Extension* Create(Dispatcher* dispatcher,
                                   ChromeV8Context* context);
};

}  

#endif  

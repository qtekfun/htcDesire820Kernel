// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EVENT_UTILS_H_
#define CHROME_BROWSER_UI_COCOA_EVENT_UTILS_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "webkit/glue/window_open_disposition.h"

namespace event_utils {

WindowOpenDisposition WindowOpenDispositionFromNSEvent(NSEvent* event);

WindowOpenDisposition WindowOpenDispositionFromNSEventWithFlags(
    NSEvent* event, NSUInteger flags);

}  

#endif  

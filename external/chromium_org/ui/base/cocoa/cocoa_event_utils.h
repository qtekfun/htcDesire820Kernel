// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_COCOA_EVENT_UTILS_H_
#define UI_BASE_COCOA_COCOA_EVENT_UTILS_H_

#import <Cocoa/Cocoa.h>

#include "ui/base/ui_export.h"
#include "ui/base/window_open_disposition.h"

namespace ui {

UI_EXPORT int EventFlagsFromNSEvent(NSEvent* event);

UI_EXPORT int EventFlagsFromNSEventWithModifiers(NSEvent* event,
                                                 NSUInteger modifiers);

UI_EXPORT WindowOpenDisposition WindowOpenDispositionFromNSEvent(
    NSEvent* event);

UI_EXPORT WindowOpenDisposition WindowOpenDispositionFromNSEventWithFlags(
    NSEvent* event, NSUInteger flags);

}  

#endif  

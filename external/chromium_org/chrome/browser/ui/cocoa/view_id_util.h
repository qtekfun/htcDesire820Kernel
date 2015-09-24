// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_VIEW_ID_UTIL_H_
#define CHROME_BROWSER_UI_COCOA_VIEW_ID_UTIL_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/view_ids.h"
#include "ui/gfx/native_widget_types.h"



namespace view_id_util {

void SetID(NSView* view, ViewID viewID);

void UnsetID(NSView* view);

NSView* GetView(NSWindow* window, ViewID viewID);

}  


@interface NSView (ViewID)

- (ViewID)viewID;

@end

#endif  

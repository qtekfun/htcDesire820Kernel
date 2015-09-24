// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_UTILITIES_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_INFOBAR_UTILITIES_H_

#import <Cocoa/Cocoa.h>

namespace InfoBarUtilities {

void MoveControl(NSView* anchor, NSView* toMove, int spacing, bool after);

void VerticallyCenterView(NSView *toMove);

bool VerifyControlOrderAndSpacing(id before, id after);

NSTextField* CreateLabel(NSRect bounds);

void AddMenuItem(NSMenu *menu, id target, SEL selector, NSString* title,
    int tag, bool enabled, bool checked);

}  

#endif  

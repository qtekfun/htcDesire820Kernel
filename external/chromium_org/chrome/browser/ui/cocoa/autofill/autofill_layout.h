// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_LAYOUT_H_
#define CHROME_BROWSER_UI_COCOA_AUTOFILL_AUTOFILL_LAYOUT_H_

#import <Cocoa/Cocoa.h>

@protocol AutofillLayout

- (NSSize)preferredSize;

- (void)performLayout;

@end

#endif  

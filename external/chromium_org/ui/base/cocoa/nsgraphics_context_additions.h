// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_NSGRAPHICS_CONTEXT_ADDITIONS_H_
#define UI_BASE_COCOA_NSGRAPHICS_CONTEXT_ADDITIONS_H_

#import <Cocoa/Cocoa.h>

@interface NSGraphicsContext (CrAdditions)

- (void)cr_setPatternPhase:(NSPoint)phase
                   forView:(NSView*)view;

@end

#endif  

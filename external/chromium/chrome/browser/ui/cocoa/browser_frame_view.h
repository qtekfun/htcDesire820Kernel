// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

@interface BrowserFrameView : NSView

+ (BOOL)drawWindowThemeInDirtyRect:(NSRect)dirtyRect
                           forView:(NSView*)view
                            bounds:(NSRect)bounds
                            offset:(NSPoint)offset
              forceBlackBackground:(BOOL)forceBlackBackground;

+ (NSColor*)titleColorForThemeView:(NSView*)view;

@end

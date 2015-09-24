// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_IMAGE_UTILS_H_
#define CHROME_BROWSER_UI_COCOA_IMAGE_UTILS_H_
#pragma once

#import <Cocoa/Cocoa.h>

@interface NSImage (FlippedAdditions)

- (void)drawInRect:(NSRect)dstRect
          fromRect:(NSRect)srcRect
         operation:(NSCompositingOperation)op
          fraction:(CGFloat)requestedAlpha
      neverFlipped:(BOOL)neverFlipped;
@end

#endif  

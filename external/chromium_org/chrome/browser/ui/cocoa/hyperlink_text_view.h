// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

@class NSColor;

@interface HyperlinkTextView : NSTextView {
 @private
  BOOL acceptsFirstResponder_;
}

- (void)setMessageAndLink:(NSString*)message
                 withLink:(NSString*)link
                 atOffset:(NSUInteger)linkOffset
                     font:(NSFont*)font
             messageColor:(NSColor*)messageColor
                linkColor:(NSColor*)linkColor;

- (void)setMessage:(NSString*)message
          withFont:(NSFont*)font
      messageColor:(NSColor*)messageColor;

- (void)addLinkRange:(NSRange)range
            withName:(id)name
           linkColor:(NSColor*)linkColor;

- (void)setAcceptsFirstResponder:(BOOL)acceptsFirstResponder;

@end

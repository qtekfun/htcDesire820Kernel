// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BACKGROUND_TILE_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_BACKGROUND_TILE_VIEW_H_
#pragma once

#import <Cocoa/Cocoa.h>


@interface BackgroundTileView : NSView {
 @private
  BOOL showsDivider_;
  NSImage* tileImage_;
}

@property(nonatomic, retain) NSImage* tileImage;
@end

#endif  

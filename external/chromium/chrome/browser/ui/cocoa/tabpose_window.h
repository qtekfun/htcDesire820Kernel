// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABPOSE_WINDOW_H_
#define CHROME_BROWSER_UI_COCOA_TABPOSE_WINDOW_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_cftyperef.h"

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"

namespace tabpose {

class Tile;
class TileSet;

}

namespace tabpose {

enum WindowState {
  kFadingIn,
  kFadedIn,
  kFadingOut,
};

}  

class TabStripModel;
class TabStripModelObserverBridge;

@interface TabposeWindow : NSWindow {
 @private
  tabpose::WindowState state_;

  
  CALayer* rootLayer_;  

  
  CALayer* bgLayer_;  

  
  CALayer* topGradient_;  

  
  CALayer* selectionHighlight_;  

  
  base::mac::ScopedCFTypeRef<CGColorRef> gray_;
  base::mac::ScopedCFTypeRef<CGColorRef> darkBlue_;

  TabStripModel* tabStripModel_;  

  
  
  scoped_nsobject<NSMutableArray> allThumbnailLayers_;

  scoped_nsobject<NSMutableArray> allFaviconLayers_;
  scoped_nsobject<NSMutableArray> allTitleLayers_;

  
  scoped_ptr<tabpose::TileSet> tileSet_;

  
  
  NSRect containingRect_;

  
  scoped_ptr<TabStripModelObserverBridge> tabStripModelObserverBridge_;

  
  base::mac::ScopedCFTypeRef<CGImageRef> closeIcon_;

  
  
  BOOL showAllCloseLayers_;
}

+ (id)openTabposeFor:(NSWindow*)parent
                rect:(NSRect)rect
               slomo:(BOOL)slomo
       tabStripModel:(TabStripModel*)tabStripModel;
@end

@interface TabposeWindow (TestingAPI)
- (void)selectTileAtIndexWithoutAnimation:(int)newIndex;
- (NSUInteger)thumbnailLayerCount;
- (int)selectedIndex;
@end

#endif  

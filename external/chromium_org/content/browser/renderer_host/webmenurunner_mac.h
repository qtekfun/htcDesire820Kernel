// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_WEBMENURUNNER_MAC_H_
#define CONTENT_BROWSER_RENDERER_HOST_WEBMENURUNNER_MAC_H_

#import <Cocoa/Cocoa.h>

#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "content/public/common/menu_item.h"



@interface WebMenuRunner : NSObject {
 @private
  
  base::scoped_nsobject<NSMenu> menu_;

  
  
  BOOL menuItemWasChosen_;

  
  int index_;

  
  CGFloat fontSize_;

  
  BOOL rightAligned_;
}

- (id)initWithItems:(const std::vector<content::MenuItem>&)items
           fontSize:(CGFloat)fontSize
       rightAligned:(BOOL)rightAligned;

- (BOOL)menuItemWasChosen;

- (void)runMenuInView:(NSView*)view
           withBounds:(NSRect)bounds
         initialIndex:(int)index;

- (int)indexOfSelectedItem;

@end  

#endif 

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_WEBMENURUNNER_MAC_H_
#define WEBKIT_GLUE_WEBMENURUNNER_MAC_H_

#import <Cocoa/Cocoa.h>

#include <vector>

#include "base/memory/scoped_nsobject.h"
#include "webkit/glue/webmenuitem.h"



@interface WebMenuRunner : NSObject {
 @private
  
  scoped_nsobject<NSMenu> menu_;

  
  
  BOOL menuItemWasChosen_;

  
  int index_;

  
  CGFloat fontSize_;

  
  BOOL rightAligned_;
}

- (id)initWithItems:(const std::vector<WebMenuItem>&)items
           fontSize:(CGFloat)fontSize
       rightAligned:(BOOL)rightAligned;

- (BOOL)menuItemWasChosen;

- (void)runMenuInView:(NSView*)view
           withBounds:(NSRect)bounds
         initialIndex:(int)index;

- (int)indexOfSelectedItem;

@end  

namespace webkit_glue {
NSEvent* EventWithMenuAction(BOOL item_chosen, int window_num,
                             int item_height, int selected_index,
                             NSRect menu_bounds, NSRect view_bounds);
}  

#endif 

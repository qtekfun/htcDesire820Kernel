// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/page_info_model.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"

@interface PageInfoBubbleController : BaseBubbleController {
 @private
  
  scoped_ptr<PageInfoModel> model_;

  
  scoped_ptr<PageInfoModel::PageInfoModelObserver> bridge_;

  
  int certID_;
}

@property(nonatomic, assign) int certID;

- (id)initWithPageInfoModel:(PageInfoModel*)model
              modelObserver:(PageInfoModel::PageInfoModelObserver*)bridge
               parentWindow:(NSWindow*)parentWindow;

- (IBAction)showCertWindow:(id)sender;

- (IBAction)showHelpPage:(id)sender;

@end

@interface PageInfoBubbleController (ExposedForUnitTesting)
- (void)performLayout;
@end

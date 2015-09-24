// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "ui/base/cocoa/menu_controller.h"
#include "base/mac/scoped_nsobject.h"

@class DownloadItemController;

@interface DownloadShelfContextMenuController : MenuController {
 @private
  base::scoped_nsobject<DownloadItemController> itemController_;
  id<NSMenuDelegate> menuDelegate_;
}

- (id)initWithItemController:(DownloadItemController*)itemController
                withDelegate:(id<NSMenuDelegate>)menuDelegate;

- (void)menuDidClose:(NSMenu*)menu;
@end

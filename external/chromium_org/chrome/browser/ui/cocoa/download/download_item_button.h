// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/files/file_path.h"
#import "chrome/browser/ui/cocoa/draggable_button.h"

@class DownloadItemController;

@interface DownloadItemButton : DraggableButton<NSMenuDelegate> {
 @private
  base::FilePath downloadPath_;
  DownloadItemController* controller_;  
}

@property(assign, nonatomic) base::FilePath download;
@property(assign, nonatomic) DownloadItemController* controller;

- (void)beginDrag:(NSEvent*)event;

@end

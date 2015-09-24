// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/file_path.h"
#import "chrome/browser/ui/cocoa/draggable_button.h"

@class DownloadItemController;

@interface DownloadItemButton : DraggableButton<NSMenuDelegate> {
 @private
  FilePath downloadPath_;
  DownloadItemController* controller_;  
}

@property(assign, nonatomic) FilePath download;
@property(assign, nonatomic) DownloadItemController* controller;

- (void)beginDrag:(NSEvent*)event;

@end

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/time/time.h"

@interface DockIcon : NSObject {
 @private
  
  base::TimeTicks lastUpdate_;

  
  
  BOOL forceUpdate_;
}

+ (DockIcon*)sharedDockIcon;

- (void)updateIcon;


- (void)setDownloads:(int)downloads;

- (void)setIndeterminate:(BOOL)indeterminate;

- (void)setProgress:(float)progress;

@end

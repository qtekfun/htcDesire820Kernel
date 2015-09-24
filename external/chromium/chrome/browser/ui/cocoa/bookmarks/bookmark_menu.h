// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"


@interface BookmarkMenu : NSMenu {
 @private
  int64 id_;  
}
- (void)setRepresentedObject:(id)object;
@property(nonatomic) int64 id;
@end


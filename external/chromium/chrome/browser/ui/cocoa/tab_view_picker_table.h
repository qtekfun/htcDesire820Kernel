// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"

@interface TabViewPickerTable : NSOutlineView <NSTabViewDelegate,
                                              NSOutlineViewDelegate,
                                              NSOutlineViewDataSource> {
 @public
  IBOutlet NSTabView* tabView_;  

 @private
  id oldTabViewDelegate_;

  
  scoped_nsobject<NSString> heading_;
}
@property(nonatomic, copy) NSString* heading;
@end

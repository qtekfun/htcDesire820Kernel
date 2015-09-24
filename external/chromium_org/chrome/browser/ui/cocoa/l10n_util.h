// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/strings/string16.h"

namespace cocoa_l10n_util {

NSInteger CompareFrameY(id view1, id view2, void* context);

NSSize WrapOrSizeToFit(NSView* view);

CGFloat VerticallyReflowGroup(NSArray* views);

// Like |ReplaceStringPlaceholders(const base::string16&,
NSString* ReplaceNSStringPlaceholders(NSString* formatString,
                                      const base::string16& a,
                                      size_t* offset);

NSString* TooltipForURLAndTitle(NSString* url, NSString* title);

}  

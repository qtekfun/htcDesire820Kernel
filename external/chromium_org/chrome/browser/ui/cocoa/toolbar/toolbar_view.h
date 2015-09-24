// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TOOLBAR_TOOLBAR_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_TOOLBAR_TOOLBAR_VIEW_H_

#import <Cocoa/Cocoa.h>
#import "chrome/browser/ui/cocoa/background_gradient_view.h"


@interface ToolbarView : BackgroundGradientView {
 @private
  
  
  CGFloat dividerOpacity_;
}

@property(assign, nonatomic) CGFloat dividerOpacity;
@end

#endif  

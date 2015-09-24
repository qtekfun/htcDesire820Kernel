// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_OMNIBOX_OMNIBOX_POPUP_SEPARATOR_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_OMNIBOX_OMNIBOX_POPUP_SEPARATOR_VIEW_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/ui/cocoa/background_gradient_view.h"

@interface OmniboxPopupTopSeparatorView : BackgroundGradientView {
}

+ (CGFloat)preferredHeight;

@end

@interface OmniboxPopupBottomSeparatorView : NSView {
}

+ (CGFloat)preferredHeight;

@end

#endif  

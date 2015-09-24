// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_MEDIA_INDICATOR_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_TABS_MEDIA_INDICATOR_VIEW_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/tabs/tab_utils.h"

class MediaIndicatorViewAnimationDelegate;

namespace gfx {
class Animation;
}  

@interface MediaIndicatorView : NSImageView {
 @private
  TabMediaState mediaState_;
  scoped_ptr<MediaIndicatorViewAnimationDelegate> delegate_;
  scoped_ptr<gfx::Animation> animation_;
  TabMediaState animatingMediaState_;
}

@property(readonly, nonatomic) TabMediaState mediaState;
@property(readonly, nonatomic) TabMediaState animatingMediaState;

- (id)init;

- (void)updateIndicator:(TabMediaState)mediaState;

- (void)setAnimationDoneCallbackObject:(id)anObject withSelector:(SEL)selector;

@end

@interface MediaIndicatorView(TestingAPI)
- (void)disableAnimations;
@end

#endif  

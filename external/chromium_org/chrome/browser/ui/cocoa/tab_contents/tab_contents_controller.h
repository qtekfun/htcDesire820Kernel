// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_TAB_CONTENTS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TAB_CONTENTS_TAB_CONTENTS_CONTROLLER_H_

#include <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"

class FullscreenObserver;

namespace content {
class WebContents;
}


@interface TabContentsController : NSViewController {
 @private
   content::WebContents* contents_;  
   
   
   scoped_ptr<FullscreenObserver> fullscreenObserver_;
   
   
   BOOL isEmbeddingFullscreenWidget_;
}
@property(readonly, nonatomic) content::WebContents* webContents;

- (id)initWithContents:(content::WebContents*)contents
    andAutoEmbedFullscreen:(BOOL)enableEmbeddedFullscreen;

- (void)ensureContentsSizeDoesNotChange;

- (void)ensureContentsVisible;

- (void)changeWebContents:(content::WebContents*)newContents;

- (void)willBecomeUnselectedTab;

- (void)willBecomeSelectedTab;

- (void)tabDidChange:(content::WebContents*)updatedContents;

- (void)toggleFullscreenWidget:(BOOL)enterFullscreen;

@end

#endif  

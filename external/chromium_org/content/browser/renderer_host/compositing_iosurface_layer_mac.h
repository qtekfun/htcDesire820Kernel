// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_COMPOSITING_IOSURFACE_LAYER_MAC_H_
#define CONTENT_BROWSER_RENDERER_HOST_COMPOSITING_IOSURFACE_LAYER_MAC_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_cftyperef.h"
#include "base/memory/ref_counted.h"

namespace content {
class CompositingIOSurfaceContext;
class RenderWidgetHostViewMac;
}

@interface CompositingIOSurfaceLayer : CAOpenGLLayer {
 @private
  content::RenderWidgetHostViewMac* renderWidgetHostView_;
  scoped_refptr<content::CompositingIOSurfaceContext> context_;
}

@property(nonatomic, readonly)
    scoped_refptr<content::CompositingIOSurfaceContext> context;

- (id)initWithRenderWidgetHostViewMac:(content::RenderWidgetHostViewMac*)r;

- (void)updateScaleFactor;

- (void)disableCompositing;

@end

#endif  

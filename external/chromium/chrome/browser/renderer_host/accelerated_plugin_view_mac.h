// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_ACCELERATED_PLUGIN_VIEW_MAC_H
#define CHROME_BROWSER_RENDERER_HOST_ACCELERATED_PLUGIN_VIEW_MAC_H

#import <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

#include "base/memory/scoped_nsobject.h"
#include "ui/gfx/native_widget_types.h"

class RenderWidgetHostViewMac;

@interface NSObject (UnderlayableSurface)
- (void)underlaySurfaceAdded;
- (void)underlaySurfaceRemoved;
@end


@interface AcceleratedPluginView : NSView {
  scoped_nsobject<NSOpenGLPixelFormat> glPixelFormat_;
  CGLPixelFormatObj cglPixelFormat_;  
  scoped_nsobject<NSOpenGLContext> glContext_;
  CGLContextObj cglContext_;  

  CVDisplayLinkRef displayLink_;  

  RenderWidgetHostViewMac* renderWidgetHostView_;  
  gfx::PluginWindowHandle pluginHandle_;  

  
  
  
  
  uint64 swapBuffersCount_;
  
  
  
  volatile uint64 acknowledgedSwapBuffersCount_;

  
  
  
  volatile int rendererId_;
  volatile int32 routeId_;
  volatile int gpuHostId_;

  
  
  NSSize cachedSize_;

  
  scoped_nsobject<NSArray> cutoutRects_;

  
  
  int globalFrameDidChangeCGLLockCount_;
}

- (id)initWithRenderWidgetHostViewMac:(RenderWidgetHostViewMac*)r
                         pluginHandle:(gfx::PluginWindowHandle)pluginHandle;
- (void)drawView;

- (void)setCutoutRects:(NSArray*)cutout_rects;

- (void)updateSwapBuffersCount:(uint64)count
                  fromRenderer:(int)rendererId
                       routeId:(int32)routeId
                     gpuHostId:(int)gpuHostId;

- (void)onRenderWidgetHostViewGone;

@property NSSize cachedSize;
@end

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TRACKING_AREA_H_
#define CHROME_BROWSER_UI_COCOA_TRACKING_AREA_H_

#import <AppKit/AppKit.h>

#include "base/memory/scoped_nsobject.h"

@class CrTrackingAreaOwnerProxy;

@interface CrTrackingArea : NSTrackingArea {
 @private
  scoped_nsobject<CrTrackingAreaOwnerProxy> ownerProxy_;
}

- (id)initWithRect:(NSRect)rect
           options:(NSTrackingAreaOptions)options
      proxiedOwner:(id)owner  
          userInfo:(NSDictionary*)userInfo;

- (void)clearOwner;

- (void)clearOwnerWhenWindowWillClose:(NSWindow*)window;

@end


class ScopedCrTrackingArea {
 public:
  
  explicit ScopedCrTrackingArea(CrTrackingArea* tracking_area = nil);
  ~ScopedCrTrackingArea();

  
  
  
  void reset(CrTrackingArea* tracking_area = nil);

  CrTrackingArea* get() const;

 private:
  scoped_nsobject<CrTrackingArea> tracking_area_;
  DISALLOW_COPY_AND_ASSIGN(ScopedCrTrackingArea);
};

#endif  

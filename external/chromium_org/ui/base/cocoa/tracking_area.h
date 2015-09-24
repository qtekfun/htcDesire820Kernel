// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_TRACKING_AREA_H_
#define UI_BASE_COCOA_TRACKING_AREA_H_

#import <AppKit/AppKit.h>

#include "base/mac/scoped_nsobject.h"
#include "ui/base/ui_export.h"

@class CrTrackingAreaOwnerProxy;

UI_EXPORT
@interface CrTrackingArea : NSTrackingArea {
 @private
  base::scoped_nsobject<CrTrackingAreaOwnerProxy> ownerProxy_;
}

- (id)initWithRect:(NSRect)rect
           options:(NSTrackingAreaOptions)options
             owner:(id)owner
          userInfo:(NSDictionary*)userInfo;

- (void)clearOwner;

- (void)clearOwnerWhenWindowWillClose:(NSWindow*)window;

@end


namespace ui {

class UI_EXPORT ScopedCrTrackingArea {
 public:
  
  explicit ScopedCrTrackingArea(CrTrackingArea* tracking_area = nil);
  ~ScopedCrTrackingArea();

  
  
  
  void reset(CrTrackingArea* tracking_area = nil);

  CrTrackingArea* get() const;

 private:
  base::scoped_nsobject<CrTrackingArea> tracking_area_;
  DISALLOW_COPY_AND_ASSIGN(ScopedCrTrackingArea);
};

}  

#endif  

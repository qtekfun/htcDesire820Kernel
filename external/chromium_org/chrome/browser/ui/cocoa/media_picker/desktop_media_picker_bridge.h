// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_MEDIA_PICKER_DESKTOP_MEDIA_PICKER_BRIDGE_H_
#define CHROME_BROWSER_UI_COCOA_MEDIA_PICKER_DESKTOP_MEDIA_PICKER_BRIDGE_H_

#include "base/basictypes.h"
#include "chrome/browser/media/desktop_media_list_observer.h"

@protocol DesktopMediaPickerObserver
- (void)sourceAddedAtIndex:(int)index;
- (void)sourceRemovedAtIndex:(int)index;
- (void)sourceNameChangedAtIndex:(int)index;
- (void)sourceThumbnailChangedAtIndex:(int)index;
@end

class DesktopMediaPickerBridge : public DesktopMediaListObserver {
 public:
  DesktopMediaPickerBridge(id<DesktopMediaPickerObserver> observer);
  virtual ~DesktopMediaPickerBridge();

  
  virtual void OnSourceAdded(int index) OVERRIDE;
  virtual void OnSourceRemoved(int index) OVERRIDE;
  virtual void OnSourceNameChanged(int index) OVERRIDE;
  virtual void OnSourceThumbnailChanged(int index) OVERRIDE;

 private:
  id<DesktopMediaPickerObserver> observer_;  

  DISALLOW_COPY_AND_ASSIGN(DesktopMediaPickerBridge);
};

#endif  

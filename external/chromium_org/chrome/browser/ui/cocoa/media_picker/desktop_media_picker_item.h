// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_MEDIA_PICKER_DESKTOP_MEDIA_PICKER_ITEM_H_
#define CHROME_BROWSER_UI_COCOA_MEDIA_PICKER_DESKTOP_MEDIA_PICKER_ITEM_H_

#import <AppKit/AppKit.h>

#import "base/mac/scoped_nsobject.h"
#include "chrome/browser/media/desktop_media_list.h"

@interface DesktopMediaPickerItem : NSObject {
 @private
  content::DesktopMediaID sourceID_;
  base::scoped_nsobject<NSString> imageUID_;
  base::scoped_nsobject<NSString> imageTitle_;
  base::scoped_nsobject<NSImage> image_;
  NSUInteger imageVersion_;
}

- (id)initWithSourceId:(content::DesktopMediaID)sourceID
              imageUID:(int)imageUID
            imageTitle:(NSString*)imageTitle;

- (content::DesktopMediaID)sourceID;

- (void)setImageRepresentation:(NSImage*)image;

- (void)setImageTitle:(NSString*)imageTitle;

@end

#endif  

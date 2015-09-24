// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_MEDIA_PICKER_DESKTOP_MEDIA_PICKER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_MEDIA_PICKER_DESKTOP_MEDIA_PICKER_CONTROLLER_H_

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>

#include "base/callback.h"
#import "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/media/desktop_media_list.h"
#include "chrome/browser/media/desktop_media_picker.h"
#import "chrome/browser/ui/cocoa/media_picker/desktop_media_picker_bridge.h"

@interface DesktopMediaPickerController
    : NSWindowController<NSWindowDelegate, DesktopMediaPickerObserver> {
 @private
  
  
  base::scoped_nsobject<IKImageBrowserView> sourceBrowser_;

  
  NSButton* okButton_;  

  
  NSButton* cancelButton_;  

  
  
  scoped_ptr<DesktopMediaList> media_list_;

  
  DesktopMediaPicker::DoneCallback doneCallback_;

  
  base::scoped_nsobject<NSMutableArray> items_;

  
  
  scoped_ptr<DesktopMediaPickerBridge> bridge_;

  
  int lastImageUID_;
}

- (id)initWithMediaList:(scoped_ptr<DesktopMediaList>)media_list
               callback:(const DesktopMediaPicker::DoneCallback&)callback
                appName:(const base::string16&)appName;

@end

#endif  

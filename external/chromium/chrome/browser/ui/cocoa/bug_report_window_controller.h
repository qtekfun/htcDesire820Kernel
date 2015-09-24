// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BUG_REPORT_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BUG_REPORT_WINDOW_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include <vector>

#include "base/memory/scoped_nsobject.h"

class Profile;
class TabContents;

@interface BugReportWindowController : NSWindowController {
 @private
  TabContents* currentTab_;  
  Profile* profile_;  

  
  std::vector<unsigned char> pngData_;
  
  int pngWidth_;
  int pngHeight_;

  
  
  NSString* bugDescription_;  
  NSUInteger bugTypeIndex_;
  NSString* pageTitle_;  
  NSString* pageURL_;  

  
  IBOutlet NSButton* sendReportButton_;

  
  IBOutlet NSButton* cancelButton_;

  
  IBOutlet NSPopUpButton* bugTypePopUpButton_;

  
  BOOL sendScreenshot_;

  
  BOOL disableScreenshotCheckbox_;

  
  
  
  
  NSMutableArray* bugTypeList_;  

  
  
  
  
  BOOL saveSendScreenshot_;
  scoped_nsobject<NSString> saveBugDescription_;  

  
  NSDictionary* bugTypeDictionary_;  
}

@property(nonatomic, copy) NSString* bugDescription;
@property(nonatomic) NSUInteger bugTypeIndex;
@property(nonatomic, copy) NSString* pageTitle;
@property(nonatomic, copy) NSString* pageURL;
@property(nonatomic) BOOL sendScreenshot;
@property(nonatomic) BOOL disableScreenshotCheckbox;
@property(nonatomic, readonly) NSArray* bugTypeList;

- (id)initWithTabContents:(TabContents*)currentTab profile:(Profile*)profile;

- (void)runModalDialog;

- (IBAction)sendReport:(id)sender;
- (IBAction)cancel:(id)sender;

- (BOOL)isPhishingReport;

- (int)bugTypeFromIndex;

- (BOOL)control:(NSControl*)control textView:(NSTextView*)textView
    doCommandBySelector:(SEL)commandSelector;

@end

#endif  

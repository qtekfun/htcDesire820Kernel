// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_DIALOG_H_
#define CHROME_BROWSER_FIRST_RUN_DIALOG_H_

#import <Cocoa/Cocoa.h>

@interface FirstRunDialogController : NSWindowController {
 @private
  BOOL statsEnabled_;
  BOOL makeDefaultBrowser_;

  IBOutlet NSArray* objectsToSize_;
  IBOutlet NSButton* setAsDefaultCheckbox_;
  IBOutlet NSButton* statsCheckbox_;
  BOOL beenSized_;
}

- (IBAction)ok:(id)sender;

- (IBAction)learnMore:(id)sender;

@property(assign, nonatomic) BOOL statsEnabled;
@property(assign, nonatomic) BOOL makeDefaultBrowser;

@end

#endif  

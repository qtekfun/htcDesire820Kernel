// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_ABOUT_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_ABOUT_WINDOW_CONTROLLER_H_
#pragma once

#import <AppKit/AppKit.h>

@class BackgroundTileView;
class Profile;

@interface AboutLegalTextView : NSTextView
@end

@interface AboutWindowController : NSWindowController {
 @private
  IBOutlet NSTextField* version_;
  IBOutlet BackgroundTileView* backgroundView_;
  IBOutlet NSImageView* logoView_;
  IBOutlet NSView* legalBlock_;
  IBOutlet AboutLegalTextView* legalText_;

  
  
  IBOutlet NSView* updateBlock_;

  IBOutlet NSProgressIndicator* spinner_;
  IBOutlet NSImageView* updateStatusIndicator_;
  IBOutlet NSTextField* updateText_;
  IBOutlet NSButton* updateNowButton_;
  IBOutlet NSButton* promoteButton_;

  Profile* profile_;  

  
  
  CGFloat windowHeight_;
}

- (id)initWithProfile:(Profile*)profile;

- (IBAction)updateNow:(id)sender;

- (IBAction)promoteUpdater:(id)sender;

@end  

@interface AboutWindowController(JustForTesting)

- (NSTextView*)legalText;
- (NSButton*)updateButton;
- (NSTextField*)updateText;

+ (NSAttributedString*)legalTextBlock;

@end  

#endif  

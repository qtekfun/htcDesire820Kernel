// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_SPEECH_INPUT_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_SPEECH_INPUT_WINDOW_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "chrome/browser/speech/speech_input_bubble.h"
#include "chrome/browser/ui/cocoa/base_bubble_controller.h"

@interface SpeechInputWindowController : BaseBubbleController {
 @private
  SpeechInputBubble::Delegate* delegate_;  
  SpeechInputBubbleBase::DisplayMode displayMode_;

  
  IBOutlet NSImageView* iconImage_;
  IBOutlet NSTextField* instructionLabel_;
  IBOutlet NSButton* cancelButton_;
  IBOutlet NSButton* tryAgainButton_;
  IBOutlet NSButton* micSettingsButton_;
}

- (id)initWithParentWindow:(NSWindow*)parentWindow
                  delegate:(SpeechInputBubbleDelegate*)delegate
                anchoredAt:(NSPoint)anchoredAt;

- (IBAction)cancel:(id)sender;

- (IBAction)tryAgain:(id)sender;

- (IBAction)micSettings:(id)sender;

- (void)updateLayout:(SpeechInputBubbleBase::DisplayMode)mode
         messageText:(const string16&)messageText
           iconImage:(NSImage*)iconImage;

- (void)show;

- (void)hide;

- (void)setImage:(NSImage*)image;

@end

#endif  

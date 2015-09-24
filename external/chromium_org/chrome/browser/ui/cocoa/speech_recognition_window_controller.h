// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_SPEECH_RECOGNITION_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_SPEECH_RECOGNITION_WINDOW_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "chrome/browser/speech/speech_recognition_bubble.h"
#include "chrome/browser/ui/cocoa/base_bubble_controller.h"

@interface SpeechRecognitionWindowController : BaseBubbleController {
 @private
  SpeechRecognitionBubble::Delegate* delegate_;  
  SpeechRecognitionBubbleBase::DisplayMode displayMode_;

  
  IBOutlet NSImageView* iconImage_;
  IBOutlet NSTextField* instructionLabel_;
  IBOutlet NSButton* cancelButton_;
  IBOutlet NSButton* tryAgainButton_;
  IBOutlet NSButton* micSettingsButton_;
}

- (id)initWithParentWindow:(NSWindow*)parentWindow
                  delegate:(SpeechRecognitionBubbleDelegate*)delegate
                anchoredAt:(NSPoint)anchoredAt;

- (IBAction)cancel:(id)sender;

- (IBAction)tryAgain:(id)sender;

- (IBAction)micSettings:(id)sender;

- (void)updateLayout:(SpeechRecognitionBubbleBase::DisplayMode)mode
         messageText:(const base::string16&)messageText
           iconImage:(NSImage*)iconImage;

- (void)show;

- (void)hide;

- (void)setImage:(NSImage*)image;

@end

#endif  

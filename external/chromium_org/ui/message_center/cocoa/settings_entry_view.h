// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_COCOA_SETTINGS_ENTRY_VIEW_H_
#define UI_MESSAGE_CENTER_COCOA_SETTINGS_ENTRY_VIEW_H_

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#import "ui/base/cocoa/hover_image_button.h"
#include "ui/message_center/notifier_settings.h"

@class MCSettingsController;

@interface MCSettingsEntryView : NSBox {
 @private
  
  MCSettingsController* controller_;

  
  message_center::Notifier* notifier_;

  
  
  base::scoped_nsobject<NSImage> notifierIcon_;

  
  
  base::scoped_nsobject<HoverImageButton> learnMoreButton_;

  
  base::scoped_nsobject<NSButton> checkbox_;

  
  BOOL hasSeparator_;
  base::scoped_nsobject<NSBox> separator_;
}

- (id)initWithController:(MCSettingsController*)controller
                notifier:(message_center::Notifier*)notifier
                   frame:(NSRect)frame
            hasSeparator:(BOOL)hasSeparator;

- (void)setNotifierIcon:(NSImage*)notifierIcon;

- (NSButton*)checkbox;

@end

@interface MCSettingsEntryView (TestingAPI)
- (void)clickLearnMore;
@end

#endif  

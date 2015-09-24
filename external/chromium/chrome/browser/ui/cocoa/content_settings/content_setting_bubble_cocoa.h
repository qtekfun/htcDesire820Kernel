// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"

class ContentSettingBubbleModel;
@class InfoBubbleView;

namespace content_setting_bubble {
typedef std::map<NSButton*, int> PopupLinks;
}

@interface ContentSettingBubbleController : BaseBubbleController {
 @private
  IBOutlet NSTextField* titleLabel_;
  IBOutlet NSMatrix* allowBlockRadioGroup_;

  IBOutlet NSButton* manageButton_;
  IBOutlet NSButton* doneButton_;
  IBOutlet NSButton* loadAllPluginsButton_;

  
  IBOutlet NSView* contentsContainer_;

  
  IBOutlet NSButton* infoButton_;

  IBOutlet NSTextField* blockedResourcesField_;

  scoped_ptr<ContentSettingBubbleModel> contentSettingBubbleModel_;
  content_setting_bubble::PopupLinks popupLinks_;
}

+ (ContentSettingBubbleController*)
    showForModel:(ContentSettingBubbleModel*)contentSettingBubbleModel
    parentWindow:(NSWindow*)parentWindow
      anchoredAt:(NSPoint)anchoredAt;

- (IBAction)allowBlockToggled:(id)sender;

- (IBAction)closeBubble:(id)sender;

- (IBAction)manageBlocking:(id)sender;

- (IBAction)showMoreInfo:(id)sender;

- (IBAction)loadAllPlugins:(id)sender;

@end

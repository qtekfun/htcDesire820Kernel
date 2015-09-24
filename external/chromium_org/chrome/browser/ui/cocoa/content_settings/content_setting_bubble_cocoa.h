// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"
#include "content/public/common/media_stream_request.h"

class ContentSettingBubbleModel;
class ContentSettingMediaMenuModel;
@class InfoBubbleView;

namespace content_setting_bubble {
typedef std::map<NSButton*, int> PopupLinks;

struct MediaMenuParts {
  MediaMenuParts(content::MediaStreamType type, NSTextField* label);
  ~MediaMenuParts();

  content::MediaStreamType type;
  NSTextField* label;  
  scoped_ptr<ContentSettingMediaMenuModel> model;

 private:
  DISALLOW_COPY_AND_ASSIGN(MediaMenuParts);
};
typedef std::map<NSPopUpButton*, MediaMenuParts*> MediaMenuPartsMap;
}  

@interface ContentSettingBubbleController : BaseBubbleController {
 @private
  IBOutlet NSTextField* titleLabel_;
  IBOutlet NSMatrix* allowBlockRadioGroup_;

  IBOutlet NSButton* manageButton_;
  IBOutlet NSButton* doneButton_;
  IBOutlet NSButton* loadButton_;

  
  IBOutlet NSView* contentsContainer_;

  IBOutlet NSTextField* blockedResourcesField_;

  scoped_ptr<ContentSettingBubbleModel> contentSettingBubbleModel_;
  content_setting_bubble::PopupLinks popupLinks_;
  content_setting_bubble::MediaMenuPartsMap mediaMenus_;
}

+ (ContentSettingBubbleController*)
    showForModel:(ContentSettingBubbleModel*)contentSettingBubbleModel
    parentWindow:(NSWindow*)parentWindow
      anchoredAt:(NSPoint)anchoredAt;

- (IBAction)allowBlockToggled:(id)sender;

- (IBAction)closeBubble:(id)sender;

- (IBAction)manageBlocking:(id)sender;

- (IBAction)showMoreInfo:(id)sender;

- (IBAction)load:(id)sender;

- (IBAction)learnMoreLinkClicked:(id)sender;

- (IBAction)mediaMenuChanged:(id)sender;

@end

@interface ContentSettingBubbleController (TestingAPI)

- (content_setting_bubble::MediaMenuPartsMap*)mediaMenus;

@end

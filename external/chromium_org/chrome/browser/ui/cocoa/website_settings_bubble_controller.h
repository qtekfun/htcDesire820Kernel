// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"
#include "chrome/browser/ui/website_settings/website_settings_ui.h"

class WebsiteSettingsUIBridge;

namespace content {
class WebContents;
}

@interface WebsiteSettingsBubbleController : BaseBubbleController {
 @private
  content::WebContents* webContents_;

  base::scoped_nsobject<NSView> contentView_;
  base::scoped_nsobject<NSSegmentedControl> segmentedControl_;
  base::scoped_nsobject<NSTabView> tabView_;

  
  NSTextField* identityField_;

  
  NSTextField* identityStatusField_;

  
  NSView* permissionsTabContentView_;

  
  NSView* connectionTabContentView_;

  
  NSView* cookiesView_;

  
  NSButton* cookiesButton_;

  
  NSButton* certificateInfoButton_;

  
  
  int certificateId_;

  
  NSView* permissionsView_;

  NSImageView* identityStatusIcon_;
  NSTextField* identityStatusDescriptionField_;
  NSView* separatorAfterIdentity_;

  NSImageView* connectionStatusIcon_;
  NSTextField* connectionStatusDescriptionField_;
  NSView* separatorAfterConnection_;

  NSImageView* firstVisitIcon_;
  NSTextField* firstVisitHeaderField_;
  NSTextField* firstVisitDescriptionField_;
  NSView* separatorAfterFirstVisit_;

  
  
  NSButton* helpButton_;

  
  
  scoped_ptr<WebsiteSettings> presenter_;

  
  
  scoped_ptr<WebsiteSettingsUIBridge> bridge_;
}

- (id)initWithParentWindow:(NSWindow*)parentWindow
   websiteSettingsUIBridge:(WebsiteSettingsUIBridge*)bridge
               webContents:(content::WebContents*)webContents
            isInternalPage:(BOOL)isInternalPage;

- (CGFloat)defaultWindowWidth;

@end

class WebsiteSettingsUIBridge : public WebsiteSettingsUI {
 public:
  WebsiteSettingsUIBridge();
  virtual ~WebsiteSettingsUIBridge();

  
  
  
  
  
  static void Show(gfx::NativeWindow parent,
                   Profile* profile,
                   content::WebContents* web_contents,
                   const GURL& url,
                   const content::SSLStatus& ssl);

  void set_bubble_controller(
      WebsiteSettingsBubbleController* bubble_controller);

  
  virtual void SetCookieInfo(const CookieInfoList& cookie_info_list) OVERRIDE;
  virtual void SetPermissionInfo(
      const PermissionInfoList& permission_info_list) OVERRIDE;
  virtual void SetIdentityInfo(const IdentityInfo& identity_info) OVERRIDE;
  virtual void SetFirstVisit(const base::string16& first_visit) OVERRIDE;
  virtual void SetSelectedTab(TabId tab_id) OVERRIDE;

 private:
  
  WebsiteSettingsBubbleController* bubble_controller_;

  DISALLOW_COPY_AND_ASSIGN(WebsiteSettingsUIBridge);
};

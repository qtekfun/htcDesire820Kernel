// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/browsing_data/cookies_tree_model.h"
#include "chrome/browser/ui/cocoa/constrained_window/constrained_window_mac.h"
#import "chrome/browser/ui/cocoa/content_settings/cookie_tree_node.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

@class CollectedCookiesWindowController;
@class CookieDetailsViewController;
@class VerticalGradientView;

namespace content {
class WebContents;
}

class CollectedCookiesMac : public ConstrainedWindowMacDelegate,
                            public content::NotificationObserver {
 public:
  CollectedCookiesMac(content::WebContents* web_contents);
  virtual ~CollectedCookiesMac();

  void PerformClose();

  
  virtual void OnConstrainedWindowClosed(
      ConstrainedWindowMac* window) OVERRIDE;

  CollectedCookiesWindowController* sheet_controller() const {
    return sheet_controller_.get();
  }

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  scoped_ptr<ConstrainedWindowMac> window_;

  base::scoped_nsobject<CollectedCookiesWindowController> sheet_controller_;

  DISALLOW_COPY_AND_ASSIGN(CollectedCookiesMac);
};

@interface CollectedCookiesWindowController : NSWindowController
                                              <NSOutlineViewDelegate,
                                               NSTabViewDelegate,
                                               NSWindowDelegate> {
 @private
  
  scoped_ptr<CookiesTreeModel> allowedTreeModel_;
  scoped_ptr<CookiesTreeModel> blockedTreeModel_;

  
  base::scoped_nsobject<NSMutableArray> icons_;

  
  base::scoped_nsobject<CocoaCookieTreeNode> cocoaAllowedTreeModel_;
  base::scoped_nsobject<CocoaCookieTreeNode> cocoaBlockedTreeModel_;

  BOOL allowedCookiesButtonsEnabled_;
  BOOL blockedCookiesButtonsEnabled_;

  IBOutlet NSTreeController* allowedTreeController_;
  IBOutlet NSTreeController* blockedTreeController_;
  IBOutlet NSOutlineView* allowedOutlineView_;
  IBOutlet NSOutlineView* blockedOutlineView_;
  IBOutlet VerticalGradientView* infoBar_;
  IBOutlet NSImageView* infoBarIcon_;
  IBOutlet NSTextField* infoBarText_;
  IBOutlet NSTabView* tabView_;
  IBOutlet NSScrollView* blockedScrollView_;
  IBOutlet NSTextField* blockedCookiesText_;
  IBOutlet NSView* cookieDetailsViewPlaceholder_;

  base::scoped_nsobject<NSViewAnimation> animation_;

  base::scoped_nsobject<CookieDetailsViewController> detailsViewController_;

  content::WebContents* webContents_;  

  CollectedCookiesMac* collectedCookiesMac_;  

  BOOL infoBarVisible_;

  BOOL contentSettingsChanged_;
}

@property(readonly, nonatomic) IBOutlet NSTreeController* allowedTreeController;
@property(readonly, nonatomic) IBOutlet NSTreeController* blockedTreeController;
@property(readonly, nonatomic) IBOutlet NSOutlineView* allowedOutlineView;
@property(readonly, nonatomic) IBOutlet NSOutlineView* blockedOutlineView;
@property(readonly, nonatomic) IBOutlet VerticalGradientView* infoBar;
@property(readonly, nonatomic) IBOutlet NSImageView* infoBarIcon;
@property(readonly, nonatomic) IBOutlet NSTextField* infoBarText;
@property(readonly, nonatomic) IBOutlet NSTabView* tabView;
@property(readonly, nonatomic) IBOutlet NSScrollView* blockedScrollView;
@property(readonly, nonatomic) IBOutlet NSTextField* blockedCookiesText;
@property(readonly, nonatomic) IBOutlet NSView* cookieDetailsViewPlaceholder;

@property(assign, nonatomic) BOOL allowedCookiesButtonsEnabled;
@property(assign, nonatomic) BOOL blockedCookiesButtonsEnabled;

- (id)initWithWebContents:(content::WebContents*)webContents
      collectedCookiesMac:(CollectedCookiesMac*)collectedCookiesMac;

- (IBAction)closeSheet:(id)sender;

- (IBAction)allowOrigin:(id)sender;
- (IBAction)allowForSessionFromOrigin:(id)sender;
- (IBAction)blockOrigin:(id)sender;

- (CocoaCookieTreeNode*)cocoaAllowedTreeModel;
- (CocoaCookieTreeNode*)cocoaBlockedTreeModel;
- (void)setCocoaAllowedTreeModel:(CocoaCookieTreeNode*)model;
- (void)setCocoaBlockedTreeModel:(CocoaCookieTreeNode*)model;

- (CookiesTreeModel*)allowedTreeModel;
- (CookiesTreeModel*)blockedTreeModel;

- (void)loadTreeModelFromWebContents;
@end

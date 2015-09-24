// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/weak_ptr.h"

@protocol InfoBarContainerControllerBase;
class InfoBarCocoa;
class InfoBarDelegate;
class InfoBarService;
@class InfoBarGradientView;

@interface InfoBarController : NSViewController<NSTextViewDelegate> {
 @private
  id<InfoBarContainerControllerBase> containerController_;  
  base::WeakPtr<InfoBarCocoa> infobar_;

 @protected
  IBOutlet InfoBarGradientView* infoBarView_;
  IBOutlet NSImageView* image_;
  IBOutlet NSTextField* labelPlaceholder_;
  IBOutlet NSButton* okButton_;
  IBOutlet NSButton* cancelButton_;
  IBOutlet NSButton* closeButton_;

  
  
  
  
  base::scoped_nsobject<NSTextView> label_;
}

@property(nonatomic, assign)
    id<InfoBarContainerControllerBase> containerController;
@property(nonatomic, readonly) InfoBarDelegate* delegate;
@property(nonatomic, readonly) InfoBarCocoa* infobar;

- (id)initWithInfoBar:(InfoBarCocoa*)infobar;

- (BOOL)isOwned;

- (void)ok:(id)sender;
- (void)cancel:(id)sender;

- (IBAction)dismiss:(id)sender;

- (void)removeSelf;

- (void)addAdditionalControls;

- (void)infobarWillHide;

- (void)infobarWillClose;

- (void)removeButtons;

- (void)layoutArrow;

@end

@interface InfoBarController (Protected)
- (void)disablePopUpMenu:(NSMenu*)menu;
@end


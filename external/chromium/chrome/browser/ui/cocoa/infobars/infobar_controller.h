// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"

@class AnimatableView;
@class HoverCloseButton;
@protocol InfoBarContainer;
class InfoBarDelegate;
@class InfoBarGradientView;

@interface InfoBarController : NSViewController<NSTextViewDelegate> {
 @private
  id<InfoBarContainer> containerController_;  
  BOOL infoBarClosing_;

 @protected
  IBOutlet InfoBarGradientView* infoBarView_;
  IBOutlet NSImageView* image_;
  IBOutlet NSTextField* labelPlaceholder_;
  IBOutlet NSButton* okButton_;
  IBOutlet NSButton* cancelButton_;
  IBOutlet HoverCloseButton* closeButton_;

  
  
  
  InfoBarDelegate* delegate_;  

  
  
  
  
  scoped_nsobject<NSTextView> label_;
};

- (id)initWithDelegate:(InfoBarDelegate*)delegate;

- (void)ok:(id)sender;
- (void)cancel:(id)sender;

- (IBAction)dismiss:(id)sender;

- (AnimatableView*)animatableView;

- (void)open;
- (void)animateOpen;

- (void)close;
- (void)animateClosed;

- (void)addAdditionalControls;

- (void)infobarWillClose;

- (void)setLabelToMessage:(NSString*)message;

- (void)removeButtons;

@property(nonatomic, assign) id<InfoBarContainer> containerController;
@property(nonatomic, readonly) InfoBarDelegate* delegate;

@end


@interface LinkInfoBarController : InfoBarController
- (void)linkClicked;
@end


@interface ConfirmInfoBarController : InfoBarController
- (IBAction)ok:(id)sender;
- (IBAction)cancel:(id)sender;
- (void)linkClicked;
@end

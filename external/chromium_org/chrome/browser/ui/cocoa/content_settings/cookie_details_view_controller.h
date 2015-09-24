// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "net/cookies/cookie_monster.h"

@class CocoaCookieTreeNode;
@class GTMUILocalizerAndLayoutTweaker;

@interface CookieDetailsViewController : NSViewController {
 @private
  
  
  IBOutlet NSObjectController* objectController_;

  
  
  
  
  IBOutlet GTMUILocalizerAndLayoutTweaker* tweaker_;
}

@property(nonatomic, readonly) BOOL hasExpiration;

- (id)init;

- (void)setContentObject:(id)content;

- (void)shrinkViewToFit;

- (void)configureBindingsForTreeController:(NSTreeController*)controller;

- (IBAction)setCookieDoesntHaveExplicitExpiration:(id)sender;

- (IBAction)setCookieHasExplicitExpiration:(id)sender;

@end


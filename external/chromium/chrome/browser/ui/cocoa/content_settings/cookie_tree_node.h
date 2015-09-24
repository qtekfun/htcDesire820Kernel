// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/cookies_tree_model.h"
#include "chrome/browser/ui/cocoa/content_settings/cookie_details.h"

@interface CocoaCookieTreeNode : NSObject {
  scoped_nsobject<NSString> title_;
  scoped_nsobject<NSMutableArray> children_;
  scoped_nsobject<CocoaCookieDetails> details_;
  CookieTreeNode* treeNode_;  
}

- (id)initWithNode:(CookieTreeNode*)node;

- (void)rebuild;

- (NSString*)title;
- (CocoaCookieDetailsType)nodeType;
- (ui::TreeModelNode*)treeNode;

- (NSMutableArray*)mutableChildren;
- (NSArray*)children;
- (BOOL)isLeaf;

- (CocoaCookieDetails*)details;

@end
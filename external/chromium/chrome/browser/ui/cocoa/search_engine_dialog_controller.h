// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include <vector>

#import "base/memory/ref_counted.h"
#import "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

class Profile;
class SearchEngineDialogControllerBridge;
class TemplateURL;
class TemplateURLModel;

@interface SearchEngineDialogController : NSWindowController {
 @private
  
  Profile* profile_;

  
  bool randomize_;

  
  TemplateURLModel* searchEnginesModel_;

  
  scoped_refptr<SearchEngineDialogControllerBridge> bridge_;

  
  std::vector<const TemplateURL*> choices_;

  IBOutlet NSImageView* headerImageView_;
  IBOutlet NSView* searchEngineView_;
}

@property(assign, nonatomic) Profile* profile;
@property(assign, nonatomic) bool randomize;

@property(readonly) NSFont* mainLabelFont;

@end

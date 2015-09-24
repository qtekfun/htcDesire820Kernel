// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_model_observer_for_cocoa.h"

class BookmarkBubbleNotificationBridge;
class BookmarkModel;
class BookmarkNode;
@class BookmarkBubbleController;
@class InfoBubbleView;


@interface BookmarkBubbleController : NSWindowController<NSWindowDelegate> {
 @private
  NSWindow* parentWindow_;  

  
  BookmarkModel* model_;  
  const BookmarkNode* node_;  

  
  const BookmarkNode* pulsingBookmarkNode_;  

  BOOL alreadyBookmarked_;

  
  scoped_ptr<BookmarkModelObserverForCocoa> bookmark_observer_;

  
  scoped_ptr<BookmarkBubbleNotificationBridge> chrome_observer_;

  IBOutlet NSTextField* bigTitle_;   
  IBOutlet NSTextField* nameTextField_;
  IBOutlet NSPopUpButton* folderPopUpButton_;
  IBOutlet InfoBubbleView* bubble_;  
}

@property(readonly, nonatomic) const BookmarkNode* node;

- (id)initWithParentWindow:(NSWindow*)parentWindow
                     model:(BookmarkModel*)model
                      node:(const BookmarkNode*)node
         alreadyBookmarked:(BOOL)alreadyBookmarked;

- (IBAction)ok:(id)sender;
- (IBAction)remove:(id)sender;
- (IBAction)cancel:(id)sender;

- (IBAction)edit:(id)sender;
- (IBAction)folderChanged:(id)sender;

@end


@interface BookmarkBubbleController(ExposedForUnitTesting)
- (void)addFolderNodes:(const BookmarkNode*)parent
         toPopUpButton:(NSPopUpButton*)button
           indentation:(int)indentation;
- (void)setTitle:(NSString*)title parentFolder:(const BookmarkNode*)parent;
- (void)setParentFolderSelection:(const BookmarkNode*)parent;
+ (NSString*)chooseAnotherFolderString;
- (NSPopUpButton*)folderPopUpButton;
@end

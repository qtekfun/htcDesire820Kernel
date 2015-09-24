// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/base_bubble_controller.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_model_observer_for_cocoa.h"

class BookmarkModel;
class BookmarkNode;
@class BookmarkBubbleController;
@class BookmarkSyncPromoController;

@interface BookmarkBubbleController : BaseBubbleController {
 @private
  
  BookmarkModel* model_;  
  const BookmarkNode* node_;  

  
  const BookmarkNode* pulsingBookmarkNode_;  

  BOOL alreadyBookmarked_;

  
  scoped_ptr<BookmarkModelObserverForCocoa> bookmarkObserver_;

  
  base::scoped_nsobject<BookmarkSyncPromoController> syncPromoController_;

  IBOutlet NSTextField* bigTitle_;   
  IBOutlet NSTextField* nameTextField_;
  IBOutlet NSPopUpButton* folderPopUpButton_;
  IBOutlet NSView* syncPromoPlaceholder_;
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


@interface BookmarkBubbleController (ExposedForUnitTesting)

@property(nonatomic, readonly) NSView* syncPromoPlaceholder;

- (void)addFolderNodes:(const BookmarkNode*)parent
         toPopUpButton:(NSPopUpButton*)button
           indentation:(int)indentation;
- (void)setTitle:(NSString*)title parentFolder:(const BookmarkNode*)parent;
- (void)setParentFolderSelection:(const BookmarkNode*)parent;
+ (NSString*)chooseAnotherFolderString;
- (NSPopUpButton*)folderPopUpButton;
@end

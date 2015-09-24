// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>
#include <vector>
#import "chrome/browser/ui/cocoa/draggable_button.h"
#include "ui/base/window_open_disposition.h"

@class BookmarkBarFolderController;
@class BookmarkButton;
struct BookmarkNodeData;
class BookmarkModel;
class BookmarkNode;
@class BrowserWindowController;
class ThemeService;

@protocol BookmarkButtonDelegate

- (void)fillPasteboard:(NSPasteboard*)pboard
       forDragOfButton:(BookmarkButton*)button;

- (void)mouseEnteredButton:(id)button event:(NSEvent*)event;
- (void)mouseExitedButton:(id)button event:(NSEvent*)event;

- (BOOL)dragShouldLockBarVisibility;

- (NSWindow*)browserWindow;

- (BOOL)canDragBookmarkButtonToTrash:(BookmarkButton*)button;

- (void)didDragBookmarkToTrash:(BookmarkButton*)button;

- (void)bookmarkDragDidEnd:(BookmarkButton*)button
                 operation:(NSDragOperation)operation;

@end


@protocol BookmarkButtonControllerProtocol

- (void)closeAllBookmarkFolders;

- (void)closeBookmarkFolder:(id)sender;

- (BookmarkModel*)bookmarkModel;

- (BOOL)draggingAllowed:(id<NSDraggingInfo>)info;
- (NSDragOperation)draggingEntered:(id<NSDraggingInfo>)info;
- (void)draggingExited:(id<NSDraggingInfo>)info;

- (BOOL)dragShouldLockBarVisibility;


- (BOOL)dragButton:(BookmarkButton*)sourceButton
                to:(NSPoint)point
              copy:(BOOL)copy;

- (BOOL)dragBookmarkData:(id<NSDraggingInfo>)info;

- (std::vector<const BookmarkNode*>)retrieveBookmarkNodeData;

- (BOOL)shouldShowIndicatorShownForPoint:(NSPoint)point;

- (CGFloat)indicatorPosForDragToPoint:(NSPoint)point;

- (void)setDropInsertionPos:(CGFloat)where;

- (void)clearDropInsertionPos;

- (ThemeService*)themeService;

- (void)childFolderWillShow:(id<BookmarkButtonControllerProtocol>)child;

- (void)childFolderWillClose:(id<BookmarkButtonControllerProtocol>)child;

- (BookmarkBarFolderController*)folderController;

- (void)addNewFolderControllerWithParentButton:(BookmarkButton*)parentButton;

- (void)openAll:(const BookmarkNode*)node
    disposition:(WindowOpenDisposition)disposition;


- (void)addButtonForNode:(const BookmarkNode*)node
                 atIndex:(NSInteger)buttonIndex;

- (BOOL)addURLs:(NSArray*)urls withTitles:(NSArray*)titles at:(NSPoint)point;

- (void)moveButtonFromIndex:(NSInteger)fromIndex toIndex:(NSInteger)toIndex;

- (void)removeButton:(NSInteger)buttonIndex animate:(BOOL)poof;

- (id<BookmarkButtonControllerProtocol>)controllerForNode:
      (const BookmarkNode*)node;

@end  


@interface BookmarkButton : DraggableButton {
 @private
  IBOutlet NSObject<BookmarkButtonDelegate>* delegate_;  

  
  
  
  
  
  
  BrowserWindowController* visibilityDelegate_;  

  NSPoint dragMouseOffset_;
  NSPoint dragEndScreenLocation_;
  BOOL dragPending_;
  BOOL acceptsTrackIn_;
  NSTrackingArea* area_;
}

@property(assign, nonatomic) NSObject<BookmarkButtonDelegate>* delegate;
@property(assign, nonatomic) BOOL acceptsTrackIn;

- (const BookmarkNode*)bookmarkNode;

- (BOOL)isFolder;

- (void)mouseDragged:(NSEvent*)theEvent;

- (BOOL)acceptsTrackInFrom:(id)sender;

- (BOOL)isEmpty;

- (void)setIsContinuousPulsing:(BOOL)flag;

- (BOOL)isContinuousPulsing;

- (NSPoint)screenLocationForRemoveAnimation;

+ (BookmarkButton*)draggedButton;


@end  


@interface BookmarkButton(TestingAPI)
- (void)beginDrag:(NSEvent*)event;
@end

namespace bookmark_button {

extern NSString* const kPulseBookmarkButtonNotification;

extern NSString* const kBookmarkKey;

extern NSString* const kBookmarkPulseFlagKey;

};

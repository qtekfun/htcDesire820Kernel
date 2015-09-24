// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_EDITOR_BASE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_EDITOR_BASE_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/bookmarks/bookmark_expanded_state_tracker.h"
#include "chrome/browser/ui/bookmarks/bookmark_editor.h"

class BookmarkEditorBaseControllerBridge;
class BookmarkModel;
@class BookmarkTreeBrowserCell;

@interface BookmarkEditorBaseController : NSWindowController {
 @private
  IBOutlet NSButton* newFolderButton_;
  IBOutlet NSButton* okButton_;  
  IBOutlet NSTreeController* folderTreeController_;
  IBOutlet NSOutlineView* folderTreeView_;

  NSWindow* parentWindow_;  
  Profile* profile_;  
  const BookmarkNode* parentNode_;  
  GURL url_;  
  base::string16 title_;
  BookmarkEditor::Configuration configuration_;
  NSString* initialName_;
  NSString* displayName_;  
  BOOL okEnabled_;  
  BOOL creatingNewFolders_;  
  
  
  base::scoped_nsobject<NSArray> folderTreeArray_;
  
  
  base::scoped_nsobject<NSArray> tableSelectionPaths_;
  
  scoped_ptr<BookmarkEditorBaseControllerBridge> observer_;
}

@property(nonatomic, copy) NSString* initialName;
@property(nonatomic, copy) NSString* displayName;
@property(nonatomic, assign) BOOL okEnabled;
@property(nonatomic, retain, readonly) NSArray* folderTreeArray;
@property(nonatomic, copy) NSArray* tableSelectionPaths;

- (id)initWithParentWindow:(NSWindow*)parentWindow
                   nibName:(NSString*)nibName
                   profile:(Profile*)profile
                    parent:(const BookmarkNode*)parent
                       url:(const GURL&)url
                     title:(const base::string16&)title
             configuration:(BookmarkEditor::Configuration)configuration;

- (void)runAsModalSheet;

- (IBAction)newFolder:(id)sender;

- (IBAction)cancel:(id)sender;

- (IBAction)ok:(id)sender;


- (const BookmarkNode*)selectedNode;

- (void)expandNodes:(const BookmarkExpandedStateTracker::Nodes&)nodes;

- (BookmarkExpandedStateTracker::Nodes)getExpandedNodes;

- (void)selectNodeInBrowser:(const BookmarkNode*)node;

- (void)nodeRemoved:(const BookmarkNode*)node
         fromParent:(const BookmarkNode*)parent;
- (void)modelChangedPreserveSelection:(BOOL)preserve;

- (BookmarkModel*)bookmarkModel;
- (Profile*)profile;
- (const BookmarkNode*)parentNode;
- (const GURL&)url;
- (const base::string16&)title;

@end

@interface BookmarkFolderInfo : NSObject {
 @private
  NSString* folderName_;
  const BookmarkNode* folderNode_;  
  NSMutableArray* children_;
  BOOL newFolder_;
}

@property(nonatomic, copy) NSString* folderName;
@property(nonatomic, assign) const BookmarkNode* folderNode;
@property(nonatomic, retain) NSMutableArray* children;
@property(nonatomic, assign) BOOL newFolder;

+ (id)bookmarkFolderInfoWithFolderName:(NSString*)folderName;

- (id)initWithFolderName:(NSString*)folderName
              folderNode:(const BookmarkNode*)folderNode
                children:(NSMutableArray*)children
               newFolder:(BOOL)newFolder;

+ (id)bookmarkFolderInfoWithFolderName:(NSString*)folderName
                            folderNode:(const BookmarkNode*)folderNode
                              children:(NSMutableArray*)children;

@end

@interface BookmarkEditorBaseController(TestingAPI)

@property(nonatomic, readonly) BOOL okButtonEnabled;

- (void)createNewFolders;

- (void)selectTestNodeInBrowser:(const BookmarkNode*)node;

- (BookmarkFolderInfo*)selectedFolder;

@end

#endif  

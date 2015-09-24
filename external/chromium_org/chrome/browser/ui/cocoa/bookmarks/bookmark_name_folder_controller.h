// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_NAME_FOLDER_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_NAME_FOLDER_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/bookmarks/bookmark_model.h"

class BookmarkModelObserverForCocoa;

@interface BookmarkNameFolderController : NSWindowController {
 @private
  IBOutlet NSTextField* nameField_;
  IBOutlet NSButton* okButton_;

  NSWindow* parentWindow_;  
  Profile* profile_;  

  
  
  
  const BookmarkNode* node_;
  const BookmarkNode* parent_;
  int newIndex_;

  base::scoped_nsobject<NSString> initialName_;

  
  scoped_ptr<BookmarkModelObserverForCocoa> observer_;
}

- (id)initWithParentWindow:(NSWindow*)window
                   profile:(Profile*)profile
                      node:(const BookmarkNode*)node;
- (id)initWithParentWindow:(NSWindow*)window
                   profile:(Profile*)profile
                    parent:(const BookmarkNode*)parent
                  newIndex:(int)newIndex;
- (void)runAsModalSheet;
- (IBAction)cancel:(id)sender;
- (IBAction)ok:(id)sender;
@end

@interface BookmarkNameFolderController(TestingAPI)
- (NSString*)folderName;
- (void)setFolderName:(NSString*)name;
- (NSButton*)okButton;
@end

#endif  

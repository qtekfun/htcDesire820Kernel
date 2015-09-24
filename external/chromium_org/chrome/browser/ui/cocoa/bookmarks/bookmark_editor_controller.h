// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_EDITOR_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_EDITOR_CONTROLLER_H_

#import "chrome/browser/ui/cocoa/bookmarks/bookmark_editor_base_controller.h"

@interface BookmarkEditorController : BookmarkEditorBaseController {
 @private
  const BookmarkNode* node_;  
  base::scoped_nsobject<NSString> initialUrl_;
  NSString* displayURL_;  
  IBOutlet NSTextField* urlField_;
  IBOutlet NSTextField* nameTextField_;
}

@property(nonatomic, copy) NSString* displayURL;

- (id)initWithParentWindow:(NSWindow*)parentWindow
                   profile:(Profile*)profile
                    parent:(const BookmarkNode*)parent
                      node:(const BookmarkNode*)node
                       url:(const GURL&)url
                     title:(const base::string16&)title
             configuration:(BookmarkEditor::Configuration)configuration;

@end

@interface BookmarkEditorController (UnitTesting)
- (NSColor *)urlFieldColor;
@end

#endif  

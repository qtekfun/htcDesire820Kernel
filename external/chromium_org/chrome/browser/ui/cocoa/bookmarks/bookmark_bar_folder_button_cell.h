// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_FOLDER_BUTTON_CELL_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_FOLDER_BUTTON_CELL_H_

#import "chrome/browser/ui/cocoa/bookmarks/bookmark_button_cell.h"

@interface BookmarkBarFolderButtonCell : BookmarkButtonCell

+ (id)buttonCellForNode:(const BookmarkNode*)node
                   text:(NSString*)text
                  image:(NSImage*)image
         menuController:(BookmarkContextMenuCocoaController*)menuController;

@end

#endif  

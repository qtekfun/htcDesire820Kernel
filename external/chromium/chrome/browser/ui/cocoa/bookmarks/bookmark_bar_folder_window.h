// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_FOLDER_WINDOW_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_BAR_FOLDER_WINDOW_H_
#pragma once

#import <Cocoa/Cocoa.h>
#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"


@interface BookmarkBarFolderWindow : NSWindow
@end

@interface BookmarkBarFolderWindowContentView : NSView
@end

@interface BookmarkBarFolderWindowScrollView : NSScrollView
@end


#endif  

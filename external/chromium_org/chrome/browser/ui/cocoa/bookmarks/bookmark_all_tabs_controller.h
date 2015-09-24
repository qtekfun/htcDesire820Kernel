// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_ALL_TABS_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_ALL_TABS_CONTROLLER_H_

#include <utility>
#include <vector>

#include "base/strings/string16.h"
#import "chrome/browser/ui/cocoa/bookmarks/bookmark_editor_base_controller.h"

typedef std::pair<base::string16, GURL> ActiveTabNameURLPair;
typedef std::vector<ActiveTabNameURLPair> ActiveTabsNameURLPairVector;

@interface BookmarkAllTabsController : BookmarkEditorBaseController {
 @private
  ActiveTabsNameURLPairVector activeTabPairsVector_;
}

- (id)initWithParentWindow:(NSWindow*)parentWindow
                   profile:(Profile*)profile
                    parent:(const BookmarkNode*)parent
                       url:(const GURL&)url
                     title:(const base::string16&)title
             configuration:(BookmarkEditor::Configuration)configuration;

@end

@interface BookmarkAllTabsController(TestingAPI)

- (void)UpdateActiveTabPairs;

- (ActiveTabsNameURLPairVector*)activeTabPairsVector;

@end

#endif  

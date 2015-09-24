// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_ENHANCED_BOOKMARKS_FEATURES_H_
#define CHROME_BROWSER_BOOKMARKS_ENHANCED_BOOKMARKS_FEATURES_H_

#include <string>

bool IsEnhancedBookmarksExperimentEnabled();

bool IsEnableDomDistillerSet();

bool IsEnableSyncArticlesSet();

std::string GetEnhancedBookmarksExtensionId();

#endif  

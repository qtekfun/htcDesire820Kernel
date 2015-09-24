// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MEDIA_MEDIA_BROWSERTEST_H_
#define CONTENT_BROWSER_MEDIA_MEDIA_BROWSERTEST_H_

#include <utility>
#include <vector>

#include "content/test/content_browser_test.h"

namespace content {

class TitleWatcher;

class MediaBrowserTest : public ContentBrowserTest {
 public:
  typedef std::pair<const char*, const char*> StringPair;

  
  static const char kEnded[];
  static const char kError[];
  static const char kFailed[];

  virtual void SetUp() OVERRIDE;

  
  
  
  
  void RunMediaTestPage(const char* html_page,
                        std::vector<StringPair>* query_params,
                        const char* expected, bool http);

  
  
  void RunTest(const GURL& gurl, const char* expected);

  virtual void AddWaitForTitles(content::TitleWatcher* title_watcher);
};

} 

#endif  

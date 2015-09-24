// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_MEDIA_BROWSERTEST_H_
#define CHROME_BROWSER_MEDIA_MEDIA_BROWSERTEST_H_

#include <utility>
#include <vector>

#include "chrome/test/base/in_process_browser_test.h"

namespace content {
class TitleWatcher;
}

class MediaBrowserTest : public InProcessBrowserTest {
 protected:
  typedef std::pair<const char*, const char*> StringPair;

  
  static const char kEnded[];
  static const char kError[];
  static const char kFailed[];

  MediaBrowserTest();
  virtual ~MediaBrowserTest();

  
  
  
  
  void RunMediaTestPage(const char* html_page,
                        std::vector<StringPair>* query_params,
                        const char* expected, bool http);

  
  
  base::string16 RunTest(const GURL& gurl, const char* expected);

  virtual void AddWaitForTitles(content::TitleWatcher* title_watcher);
};

#endif  

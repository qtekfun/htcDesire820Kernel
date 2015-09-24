// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SEARCHBOX_SEARCH_BOUNCER_H_
#define CHROME_RENDERER_SEARCHBOX_SEARCH_BOUNCER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "content/public/renderer/render_process_observer.h"
#include "url/gurl.h"

namespace IPC {
class Message;
}

class SearchBouncer : public content::RenderProcessObserver {
 public:
  SearchBouncer();
  virtual ~SearchBouncer();

  
  
  bool ShouldFork(const GURL& url) const;

  
  bool IsNewTabPage(const GURL& url) const;

  
  void OnSetSearchURLs(std::vector<GURL> search_urls, GURL new_tab_page_url);

 private:
  FRIEND_TEST_ALL_PREFIXES(SearchBouncerTest, SetSearchURLs);

  
  virtual bool OnControlMessageReceived(const IPC::Message& message) OVERRIDE;

  
  std::vector<GURL> search_urls_;
  GURL new_tab_page_url_;

  DISALLOW_COPY_AND_ASSIGN(SearchBouncer);
};

#endif  

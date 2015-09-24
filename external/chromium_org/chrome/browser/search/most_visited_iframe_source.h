// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_MOST_VISITED_IFRAME_SOURCE_H_
#define CHROME_BROWSER_SEARCH_MOST_VISITED_IFRAME_SOURCE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/search/iframe_source.h"

class MostVisitedIframeSource : public IframeSource {
 public:
  MostVisitedIframeSource();
  virtual ~MostVisitedIframeSource();

  
  static const int kNumMostVisited;
  
  static const char kMostVisitedHistogramName[];

  
  virtual void StartDataRequest(
      const std::string& path_and_query,
      int render_process_id,
      int render_view_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;

 protected:
  
  virtual std::string GetSource() const OVERRIDE;

  virtual bool ServesPath(const std::string& path) const OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(MostVisitedIframeSourceTest,
                           LogEndpointIsValidWithProvider);

  
  
  
  void LogMostVisitedProviderClick(int position, const std::string& provider);

  
  
  static std::string GetHistogramNameForProvider(const std::string& provider);

  DISALLOW_COPY_AND_ASSIGN(MostVisitedIframeSource);
};

#endif  

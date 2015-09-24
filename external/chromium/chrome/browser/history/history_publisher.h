// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_PUBLISHER_H_
#define CHROME_BROWSER_HISTORY_HISTORY_PUBLISHER_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"

#if defined(OS_WIN)
#include "base/win/scoped_comptr.h"
#include "history_indexer.h"
#endif

class GURL;

namespace base {
class Time;
}

namespace history {

class HistoryPublisher {
 public:
  HistoryPublisher();
  ~HistoryPublisher();

  
  
  
  bool Init();

  void PublishPageThumbnail(const std::vector<unsigned char>& thumbnail,
                            const GURL& url, const base::Time& time) const;
  void PublishPageContent(const base::Time& time, const GURL& url,
                          const string16& title,
                          const string16& contents) const;
  void DeleteUserHistoryBetween(const base::Time& begin_time,
                                const base::Time& end_time) const;

 private:
  struct PageData {
    const base::Time& time;
    const GURL& url;
    const char16* html;
    const char16* title;
    const char* thumbnail_format;
    const std::vector<unsigned char>* thumbnail;
  };

  void PublishDataToIndexers(const PageData& page_data) const;

#if defined(OS_WIN)
  
  
  bool ReadRegisteredIndexersFromRegistry();

  
  
  static double TimeToUTCVariantTime(const base::Time& time);

  typedef std::vector< base::win::ScopedComPtr<
      IChromeHistoryIndexer> > IndexerList;

  
  IndexerList indexers_;

  
  
  static const wchar_t* const kRegKeyRegisteredIndexersInfo;
#endif

  
  static const char* const kThumbnailImageFormat;

  DISALLOW_COPY_AND_ASSIGN(HistoryPublisher);
};

}  

#endif  

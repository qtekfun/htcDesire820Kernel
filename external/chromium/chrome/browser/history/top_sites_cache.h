// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_CACHE_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_CACHE_H_
#pragma once

#include <algorithm>
#include <map>
#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/browser/history/history_types.h"

class RefCountedBytes;

namespace history {

class TopSitesCache {
 public:
  TopSitesCache();
  ~TopSitesCache();

  
  void SetTopSites(const MostVisitedURLList& top_sites);
  const MostVisitedURLList& top_sites() const { return top_sites_; }

  
  void SetThumbnails(const URLToImagesMap& images);
  const URLToImagesMap& images() const { return images_; }

  
  void SetPageThumbnail(const GURL& url,
                        RefCountedBytes* thumbnail,
                        const ThumbnailScore& score);

  
  
  Images* GetImage(const GURL& url);

  
  
  bool GetPageThumbnail(const GURL& url,
                        scoped_refptr<RefCountedBytes>* bytes);

  
  
  bool GetPageThumbnailScore(const GURL& url, ThumbnailScore* score);

  
  GURL GetCanonicalURL(const GURL& url);

  
  bool IsKnownURL(const GURL& url);

  
  size_t GetURLIndex(const GURL& url);

  
  void RemoveUnreferencedThumbnails();

 private:
  
  
  typedef std::pair<MostVisitedURL*, size_t> CanonicalURLEntry;

  
  class CanonicalURLComparator {
   public:
    bool operator()(const CanonicalURLEntry& e1,
                    const CanonicalURLEntry& e2) const {
      return e1.first->redirects[e1.second] < e2.first->redirects[e2.second];
    }
  };

  
  
  
  
  
  
  
  typedef std::map<CanonicalURLEntry, size_t,
                   CanonicalURLComparator> CanonicalURLs;

  
  void GenerateCanonicalURLs();

  
  void StoreRedirectChain(const RedirectList& redirects, size_t destination);

  
  CanonicalURLs::iterator GetCanonicalURLsIterator(const GURL& url);

  
  MostVisitedURLList top_sites_;

  
  URLToImagesMap images_;

  
  
  CanonicalURLs canonical_urls_;

  DISALLOW_COPY_AND_ASSIGN(TopSitesCache);
};

}  

#endif  

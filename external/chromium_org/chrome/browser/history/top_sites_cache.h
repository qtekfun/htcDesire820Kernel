// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_CACHE_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_CACHE_H_

#include <map>
#include <utility>

#include "base/memory/ref_counted.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/url_utils.h"
#include "url/gurl.h"

class GURL;

namespace history {


class TopSitesCache {
 public:
  TopSitesCache();
  ~TopSitesCache();

  
  
  void SetTopSites(const MostVisitedURLList& top_sites);
  const MostVisitedURLList& top_sites() const { return top_sites_; }

  
  void SetThumbnails(const URLToImagesMap& images);
  const URLToImagesMap& images() const { return images_; }

  
  
  Images* GetImage(const GURL& url);

  
  
  
  bool GetPageThumbnail(const GURL& url,
                        scoped_refptr<base::RefCountedMemory>* bytes) const;

  
  
  bool GetPageThumbnailScore(const GURL& url, ThumbnailScore* score) const;

  
  const GURL& GetCanonicalURL(const GURL& url) const;

  
  
  
  
  GURL GetGeneralizedCanonicalURL(const GURL& url) const;

  
  bool IsKnownURL(const GURL& url) const;

  
  size_t GetURLIndex(const GURL& url) const;

  
  size_t GetNumNonForcedURLs() const;

  
  size_t GetNumForcedURLs() const;

 private:
  
  
  typedef std::pair<MostVisitedURL*, size_t> CanonicalURLEntry;

  
  class CanonicalURLComparator {
   public:
    bool operator()(const CanonicalURLEntry& e1,
                    const CanonicalURLEntry& e2) const {
      return CanonicalURLStringCompare(e1.first->redirects[e1.second].spec(),
                                       e2.first->redirects[e2.second].spec());
    }
  };

  
  
  class CanonicalURLQuery {
   public:
    explicit CanonicalURLQuery(const GURL& url);
    ~CanonicalURLQuery();
    const CanonicalURLEntry& entry() { return entry_; }

   private:
    MostVisitedURL most_visited_url_;
    CanonicalURLEntry entry_;
  };

  
  
  
  
  
  
  
  typedef std::map<CanonicalURLEntry, size_t,
                   CanonicalURLComparator> CanonicalURLs;

  
  void CountForcedURLs();

  
  void GenerateCanonicalURLs();

  
  void StoreRedirectChain(const RedirectList& redirects, size_t destination);

  
  CanonicalURLs::const_iterator GetCanonicalURLsIterator(const GURL& url) const;

  
  const GURL& GetURLFromIterator(CanonicalURLs::const_iterator it) const;

  
  size_t num_forced_urls_;

  
  
  
  MostVisitedURLList top_sites_;

  
  URLToImagesMap images_;

  
  
  CanonicalURLs canonical_urls_;

  
  
  GURL::Replacements clear_query_ref_;

  
  
  GURL::Replacements clear_path_query_ref_;

  DISALLOW_COPY_AND_ASSIGN(TopSitesCache);
};

}  

#endif  

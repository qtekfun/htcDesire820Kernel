// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TOP_SITES_H_
#define CHROME_BROWSER_HISTORY_TOP_SITES_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/common/thumbnail_score.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/image/image.h"

class GURL;
class Profile;

namespace base {
class FilePath;
class RefCountedBytes;
class RefCountedMemory;
}

namespace history {

class TopSitesCache;

class TopSites
    : public base::RefCountedThreadSafe<TopSites>,
      public content::NotificationObserver {
 public:
  TopSites() {}

  
  static TopSites* Create(Profile* profile, const base::FilePath& db_name);

  
  
  
  
  virtual bool SetPageThumbnail(const GURL& url,
                                const gfx::Image& thumbnail,
                                const ThumbnailScore& score) = 0;

  
  
  virtual bool SetPageThumbnailToJPEGBytes(
      const GURL& url,
      const base::RefCountedMemory* memory,
      const ThumbnailScore& score) = 0;

  typedef base::Callback<void(const MostVisitedURLList&)>
      GetMostVisitedURLsCallback;

  
  
  
  
  
  virtual void GetMostVisitedURLs(
      const GetMostVisitedURLsCallback& callback,
      bool include_forced_urls) = 0;

  
  
  
  
  
  
  
  virtual bool GetPageThumbnail(
      const GURL& url,
      bool prefix_match,
      scoped_refptr<base::RefCountedMemory>* bytes) = 0;

  
  
  
  virtual bool GetPageThumbnailScore(const GURL& url,
                                     ThumbnailScore* score) = 0;

  
  
  
  virtual bool GetTemporaryPageThumbnailScore(const GURL& url,
                                              ThumbnailScore* score) = 0;

  
  
  virtual void SyncWithHistory() = 0;

  

  
  virtual bool HasBlacklistedItems() const = 0;

  
  virtual void AddBlacklistedURL(const GURL& url) = 0;

  
  virtual void RemoveBlacklistedURL(const GURL& url) = 0;

  
  
  virtual bool IsBlacklisted(const GURL& url) = 0;

  
  virtual void ClearBlacklistedURLs() = 0;

  
  virtual void Shutdown() = 0;

  
  
  
  virtual CancelableRequestProvider::Handle StartQueryForMostVisited() = 0;

  
  
  virtual bool IsKnownURL(const GURL& url) = 0;

  
  
  
  virtual const std::string& GetCanonicalURLString(const GURL& url) const = 0;

  
  
  
  virtual bool IsNonForcedFull() = 0;

  
  
  
  virtual bool IsForcedFull() = 0;

  virtual bool loaded() const = 0;

  
  virtual MostVisitedURLList GetPrepopulatePages() = 0;

  
  
  
  
  
  virtual bool AddForcedURL(const GURL& url, const base::Time& time) = 0;

  struct PrepopulatedPage {
    
    int url_id;
    
    int title_id;
    
    int favicon_id;
    
    int thumbnail_id;
    
    SkColor color;
  };
 protected:
  virtual ~TopSites() {}

 private:
  friend class base::RefCountedThreadSafe<TopSites>;
};

#if defined(OS_ANDROID)
extern const TopSites::PrepopulatedPage kPrepopulatedPages[1];
#else
extern const TopSites::PrepopulatedPage kPrepopulatedPages[2];
#endif

}  

#endif  

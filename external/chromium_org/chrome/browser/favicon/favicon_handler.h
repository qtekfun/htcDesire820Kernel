// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FAVICON_FAVICON_HANDLER_H_
#define CHROME_BROWSER_FAVICON_FAVICON_HANDLER_H_

#include <map>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/browser/favicon/favicon_tab_helper.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "chrome/common/ref_counted_util.h"
#include "content/public/common/favicon_url.h"
#include "ui/gfx/favicon_size.h"
#include "ui/gfx/image/image.h"
#include "url/gurl.h"

class FaviconHandlerDelegate;
class Profile;
class SkBitmap;

namespace base {
class RefCountedMemory;
}

namespace content {
class NavigationEntry;
}


class FaviconHandler {
 public:
  enum Type {
    FAVICON,
    TOUCH,
  };

  FaviconHandler(Profile* profile,
                 FaviconHandlerDelegate* delegate,
                 Type icon_type);
  virtual ~FaviconHandler();

  
  void FetchFavicon(const GURL& url);

  
  
  void OnUpdateFaviconURL(int32 page_id,
                          const std::vector<content::FaviconURL>& candidates);

  
  
  void ProcessCurrentUrl();

  
  
  
  
  
  void OnDidDownloadFavicon(
      int id,
      const GURL& image_url,
      const std::vector<SkBitmap>& bitmaps,
      const std::vector<gfx::Size>& original_bitmap_sizes);

  
  const std::deque<content::FaviconURL>& image_urls() const {
    return image_urls_;
  }

 protected:
  
  

  
  
  virtual content::NavigationEntry* GetEntry();

  
  virtual int DownloadFavicon(const GURL& image_url, int max_bitmap_size);

  
  virtual void UpdateFaviconMappingAndFetch(
      const GURL& page_url,
      const GURL& icon_url,
      chrome::IconType icon_type,
      const FaviconService::FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  virtual void GetFavicon(
      const GURL& icon_url,
      chrome::IconType icon_type,
      const FaviconService::FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  virtual void GetFaviconForURL(
      const GURL& page_url,
      int icon_types,
      const FaviconService::FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  virtual void SetHistoryFavicons(const GURL& page_url,
                                  const GURL& icon_url,
                                  chrome::IconType icon_type,
                                  const gfx::Image& image);

  virtual FaviconService* GetFaviconService();

  
  virtual bool ShouldSaveFavicon(const GURL& url);

  
  
  
  virtual void NotifyFaviconUpdated(bool icon_url_changed);

 private:
  friend class TestFaviconHandler; 

  struct DownloadRequest {
    DownloadRequest();
    ~DownloadRequest();

    DownloadRequest(const GURL& url,
                    const GURL& image_url,
                    chrome::IconType icon_type);

    GURL url;
    GURL image_url;
    chrome::IconType icon_type;
  };

  struct FaviconCandidate {
    FaviconCandidate();
    ~FaviconCandidate();

    FaviconCandidate(const GURL& url,
                     const GURL& image_url,
                     const gfx::Image& image,
                     float score,
                     chrome::IconType icon_type);

    GURL url;
    GURL image_url;
    gfx::Image image;
    float score;
    chrome::IconType icon_type;
  };

  
  void OnFaviconDataForInitialURL(
      const std::vector<chrome::FaviconBitmapResult>& favicon_bitmap_results);

  
  
  
  void DownloadFaviconOrAskHistory(const GURL& page_url,
                                   const GURL& icon_url,
                                   chrome::IconType icon_type);

  
  void OnFaviconData(
      const std::vector<chrome::FaviconBitmapResult>& favicon_bitmap_results);

  
  
  int ScheduleDownload(const GURL& url,
                       const GURL& image_url,
                       chrome::IconType icon_type);

  
  bool UpdateFaviconCandidate(const GURL& url,
                              const GURL& image_url,
                              const gfx::Image& image,
                              float score,
                              chrome::IconType icon_type);

  
  void SetFavicon(const GURL& url,
                  const GURL& icon_url,
                  const gfx::Image& image,
                  chrome::IconType icon_type);

  
  
  void UpdateFavicon(content::NavigationEntry* entry,
      const std::vector<chrome::FaviconBitmapResult>& favicon_bitmap_results);
  void UpdateFavicon(content::NavigationEntry* entry,
                     const GURL& icon_url,
                     const gfx::Image& image);

  
  content::FaviconURL* current_candidate() {
    return (image_urls_.size() > 0) ? &image_urls_[0] : NULL;
  }

  
  
  int preferred_icon_size() {
#if defined(OS_ANDROID)
    return 0;
#else
    return icon_types_ == chrome::FAVICON ? gfx::kFaviconSize : 0;
#endif
  }

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  GURL url_;

  
  bool got_favicon_from_history_;

  
  
  
  
  bool favicon_expired_or_incomplete_;

  
  typedef std::map<int, DownloadRequest> DownloadRequests;
  DownloadRequests download_requests_;

  
  const int icon_types_;

  
  std::deque<content::FaviconURL> image_urls_;

  
  std::vector<chrome::FaviconBitmapResult> history_results_;

  
  Profile* profile_;

  
  FaviconHandlerDelegate* delegate_;  

  
  FaviconCandidate favicon_candidate_;

  DISALLOW_COPY_AND_ASSIGN(FaviconHandler);
};

#endif  

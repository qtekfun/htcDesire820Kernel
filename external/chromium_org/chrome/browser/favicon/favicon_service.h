// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FAVICON_FAVICON_SERVICE_H_
#define CHROME_BROWSER_FAVICON_FAVICON_SERVICE_H_

#include <vector>

#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "chrome/common/favicon/favicon_types.h"
#include "chrome/common/ref_counted_util.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "ui/base/layout.h"

class GURL;
class HistoryService;
struct ImportedFaviconUsage;
class Profile;

namespace chrome {
struct FaviconImageResult;
}

class FaviconService : public CancelableRequestProvider,
                       public BrowserContextKeyedService {
 public:
  explicit FaviconService(Profile* profile);

  virtual ~FaviconService();

  
  struct FaviconForURLParams {
    FaviconForURLParams(const GURL& page_url,
                        int icon_types,
                        int desired_size_in_dip)
        : page_url(page_url),
          icon_types(icon_types),
          desired_size_in_dip(desired_size_in_dip) {}

    GURL page_url;
    int icon_types;
    int desired_size_in_dip;
  };

  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(const chrome::FaviconImageResult&)>
      FaviconImageCallback;

  
  
  
  typedef base::Callback<void(const chrome::FaviconBitmapResult&)>
      FaviconRawCallback;

  
  
  
  
  
  
  
  
  typedef base::Callback<void(const std::vector<chrome::FaviconBitmapResult>&)>
      FaviconResultsCallback;

  
  
  static void FaviconResultsCallbackRunner(
      const FaviconResultsCallback& callback,
      const std::vector<chrome::FaviconBitmapResult>* results);

  
  
  
  
  
  
  
  
  CancelableTaskTracker::TaskId GetFaviconImage(
      const GURL& icon_url,
      chrome::IconType icon_type,
      int desired_size_in_dip,
      const FaviconImageCallback& callback,
      CancelableTaskTracker* tracker);

  CancelableTaskTracker::TaskId GetRawFavicon(
      const GURL& icon_url,
      chrome::IconType icon_type,
      int desired_size_in_dip,
      ui::ScaleFactor desired_scale_factor,
      const FaviconRawCallback& callback,
      CancelableTaskTracker* tracker);

  CancelableTaskTracker::TaskId GetFavicon(
      const GURL& icon_url,
      chrome::IconType icon_type,
      int desired_size_in_dip,
      const FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  CancelableTaskTracker::TaskId UpdateFaviconMappingsAndFetch(
      const GURL& page_url,
      const std::vector<GURL>& icon_urls,
      int icon_types,
      int desired_size_in_dip,
      const FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  
  
  
  
  
  
  
  CancelableTaskTracker::TaskId GetFaviconImageForURL(
      const FaviconForURLParams& params,
      const FaviconImageCallback& callback,
      CancelableTaskTracker* tracker);

  CancelableTaskTracker::TaskId GetRawFaviconForURL(
      const FaviconForURLParams& params,
      ui::ScaleFactor desired_scale_factor,
      const FaviconRawCallback& callback,
      CancelableTaskTracker* tracker);

  
  CancelableTaskTracker::TaskId GetLargestRawFaviconForURL(
      Profile* profile,
      const GURL& page_url,
      const std::vector<int>& icon_types,
      int minimum_size_in_pixels,
      const FaviconRawCallback& callback,
      CancelableTaskTracker* tracker);

  CancelableTaskTracker::TaskId GetFaviconForURL(
      const FaviconForURLParams& params,
      const FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  CancelableTaskTracker::TaskId GetLargestRawFaviconForID(
      chrome::FaviconID favicon_id,
      const FaviconRawCallback& callback,
      CancelableTaskTracker* tracker);

  
  void SetFaviconOutOfDateForPage(const GURL& page_url);

  
  
  
  
  
  void CloneFavicon(const GURL& old_page_url, const GURL& new_page_url);

  
  
  // favicons will not be overwritten.
  void SetImportedFavicons(
      const std::vector<ImportedFaviconUsage>& favicon_usage);

  
  
  
  
  
  void MergeFavicon(const GURL& page_url,
                    const GURL& icon_url,
                    chrome::IconType icon_type,
                    scoped_refptr<base::RefCountedMemory> bitmap_data,
                    const gfx::Size& pixel_size);

  
  
  
  
  
  
  
  
  
  
  void SetFavicons(const GURL& page_url,
                   const GURL& icon_url,
                   chrome::IconType icon_type,
                   const gfx::Image& image);

  
  void UnableToDownloadFavicon(const GURL& icon_url);
  bool WasUnableToDownloadFavicon(const GURL& icon_url) const;
  void ClearUnableToDownloadFavicons();

 private:
  typedef uint32 MissingFaviconURLHash;
  base::hash_set<MissingFaviconURLHash> missing_favicon_urls_;
  HistoryService* history_service_;
  Profile* profile_;

  
  
  CancelableTaskTracker::TaskId GetFaviconForURLImpl(
      const FaviconForURLParams& params,
      const std::vector<ui::ScaleFactor>& desired_scale_factors,
      const FaviconResultsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  
  
  void RunFaviconImageCallbackWithBitmapResults(
      const FaviconImageCallback& callback,
      int desired_size_in_dip,
      const std::vector<chrome::FaviconBitmapResult>& favicon_bitmap_results);

  
  
  
  void RunFaviconRawCallbackWithBitmapResults(
      const FaviconRawCallback& callback,
      int desired_size_in_dip,
      ui::ScaleFactor desired_scale_factor,
      const std::vector<chrome::FaviconBitmapResult>& favicon_bitmap_results);

  DISALLOW_COPY_AND_ASSIGN(FaviconService);
};

#endif  

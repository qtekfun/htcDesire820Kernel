// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FAVICON_HELPER_H__
#define CHROME_BROWSER_FAVICON_HELPER_H__
#pragma once

#include <map>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/favicon_service.h"
#include "chrome/common/favicon_url.h"
#include "chrome/common/ref_counted_util.h"
#include "content/browser/cancelable_request.h"
#include "content/browser/tab_contents/tab_contents_observer.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/favicon_size.h"

class NavigationEntry;
class Profile;
class RefCountedMemory;
class SkBitmap;
class TabContents;


class FaviconHelper : public TabContentsObserver {
 public:
  enum Type {
    FAVICON,
    TOUCH,
  };

  FaviconHelper(TabContents* tab_contents, Type icon_type);
  virtual ~FaviconHelper();

  
  void FetchFavicon(const GURL& url);

  
  
  
  
  
  
  
  typedef Callback3<int, bool, const SkBitmap&>::Type ImageDownloadCallback;
  int DownloadImage(const GURL& image_url,
                    int image_size,
                    history::IconType icon_type,
                    ImageDownloadCallback* callback);

  
  
  void OnUpdateFaviconURL(int32 page_id,
                          const std::vector<FaviconURL>& candidates);

 protected:
  
  
  
  
  
  virtual NavigationEntry* GetEntry();

  
  virtual int DownloadFavicon(const GURL& image_url, int image_size);

  
  virtual void UpdateFaviconMappingAndFetch(
      const GURL& page_url,
      const GURL& icon_url,
      history::IconType icon_type,
      CancelableRequestConsumerBase* consumer,
      FaviconService::FaviconDataCallback* callback);

  virtual void GetFavicon(
      const GURL& icon_url,
      history::IconType icon_type,
      CancelableRequestConsumerBase* consumer,
      FaviconService::FaviconDataCallback* callback);

  virtual void GetFaviconForURL(
      const GURL& page_url,
      int icon_types,
      CancelableRequestConsumerBase* consumer,
      FaviconService::FaviconDataCallback* callback);

  virtual void SetHistoryFavicon(const GURL& page_url,
                                 const GURL& icon_url,
                                 const std::vector<unsigned char>& image_data,
                                 history::IconType icon_type);

  virtual FaviconService* GetFaviconService();

  
  virtual bool ShouldSaveFavicon(const GURL& url);

 private:
  friend class TestFaviconHelper; 

  struct DownloadRequest {
    DownloadRequest();

    DownloadRequest(const GURL& url,
                    const GURL& image_url,
                    ImageDownloadCallback* callback,
                    history::IconType icon_type);

    GURL url;
    GURL image_url;
    ImageDownloadCallback* callback;
    history::IconType icon_type;
  };

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void OnDidDownloadFavicon(int id,
                            const GURL& image_url,
                            bool errored,
                            const SkBitmap& image);

  
  void OnFaviconDataForInitialURL(FaviconService::Handle handle,
                                  history::FaviconData favicon);

  
  
  
  void DownloadFaviconOrAskHistory(const GURL& page_url,
                                   const GURL& icon_url,
                                   history::IconType icon_type);

  
  void OnFaviconData(FaviconService::Handle handle,
                     history::FaviconData favicon);

  
  
  int ScheduleDownload(const GURL& url,
                       const GURL& image_url,
                       int image_size,
                       history::IconType icon_type,
                       ImageDownloadCallback* callback);

  
  
  void SetFavicon(const GURL& url,
                  const GURL& icon_url,
                  const SkBitmap& image,
                  history::IconType icon_type);

  
  
  
  
  void UpdateFavicon(NavigationEntry* entry,
                     scoped_refptr<RefCountedMemory> data);
  void UpdateFavicon(NavigationEntry* entry, const SkBitmap& image);

  
  
  SkBitmap ConvertToFaviconSize(const SkBitmap& image);

  void FetchFaviconInternal();

  
  FaviconURL* current_candidate() {
    return (urls_.size() > current_url_index_) ?
        &urls_[current_url_index_] : NULL;
  }

  
  
  int preferred_icon_size() {
    return icon_types_ == history::FAVICON ? kFaviconSize : 0;
  }

  
  CancelableRequestConsumer cancelable_consumer_;

  
  GURL url_;

  
  
  bool got_favicon_from_history_;

  
  
  
  
  bool favicon_expired_;

  
  typedef std::map<int, DownloadRequest> DownloadRequests;
  DownloadRequests download_requests_;

  
  const int icon_types_;

  
  std::vector<FaviconURL> urls_;

  
  size_t current_url_index_;

  
  history::FaviconData history_icon_;

  DISALLOW_COPY_AND_ASSIGN(FaviconHelper);
};

#endif  

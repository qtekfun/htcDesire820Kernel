// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FAVICON_FAVICON_TAB_HELPER_H_
#define CHROME_BROWSER_FAVICON_FAVICON_TAB_HELPER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "chrome/browser/favicon/favicon_handler_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/common/favicon_url.h"

namespace gfx {
class Image;
}

class GURL;
class FaviconHandler;
class Profile;
class SkBitmap;

class FaviconTabHelper : public content::WebContentsObserver,
                         public FaviconHandlerDelegate,
                         public content::WebContentsUserData<FaviconTabHelper> {
 public:
  virtual ~FaviconTabHelper();

  
  void FetchFavicon(const GURL& url);

  
  
  
  
  gfx::Image GetFavicon() const;

  
  bool FaviconIsValid() const;

  
  
  virtual bool ShouldDisplayFavicon();

  
  
  void set_should_fetch_icons(bool fetch) {
    should_fetch_icons_ = fetch;
  }

  
  
  virtual void DidUpdateFaviconURL(
      int32 page_id,
      const std::vector<content::FaviconURL>& candidates) OVERRIDE;

  
  void SaveFavicon();

  
  virtual content::NavigationEntry* GetActiveEntry() OVERRIDE;
  virtual int StartDownload(const GURL& url, int max_bitmap_size) OVERRIDE;
  virtual void NotifyFaviconUpdated(bool icon_url_changed) OVERRIDE;

  
  void DidDownloadFavicon(
      int id,
      int http_status_code,
      const GURL& image_url,
      const std::vector<SkBitmap>& bitmaps,
      const std::vector<gfx::Size>& original_bitmap_sizes);

 private:
  explicit FaviconTabHelper(content::WebContents* web_contents);
  friend class content::WebContentsUserData<FaviconTabHelper>;

  
  virtual void DidStartNavigationToPendingEntry(
      const GURL& url,
      content::NavigationController::ReloadType reload_type) OVERRIDE;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

  Profile* profile_;
  bool should_fetch_icons_;

  scoped_ptr<FaviconHandler> favicon_handler_;

  
  
  scoped_ptr<FaviconHandler> touch_icon_handler_;

  DISALLOW_COPY_AND_ASSIGN(FaviconTabHelper);
};

#endif  

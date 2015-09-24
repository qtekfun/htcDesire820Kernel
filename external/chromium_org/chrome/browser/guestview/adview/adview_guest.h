// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GUESTVIEW_ADVIEW_ADVIEW_GUEST_H_
#define CHROME_BROWSER_GUESTVIEW_ADVIEW_ADVIEW_GUEST_H_

#include "base/values.h"
#include "chrome/browser/guestview/guestview.h"
#include "content/public/browser/web_contents_observer.h"

class AdViewGuest : public GuestView,
                    public content::WebContentsObserver {
 public:
  AdViewGuest(content::WebContents* guest_web_contents,
              const std::string& extension_id);

  static AdViewGuest* From(int embedder_process_id, int instance_id);

  
  virtual GuestView::Type GetViewType() const OVERRIDE;
  virtual WebViewGuest* AsWebView() OVERRIDE;
  virtual AdViewGuest* AsAdView() OVERRIDE;

 private:
  virtual ~AdViewGuest();

  virtual void DidCommitProvisionalLoadForFrame(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& url,
      content::PageTransition transition_type,
      content::RenderViewHost* render_view_host) OVERRIDE;
  virtual void DidFailProvisionalLoad(
      int64 frame_id,
      const base::string16& frame_unique_name,
      bool is_main_frame,
      const GURL& validated_url,
      int error_code,
      const base::string16& error_description,
      content::RenderViewHost* render_view_host) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(AdViewGuest);
};

#endif  

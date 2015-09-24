// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_WEBAPPS_SINGLE_TAB_MODE_TAB_HELPER_H_
#define CHROME_BROWSER_ANDROID_WEBAPPS_SINGLE_TAB_MODE_TAB_HELPER_H_

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/blocked_content/blocked_window_params.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class RenderViewHost;
class WebContents;
}  

class GURL;

class SingleTabModeTabHelper
    : public content::WebContentsObserver,
      public content::WebContentsUserData<SingleTabModeTabHelper> {
 public:
  
  static bool IsRegistered(int32 process_id, int32 routing_id);

  virtual ~SingleTabModeTabHelper();

  
  void HandleOpenUrl(const BlockedWindowParams& params);

  
  
  void PermanentlyBlockAllNewWindows();

  
  virtual void RenderViewCreated(content::RenderViewHost* render_view_host)
      OVERRIDE;
  virtual void RenderViewDeleted(content::RenderViewHost* render_view_host)
      OVERRIDE;

 private:
  explicit SingleTabModeTabHelper(content::WebContents* web_contents);
  friend class content::WebContentsUserData<SingleTabModeTabHelper>;

  bool block_all_new_windows_;

  DISALLOW_COPY_AND_ASSIGN(SingleTabModeTabHelper);
};

#endif  

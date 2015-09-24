// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ZOOM_ZOOM_CONTROLLER_H_
#define CHROME_BROWSER_UI_ZOOM_ZOOM_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/prefs/pref_member.h"
#include "content/public/browser/host_zoom_map.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

class ZoomObserver;

namespace content {
class WebContents;
}

class ZoomController : public content::WebContentsObserver,
                       public content::WebContentsUserData<ZoomController> {
 public:
  virtual ~ZoomController();

  int zoom_percent() const { return zoom_percent_; }

  
  bool IsAtDefaultZoom() const;

  
  int GetResourceForZoomLevel() const;

  void set_observer(ZoomObserver* observer) { observer_ = observer; }

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

 private:
  explicit ZoomController(content::WebContents* web_contents);
  friend class content::WebContentsUserData<ZoomController>;
  friend class ZoomControllerTest;

  void OnZoomLevelChanged(const content::HostZoomMap::ZoomLevelChange& change);

  
  
  
  
  void UpdateState(const std::string& host);

  
  int zoom_percent_;

  
  DoublePrefMember default_zoom_level_;

  
  ZoomObserver* observer_;

  content::BrowserContext* browser_context_;

  scoped_ptr<content::HostZoomMap::Subscription> zoom_subscription_;

  DISALLOW_COPY_AND_ASSIGN(ZoomController);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_TAB_HELPER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_TAB_HELPER_H_
#pragma once

#include "content/browser/tab_contents/tab_contents_observer.h"
#include "chrome/browser/extensions/image_loading_tracker.h"
#include "chrome/common/web_apps.h"
#include "third_party/skia/include/core/SkBitmap.h"

class Extension;
class TabContentsWrapper;
struct WebApplicationInfo;

class ExtensionTabHelper : public TabContentsObserver,
                           public ImageLoadingTracker::Observer {
 public:
  explicit ExtensionTabHelper(TabContentsWrapper* wrapper);
  virtual ~ExtensionTabHelper();

  
  void CopyStateFrom(const ExtensionTabHelper& source);

  
  void PageActionStateChanged();

  
  
  
  void GetApplicationInfo(int32 page_id);

  

  
  
  
  
  
  
  
  void SetExtensionApp(const Extension* extension);

  
  
  
  void SetExtensionAppById(const std::string& extension_app_id);

  const Extension* extension_app() const { return extension_app_; }
  bool is_app() const { return extension_app_ != NULL; }
  const WebApplicationInfo& web_app_info() const {
    return web_app_info_;
  }

  
  
  
  
  
  SkBitmap* GetExtensionAppIcon();

  TabContents* tab_contents() const {
      return TabContentsObserver::tab_contents();
  }

  
  
  void SetAppIcon(const SkBitmap& app_icon);

 private:
  
  virtual void DidNavigateMainFramePostCommit(
      const NavigationController::LoadCommittedDetails& details,
      const ViewHostMsg_FrameNavigate_Params& params) OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  void OnDidGetApplicationInfo(int32 page_id, const WebApplicationInfo& info);
  void OnInstallApplication(const WebApplicationInfo& info);

  

  
  
  void UpdateExtensionAppIcon(const Extension* extension);

  
  virtual void OnImageLoaded(SkBitmap* image, const ExtensionResource& resource,
                             int index);

  

  
  
  const Extension* extension_app_;

  
  
  SkBitmap extension_app_icon_;

  
  scoped_ptr<ImageLoadingTracker> extension_app_image_loader_;

  
  WebApplicationInfo web_app_info_;

  TabContentsWrapper* wrapper_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionTabHelper);
};

#endif  

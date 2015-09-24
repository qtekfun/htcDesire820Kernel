// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_RENDER_VIEW_HOST_DELEGATE_HELPER_H_
#define CHROME_BROWSER_TAB_CONTENTS_RENDER_VIEW_HOST_DELEGATE_HELPER_H_
#pragma once

#include <map>

#include "base/basictypes.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/window_container_type.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebPopupType.h"
#include "ui/gfx/rect.h"
#include "webkit/glue/webpreferences.h"
#include "webkit/glue/window_open_disposition.h"

class BackgroundContents;
class Profile;
class RenderProcessHost;
class RenderViewHost;
class RenderViewHostDelegate;
class RenderWidgetHost;
class RenderWidgetHostView;
class SiteInstance;
class TabContents;

class RenderViewHostDelegateViewHelper {
 public:
  RenderViewHostDelegateViewHelper();
  virtual ~RenderViewHostDelegateViewHelper();

  
  
  
  
  virtual TabContents* CreateNewWindow(
      int route_id,
      Profile* profile,
      SiteInstance* site,
      WebUI::TypeID webui_type,
      RenderViewHostDelegate* opener,
      WindowContainerType window_container_type,
      const string16& frame_name);

  
  
  virtual RenderWidgetHostView* CreateNewWidget(int route_id,
                                                WebKit::WebPopupType popup_type,
                                                RenderProcessHost* process);

  virtual RenderWidgetHostView* CreateNewFullscreenWidget(
      int route_id, RenderProcessHost* process);

  
  
  virtual RenderWidgetHostView* GetCreatedWidget(int route_id);

  
  
  
  
  
  virtual TabContents* GetCreatedWindow(int route_id);

  
  void RenderWidgetHostDestroyed(RenderWidgetHost* host);

 private:
  BackgroundContents* MaybeCreateBackgroundContents(
      int route_id,
      Profile* profile,
      SiteInstance* site,
      const GURL& opener_url,
      const string16& frame_name);

  
  
  typedef std::map<int, RenderViewHost*> PendingContents;
  PendingContents pending_contents_;

  
  
  typedef std::map<int, RenderWidgetHostView*> PendingWidgetViews;
  PendingWidgetViews pending_widget_views_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewHostDelegateViewHelper);
};


class RenderViewHostDelegateHelper {
 public:
  static WebPreferences GetWebkitPrefs(Profile* profile, bool is_web_ui);

  static void UpdateInspectorSetting(Profile* profile,
                                     const std::string& key,
                                     const std::string& value);
  static void ClearInspectorSettings(Profile* profile);

  static bool gpu_enabled() { return gpu_enabled_; }
  static void set_gpu_enabled(bool enabled) { gpu_enabled_ = enabled; }

 private:
  RenderViewHostDelegateHelper();

  
  
  
  static bool gpu_enabled_;

  DISALLOW_COPY_AND_ASSIGN(RenderViewHostDelegateHelper);
};

#endif  

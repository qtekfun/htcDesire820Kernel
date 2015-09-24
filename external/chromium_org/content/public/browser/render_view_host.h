// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RENDER_VIEW_HOST_H_
#define CONTENT_PUBLIC_BROWSER_RENDER_VIEW_HOST_H_

#include <list>

#include "base/callback_forward.h"
#include "content/common/content_export.h"
#include "content/public/browser/render_widget_host.h"
#include "content/public/common/file_chooser_params.h"
#include "content/public/common/page_zoom.h"
#include "content/public/common/stop_find_action.h"
#include "third_party/WebKit/public/web/WebDragOperation.h"

class GURL;
struct WebPreferences;

namespace gfx {
class Point;
}

namespace base {
class FilePath;
class Value;
}

namespace media {
class AudioOutputController;
}

namespace ui {
struct SelectedFileInfo;
}

namespace blink {
struct WebFindOptions;
struct WebMediaPlayerAction;
struct WebPluginAction;
}

namespace content {

class ChildProcessSecurityPolicy;
class RenderProcessHost;
class RenderViewHostDelegate;
class SessionStorageNamespace;
class SiteInstance;
struct CustomContextMenuContext;
struct DropData;

class CONTENT_EXPORT RenderViewHost : virtual public RenderWidgetHost {
 public:
  
  
  static RenderViewHost* FromID(int render_process_id, int render_view_id);

  
  
  static RenderViewHost* From(RenderWidgetHost* rwh);

  
  
  
  static void FilterURL(const RenderProcessHost* process,
                        bool empty_allowed,
                        GURL* url);

  virtual ~RenderViewHost() {}

  
  
  virtual void AllowBindings(int binding_flags) = 0;

  
  virtual void ClearFocusedNode() = 0;

  
  
  
  virtual void ClosePage() = 0;

  
  
  virtual void CopyImageAt(int x, int y) = 0;

  
  virtual void DesktopNotificationPermissionRequestDone(
      int callback_context) = 0;
  virtual void DesktopNotificationPostDisplay(int callback_context) = 0;
  virtual void DesktopNotificationPostError(int notification_id,
                                    const base::string16& message) = 0;
  virtual void DesktopNotificationPostClose(int notification_id,
                                            bool by_user) = 0;
  virtual void DesktopNotificationPostClick(int notification_id) = 0;

  
  virtual void DirectoryEnumerationFinished(
      int request_id,
      const std::vector<base::FilePath>& files) = 0;

  
  
  virtual void DisableScrollbarsForThreshold(const gfx::Size& size) = 0;

  
  
  virtual void DragSourceEndedAt(
      int client_x, int client_y, int screen_x, int screen_y,
      blink::WebDragOperation operation) = 0;

  
  
  virtual void DragSourceMovedTo(
      int client_x, int client_y, int screen_x, int screen_y) = 0;

  
  
  virtual void DragSourceSystemDragEnded() = 0;

  
  virtual void DragTargetDragEnter(
      const DropData& drop_data,
      const gfx::Point& client_pt,
      const gfx::Point& screen_pt,
      blink::WebDragOperationsMask operations_allowed,
      int key_modifiers) = 0;
  virtual void DragTargetDragOver(
      const gfx::Point& client_pt,
      const gfx::Point& screen_pt,
      blink::WebDragOperationsMask operations_allowed,
      int key_modifiers) = 0;
  virtual void DragTargetDragLeave() = 0;
  virtual void DragTargetDrop(const gfx::Point& client_pt,
                              const gfx::Point& screen_pt,
                              int key_modifiers) = 0;

  
  
  virtual void EnableAutoResize(const gfx::Size& min_size,
                                const gfx::Size& max_size) = 0;

  
  virtual void DisableAutoResize(const gfx::Size& new_size) = 0;

  
  virtual void EnablePreferredSizeMode() = 0;

  
  virtual void ExecuteCustomContextMenuCommand(
      int action, const CustomContextMenuContext& context) = 0;

  
  
  virtual void ExecuteMediaPlayerActionAtLocation(
      const gfx::Point& location,
      const blink::WebMediaPlayerAction& action) = 0;

  
  virtual void ExecuteJavascriptInWebFrame(const base::string16& frame_xpath,
                                           const base::string16& jscript) = 0;

  
  
  typedef base::Callback<void(const base::Value*)> JavascriptResultCallback;
  virtual void ExecuteJavascriptInWebFrameCallbackResult(
      const base::string16& frame_xpath,
      const base::string16& jscript,
      const JavascriptResultCallback& callback) = 0;

  
  
  virtual void ExecutePluginActionAtLocation(
      const gfx::Point& location, const blink::WebPluginAction& action) = 0;

  
  virtual void ExitFullscreen() = 0;

  
  virtual void Find(int request_id, const base::string16& search_text,
                    const blink::WebFindOptions& options) = 0;

  
  
  virtual void StopFinding(StopFindAction action) = 0;

  
  
  
  
  
  
  
  virtual void FirePageBeforeUnload(bool for_cross_site_transition) = 0;

  
  
  
  virtual void FilesSelectedInChooser(
      const std::vector<ui::SelectedFileInfo>& files,
      FileChooserParams::Mode permissions) = 0;

  virtual RenderViewHostDelegate* GetDelegate() const = 0;

  
  
  virtual int GetEnabledBindings() const = 0;

  virtual SiteInstance* GetSiteInstance() const = 0;

  
  
  virtual void InsertCSS(const base::string16& frame_xpath,
                         const std::string& css) = 0;

  
  
  virtual bool IsRenderViewLive() const = 0;

  
  
  virtual bool IsSubframe() const = 0;

  
  virtual void NotifyContextMenuClosed(
      const CustomContextMenuContext& context) = 0;

  
  
  virtual void NotifyMoveOrResizeStarted() = 0;

  
  virtual void ReloadFrame() = 0;

  
  virtual void SetAltErrorPageURL(const GURL& url) = 0;

  
  
  virtual void SetWebUIProperty(const std::string& name,
                                const std::string& value) = 0;

  
  virtual void Zoom(PageZoom zoom) = 0;

  
  
  virtual void SyncRendererPrefs() = 0;

  virtual void ToggleSpeechInput() = 0;

  
  virtual WebPreferences GetWebkitPreferences() = 0;

  
  virtual void UpdateWebkitPreferences(const WebPreferences& prefs) = 0;

  
  virtual void NotifyTimezoneChange() = 0;

  
  
  
  typedef std::list<scoped_refptr<media::AudioOutputController> >
      AudioOutputControllerList;
  typedef base::Callback<void(const AudioOutputControllerList&)>
      GetAudioOutputControllersCallback;
  virtual void GetAudioOutputControllers(
      const GetAudioOutputControllersCallback& callback) const = 0;

#if defined(OS_ANDROID)
  
  
  virtual void ActivateNearestFindResult(int request_id, float x, float y) = 0;

  
  virtual void RequestFindMatchRects(int current_version) = 0;

  
  virtual void DisableFullscreenEncryptedMediaPlayback() = 0;
#endif

 private:
  
  friend class RenderViewHostImpl;
  RenderViewHost() {}
};

}  

#endif  

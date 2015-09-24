// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_H_
#define CONTENT_PUBLIC_BROWSER_WEB_CONTENTS_H_

#include <set>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/process/kill.h"
#include "base/strings/string16.h"
#include "base/supports_user_data.h"
#include "content/common/content_export.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/save_page_type.h"
#include "content/public/browser/web_ui.h"
#include "ipc/ipc_sender.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/window_open_disposition.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

#if defined(OS_ANDROID)
#include "base/android/scoped_java_ref.h"
#endif

namespace base {
class TimeTicks;
}

namespace gfx {
class Rect;
class Size;
}

namespace net {
struct LoadStateWithParam;
}

namespace content {

class BrowserContext;
class InterstitialPage;
class PageState;
class RenderFrameHost;
class RenderProcessHost;
class RenderViewHost;
class RenderWidgetHostView;
class SiteInstance;
class WebContentsDelegate;
class WebContentsView;
struct RendererPreferences;

class WebContents : public PageNavigator,
                    public IPC::Sender,
                    public base::SupportsUserData {
 public:
  struct CONTENT_EXPORT CreateParams {
    explicit CreateParams(BrowserContext* context);
    CreateParams(BrowserContext* context, SiteInstance* site);

    BrowserContext* browser_context;

    
    
    
    SiteInstance* site_instance;

    WebContents* opener;
    int routing_id;
    int main_frame_routing_id;

    
    gfx::Size initial_size;

    
    bool initially_hidden;

    
    
    gfx::NativeView context;
  };

  
  CONTENT_EXPORT static WebContents* Create(const CreateParams& params);

  
  
  
  
  
  
  
  
  
  
  CONTENT_EXPORT static WebContents* CreateWithSessionStorage(
      const CreateParams& params,
      const SessionStorageNamespaceMap& session_storage_namespace_map);

  
  
  CONTENT_EXPORT static WebContents* FromRenderViewHost(
      const RenderViewHost* rvh);

  virtual ~WebContents() {}

  

  
  virtual WebContentsDelegate* GetDelegate() = 0;
  virtual void SetDelegate(WebContentsDelegate* delegate) = 0;

  
  virtual NavigationController& GetController() = 0;
  virtual const NavigationController& GetController() const = 0;

  
  
  virtual content::BrowserContext* GetBrowserContext() const = 0;

  
  
  
  virtual const GURL& GetURL() const = 0;

  
  
  
  
  
  virtual const GURL& GetVisibleURL() const = 0;

  
  
  
  virtual const GURL& GetLastCommittedURL() const = 0;

  
  
  virtual RenderProcessHost* GetRenderProcessHost() const = 0;

  
  virtual RenderFrameHost* GetMainFrame() = 0;

  
  virtual RenderViewHost* GetRenderViewHost() const = 0;

  typedef base::Callback<void(RenderViewHost* ,
                              int ,
                              int )> GetRenderViewHostCallback;
  
  
  
  
  virtual void GetRenderViewHostAtPosition(
      int x,
      int y,
      const GetRenderViewHostCallback& callback) = 0;

  
  
  virtual WebContents* GetEmbedderWebContents() const = 0;

  
  
  
  virtual int GetEmbeddedInstanceID() const = 0;

  
  
  virtual int GetRoutingID() const = 0;

  
  
  virtual RenderWidgetHostView* GetRenderWidgetHostView() const = 0;

  
  
  virtual RenderWidgetHostView* GetFullscreenRenderWidgetHostView() const = 0;

  
  virtual WebContentsView* GetView() const = 0;

  
  
  
  
  virtual WebUI* CreateWebUI(const GURL& url) = 0;

  
  virtual WebUI* GetWebUI() const = 0;
  virtual WebUI* GetCommittedWebUI() const = 0;

  
  virtual void SetUserAgentOverride(const std::string& override) = 0;
  virtual const std::string& GetUserAgentOverride() const = 0;

#if defined(OS_WIN) && defined(USE_AURA)
  virtual void SetParentNativeViewAccessible(
      gfx::NativeViewAccessible accessible_parent) = 0;
#endif

  

  
  
  
  virtual const base::string16& GetTitle() const = 0;

  
  
  
  
  virtual int32 GetMaxPageID() = 0;

  
  
  virtual int32 GetMaxPageIDForSiteInstance(SiteInstance* site_instance) = 0;

  
  virtual SiteInstance* GetSiteInstance() const = 0;

  
  
  virtual SiteInstance* GetPendingSiteInstance() const = 0;

  
  virtual bool IsLoading() const = 0;

  
  
  virtual bool IsWaitingForResponse() const = 0;

  
  virtual const net::LoadStateWithParam& GetLoadState() const = 0;
  virtual const base::string16& GetLoadStateHost() const = 0;

  
  virtual uint64 GetUploadSize() const = 0;
  virtual uint64 GetUploadPosition() const = 0;

  
  virtual std::set<GURL> GetSitesInTab() const = 0;

  
  virtual const std::string& GetEncoding() const = 0;

  
  virtual bool DisplayedInsecureContent() const = 0;

  

  
  
  
  virtual void IncrementCapturerCount() = 0;
  virtual void DecrementCapturerCount() = 0;
  virtual int GetCapturerCount() const = 0;

  
  
  virtual bool IsCrashed() const  = 0;
  virtual void SetIsCrashed(base::TerminationStatus status, int error_code) = 0;

  virtual base::TerminationStatus GetCrashedStatus() const = 0;

  
  virtual bool IsBeingDestroyed() const = 0;

  
  
  virtual void NotifyNavigationStateChanged(unsigned changed_flags) = 0;

  
  virtual base::TimeTicks GetLastSelectedTime() const = 0;

  
  virtual void WasShown() = 0;
  virtual void WasHidden() = 0;

  
  
  
  
  virtual bool NeedToFireBeforeUnload() = 0;

  

  
  virtual void Stop() = 0;

  
  
  virtual WebContents* Clone() = 0;

  
  
  
  
  virtual void FocusThroughTabTraversal(bool reverse) = 0;

  

  
  virtual bool ShowingInterstitialPage() const = 0;

  
  
  virtual InterstitialPage* GetInterstitialPage() const = 0;

  

  
  
  virtual bool IsSavable() = 0;

  
  virtual void OnSavePage() = 0;

  
  
  
  virtual bool SavePage(const base::FilePath& main_file,
                        const base::FilePath& dir_path,
                        SavePageType save_type) = 0;

  
  virtual void SaveFrame(const GURL& url,
                         const Referrer& referrer) = 0;

  
  virtual void GenerateMHTML(
      const base::FilePath& file,
      const base::Callback<void(
          int64 )>& callback) = 0;

  
  virtual bool IsActiveEntry(int32 page_id) = 0;

  
  virtual const std::string& GetContentsMimeType() const = 0;

  
  virtual bool WillNotifyDisconnection() const = 0;

  
  
  
  
  
  virtual void SetOverrideEncoding(const std::string& encoding) = 0;

  
  
  virtual void ResetOverrideEncoding() = 0;

  
  virtual content::RendererPreferences* GetMutableRendererPrefs() = 0;

  
  
  virtual void Close() = 0;

  
  
  virtual void SystemDragEnded() = 0;

  
  
  
  virtual void UserGestureDone() = 0;

  
  
  
  
  virtual void SetClosedByUserGesture(bool value) = 0;
  virtual bool GetClosedByUserGesture() const = 0;

  
  virtual double GetZoomLevel() const = 0;

  
  virtual int GetZoomPercent(bool* enable_increment,
                             bool* enable_decrement) const = 0;

  
  virtual void ViewSource() = 0;

  virtual void ViewFrameSource(const GURL& url,
                               const PageState& page_state)= 0;

  
  virtual int GetMinimumZoomPercent() const = 0;
  virtual int GetMaximumZoomPercent() const = 0;

  
  virtual gfx::Size GetPreferredSize() const = 0;

  
  
  
  virtual bool GotResponseToLockMouseRequest(bool allowed) = 0;

  
  virtual void DidChooseColorInColorChooser(SkColor color) = 0;

  
  virtual void DidEndColorChooser() = 0;

  
  
  
  virtual bool FocusLocationBarByDefault() = 0;

  
  virtual bool HasOpener() const = 0;

  typedef base::Callback<void(
      int, 
      int, 
      const GURL&, 
      const std::vector<SkBitmap>&, 
      const std::vector<gfx::Size>&)>
          ImageDownloadCallback;

  
  
  
  
  
  
  
  
  virtual int DownloadImage(const GURL& url,
                            bool is_favicon,
                            uint32_t max_bitmap_size,
                            const ImageDownloadCallback& callback) = 0;

  
  
  virtual void SetZoomLevel(double level) = 0;

#if defined(OS_ANDROID)
  CONTENT_EXPORT static WebContents* FromJavaWebContents(
      jobject jweb_contents_android);
  virtual base::android::ScopedJavaLocalRef<jobject> GetJavaWebContents() = 0;
#endif  

 private:
  
  friend class WebContentsImpl;
  WebContents() {}
};

}  

#endif  

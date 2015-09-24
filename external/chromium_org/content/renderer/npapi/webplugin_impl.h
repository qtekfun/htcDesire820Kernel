// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_NPAPI_WEBPLUGIN_IMPL_H_
#define CONTENT_RENDERER_NPAPI_WEBPLUGIN_IMPL_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/child/npapi/webplugin.h"
#include "content/common/content_export.h"
#include "content/common/webplugin_geometry.h"
#include "third_party/WebKit/public/platform/WebRect.h"
#include "third_party/WebKit/public/platform/WebString.h"
#include "third_party/WebKit/public/platform/WebURLLoaderClient.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "third_party/WebKit/public/platform/WebVector.h"
#include "third_party/WebKit/public/web/WebPlugin.h"
#include "ui/gfx/native_widget_types.h"
#include "url/gurl.h"

namespace cc {
class IOSurfaceLayer;
}

namespace blink {
class WebFrame;
class WebLayer;
class WebPluginContainer;
class WebURLResponse;
class WebURLLoader;
class WebURLRequest;
}

namespace webkit_glue {
class MultipartResponseDelegate;
}  

namespace content {
class RenderFrameImpl;
class RenderViewImpl;
class WebPluginDelegate;

class WebPluginImpl : public WebPlugin,
                      public blink::WebPlugin,
                      public blink::WebURLLoaderClient {
 public:
  WebPluginImpl(
      blink::WebFrame* frame,
      const blink::WebPluginParams& params,
      const base::FilePath& file_path,
      const base::WeakPtr<RenderViewImpl>& render_view,
      RenderFrameImpl* render_frame);
  virtual ~WebPluginImpl();

  
  CONTENT_EXPORT static bool SetPostData(blink::WebURLRequest* request,
                                         const char* buf,
                                         uint32 length);

  blink::WebFrame* webframe() { return webframe_; }
  WebPluginDelegate* delegate() { return delegate_; }

  
  virtual bool initialize(
      blink::WebPluginContainer* container);
  virtual void destroy();
  virtual NPObject* scriptableObject();
  virtual struct _NPP* pluginNPP();
  virtual bool getFormValue(blink::WebString& value);
  virtual void paint(
      blink::WebCanvas* canvas, const blink::WebRect& paint_rect);
  virtual void updateGeometry(
      const blink::WebRect& frame_rect, const blink::WebRect& clip_rect,
      const blink::WebVector<blink::WebRect>& cut_outs, bool is_visible);
  virtual void updateFocus(bool focused);
  virtual void updateVisibility(bool visible);
  virtual bool acceptsInputEvents();
  virtual bool handleInputEvent(
      const blink::WebInputEvent& event, blink::WebCursorInfo& cursor_info);
  virtual void didReceiveResponse(const blink::WebURLResponse& response);
  virtual void didReceiveData(const char* data, int data_length);
  virtual void didFinishLoading();
  virtual void didFailLoading(const blink::WebURLError& error);
  virtual void didFinishLoadingFrameRequest(
      const blink::WebURL& url, void* notify_data);
  virtual void didFailLoadingFrameRequest(
      const blink::WebURL& url, void* notify_data,
      const blink::WebURLError& error);
  virtual bool isPlaceholder() OVERRIDE;

  
  virtual void SetWindow(gfx::PluginWindowHandle window) OVERRIDE;
  virtual void SetAcceptsInputEvents(bool accepts) OVERRIDE;
  virtual void WillDestroyWindow(gfx::PluginWindowHandle window) OVERRIDE;
  virtual void CancelResource(unsigned long id) OVERRIDE;
  virtual void Invalidate() OVERRIDE;
  virtual void InvalidateRect(const gfx::Rect& rect) OVERRIDE;
  virtual NPObject* GetWindowScriptNPObject() OVERRIDE;
  virtual NPObject* GetPluginElement() OVERRIDE;
  virtual bool FindProxyForUrl(const GURL& url,
                               std::string* proxy_list) OVERRIDE;
  virtual void SetCookie(const GURL& url,
                         const GURL& first_party_for_cookies,
                         const std::string& cookie) OVERRIDE;
  virtual std::string GetCookies(const GURL& url,
                                 const GURL& first_party_for_cookies) OVERRIDE;
  virtual void HandleURLRequest(const char* url,
                                const char *method,
                                const char* target,
                                const char* buf,
                                unsigned int len,
                                int notify_id,
                                bool popups_allowed,
                                bool notify_redirects) OVERRIDE;
  virtual void CancelDocumentLoad() OVERRIDE;
  virtual void InitiateHTTPRangeRequest(const char* url,
                                        const char* range_info,
                                        int pending_request_id) OVERRIDE;
  virtual void DidStartLoading() OVERRIDE;
  virtual void DidStopLoading() OVERRIDE;
  virtual bool IsOffTheRecord() OVERRIDE;
  virtual void SetDeferResourceLoading(unsigned long resource_id,
                                       bool defer) OVERRIDE;
  virtual void URLRedirectResponse(bool allow, int resource_id) OVERRIDE;
  virtual bool CheckIfRunInsecureContent(const GURL& url) OVERRIDE;
#if defined(OS_WIN)
  void SetWindowlessData(HANDLE pump_messages_event,
                         gfx::NativeViewId dummy_activation_window) { }
  void ReparentPluginWindow(HWND window, HWND parent) { }
  void ReportExecutableMemory(size_t size) { }
#endif
#if defined(OS_MACOSX)
  virtual WebPluginAcceleratedSurface* GetAcceleratedSurface(
      gfx::GpuPreference gpu_preference) OVERRIDE;
  virtual void AcceleratedPluginEnabledRendering() OVERRIDE;
  virtual void AcceleratedPluginAllocatedIOSurface(int32 width,
                                                   int32 height,
                                                   uint32 surface_id) OVERRIDE;
  virtual void AcceleratedPluginSwappedIOSurface() OVERRIDE;
#endif

 private:
  
  GURL CompleteURL(const char* url);

  enum RoutingStatus {
    ROUTED,
    NOT_ROUTED,
    INVALID_URL,
    GENERAL_FAILURE
  };

  
  
  enum Referrer {
    PLUGIN_SRC,
    DOCUMENT_URL,
    NO_REFERRER
  };

  
  
  
  RoutingStatus RouteToFrame(const char* url,
                             bool is_javascript_url,
                             bool popups_allowed,
                             const char* method,
                             const char* target,
                             const char* buf,
                             unsigned int len,
                             int notify_id,
                             Referrer referrer_flag);

  
  
  unsigned long GetNextResourceId();

  
  
  bool InitiateHTTPRequest(unsigned long resource_id,
                           WebPluginResourceClient* client,
                           const GURL& url,
                           const char* method,
                           const char* buf,
                           int len,
                           const char* range_info,
                           Referrer referrer_flag,
                           bool notify_redirects,
                           bool check_mixed_scripting);

  gfx::Rect GetWindowClipRect(const gfx::Rect& rect);

  
  void SetContainer(blink::WebPluginContainer* container);

  
  
  
  void TearDownPluginInstance(blink::WebURLLoader* loader_to_ignore);

  
  
  
  virtual void willSendRequest(blink::WebURLLoader* loader,
                               blink::WebURLRequest& request,
                               const blink::WebURLResponse& response);
  virtual void didSendData(blink::WebURLLoader* loader,
                           unsigned long long bytes_sent,
                           unsigned long long total_bytes_to_be_sent);
  virtual void didReceiveResponse(blink::WebURLLoader* loader,
                                  const blink::WebURLResponse& response);

  virtual void didReceiveData(blink::WebURLLoader* loader, const char *buffer,
                              int data_length, int encoded_data_length);
  virtual void didFinishLoading(blink::WebURLLoader* loader,
                                double finishTime);
  virtual void didFail(blink::WebURLLoader* loader,
                       const blink::WebURLError& error);

  
  
  void RemoveClient(size_t i);

  
  
  void RemoveClient(blink::WebURLLoader* loader);

  
  
  
  
  bool HandleHttpMultipartResponse(const blink::WebURLResponse& response,
                                   WebPluginResourceClient* client);

  void HandleURLRequestInternal(const char* url,
                                const char* method,
                                const char* target,
                                const char* buf,
                                unsigned int len,
                                int notify_id,
                                bool popups_allowed,
                                Referrer referrer_flag,
                                bool notify_redirects,
                                bool check_mixed_scripting);

  
  
  bool ReinitializePluginForResponse(blink::WebURLLoader* loader);

  
  void OnDownloadPluginSrcUrl();

  struct ClientInfo;

  
  WebPluginResourceClient* GetClientFromLoader(blink::WebURLLoader* loader);
  ClientInfo* GetClientInfoFromLoader(blink::WebURLLoader* loader);

  
  void SetReferrer(blink::WebURLRequest* request, Referrer referrer_flag);

  
  bool IsValidUrl(const GURL& url, Referrer referrer_flag);

  WebPluginDelegate* CreatePluginDelegate();

  std::vector<ClientInfo> clients_;

  bool windowless_;
  gfx::PluginWindowHandle window_;
#if defined(OS_MACOSX)
  bool next_io_surface_allocated_;
  int32 next_io_surface_width_;
  int32 next_io_surface_height_;
  uint32 next_io_surface_id_;
  scoped_refptr<cc::IOSurfaceLayer> io_surface_layer_;
  scoped_ptr<blink::WebLayer> web_layer_;
#endif
  bool accepts_input_events_;
  RenderFrameImpl* render_frame_;
  base::WeakPtr<RenderViewImpl> render_view_;
  blink::WebFrame* webframe_;

  WebPluginDelegate* delegate_;

  
  blink::WebPluginContainer* container_;

  
  struct _NPP* npp_;

  typedef std::map<WebPluginResourceClient*,
                   webkit_glue::MultipartResponseDelegate*>
      MultiPartResponseHandlerMap;
  
  
  MultiPartResponseHandlerMap multi_part_response_map_;

  
  GURL plugin_url_;

  
  bool load_manually_;

  
  bool first_geometry_update_;

  
  bool ignore_response_error_;

  
  WebPluginGeometry geometry_;

  
  base::FilePath file_path_;

  
  std::string mime_type_;

  
  
  std::vector<std::string> arg_names_;
  std::vector<std::string> arg_values_;

  base::WeakPtrFactory<WebPluginImpl> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(WebPluginImpl);
};

}  

#endif  

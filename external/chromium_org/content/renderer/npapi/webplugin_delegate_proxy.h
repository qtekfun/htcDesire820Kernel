// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_NPAPI_WEBPLUGIN_DELEGATE_PROXY_H_
#define CONTENT_RENDERER_NPAPI_WEBPLUGIN_DELEGATE_PROXY_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "content/child/npapi/webplugin_delegate.h"
#include "content/public/common/webplugininfo.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_message.h"
#include "ipc/ipc_sender.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/surface/transport_dib.h"
#include "url/gurl.h"

#if defined(OS_MACOSX)
#include "base/containers/hash_tables.h"
#include "base/memory/linked_ptr.h"
#endif

struct NPObject;
struct PluginHostMsg_URLRequest_Params;
class SkBitmap;

namespace base {
class WaitableEvent;
}

namespace content {
class NPObjectStub;
class PluginChannelHost;
class RenderFrameImpl;
class RenderViewImpl;
class WebPluginImpl;

class WebPluginDelegateProxy
    : public WebPluginDelegate,
      public IPC::Listener,
      public IPC::Sender,
      public base::SupportsWeakPtr<WebPluginDelegateProxy> {
 public:
  WebPluginDelegateProxy(WebPluginImpl* plugin,
                         const std::string& mime_type,
                         const base::WeakPtr<RenderViewImpl>& render_view,
                         RenderFrameImpl* render_frame);

  
  virtual void PluginDestroyed() OVERRIDE;
  virtual bool Initialize(const GURL& url,
                          const std::vector<std::string>& arg_names,
                          const std::vector<std::string>& arg_values,
                          bool load_manually) OVERRIDE;
  virtual void UpdateGeometry(const gfx::Rect& window_rect,
                              const gfx::Rect& clip_rect) OVERRIDE;
  virtual void Paint(SkCanvas* canvas, const gfx::Rect& rect) OVERRIDE;
  virtual NPObject* GetPluginScriptableObject() OVERRIDE;
  virtual struct _NPP* GetPluginNPP() OVERRIDE;
  virtual bool GetFormValue(base::string16* value) OVERRIDE;
  virtual void DidFinishLoadWithReason(const GURL& url, NPReason reason,
                                       int notify_id) OVERRIDE;
  virtual void SetFocus(bool focused) OVERRIDE;
  virtual bool HandleInputEvent(const blink::WebInputEvent& event,
                                WebCursor::CursorInfo* cursor) OVERRIDE;
  virtual int GetProcessId() OVERRIDE;

  
  
  virtual void SetContentAreaFocus(bool has_focus);
#if defined(OS_WIN)
  
  virtual void ImeCompositionUpdated(
      const base::string16& text,
      const std::vector<int>& clauses,
      const std::vector<int>& target,
      int cursor_position,
      int plugin_id);
  
  
  virtual void ImeCompositionCompleted(const base::string16& text,
                                       int plugin_id);
#endif
#if defined(OS_MACOSX)
  
  virtual void SetWindowFocus(bool window_has_focus);
  
  virtual void SetContainerVisibility(bool is_visible);
  
  virtual void WindowFrameChanged(gfx::Rect window_frame, gfx::Rect view_frame);
  
  
  virtual void ImeCompositionCompleted(const base::string16& text,
                                       int plugin_id);
#endif

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  virtual void SendJavaScriptStream(const GURL& url,
                                    const std::string& result,
                                    bool success,
                                    int notify_id) OVERRIDE;

  virtual void DidReceiveManualResponse(const GURL& url,
                                        const std::string& mime_type,
                                        const std::string& headers,
                                        uint32 expected_length,
                                        uint32 last_modified) OVERRIDE;
  virtual void DidReceiveManualData(const char* buffer, int length) OVERRIDE;
  virtual void DidFinishManualLoading() OVERRIDE;
  virtual void DidManualLoadFail() OVERRIDE;
  virtual WebPluginResourceClient* CreateResourceClient(
      unsigned long resource_id, const GURL& url, int notify_id) OVERRIDE;
  virtual WebPluginResourceClient* CreateSeekableResourceClient(
      unsigned long resource_id, int range_request_id) OVERRIDE;
  virtual void FetchURL(unsigned long resource_id,
                        int notify_id,
                        const GURL& url,
                        const GURL& first_party_for_cookies,
                        const std::string& method,
                        const char* buf,
                        unsigned int len,
                        const GURL& referrer,
                        bool notify_redirects,
                        bool is_plugin_src_load,
                        int origin_pid,
                        int render_view_id) OVERRIDE;

  gfx::PluginWindowHandle GetPluginWindowHandle();

 protected:
  friend class base::DeleteHelper<WebPluginDelegateProxy>;
  virtual ~WebPluginDelegateProxy();

 private:
  struct SharedBitmap {
    SharedBitmap();
    ~SharedBitmap();

    scoped_ptr<TransportDIB> dib;
    scoped_ptr<SkCanvas> canvas;
  };

  
  
  void OnSetWindow(gfx::PluginWindowHandle window);
  void OnCompleteURL(const std::string& url_in, std::string* url_out,
                     bool* result);
  void OnHandleURLRequest(const PluginHostMsg_URLRequest_Params& params);
  void OnCancelResource(int id);
  void OnInvalidateRect(const gfx::Rect& rect);
  void OnGetWindowScriptNPObject(int route_id, bool* success);
  void OnResolveProxy(const GURL& url, bool* result, std::string* proxy_list);
  void OnGetPluginElement(int route_id, bool* success);
  void OnSetCookie(const GURL& url,
                   const GURL& first_party_for_cookies,
                   const std::string& cookie);
  void OnGetCookies(const GURL& url, const GURL& first_party_for_cookies,
                    std::string* cookies);
  void OnCancelDocumentLoad();
  void OnInitiateHTTPRangeRequest(const std::string& url,
                                  const std::string& range_info,
                                  int range_request_id);
  void OnDidStartLoading();
  void OnDidStopLoading();
  void OnDeferResourceLoading(unsigned long resource_id, bool defer);
  void OnURLRedirectResponse(bool allow, int resource_id);
  void OnCheckIfRunInsecureContent(const GURL& url, bool* result);
#if defined(OS_MACOSX)
  void OnFocusChanged(bool focused);
  void OnStartIme();
  
  void OnAcceleratedPluginEnabledRendering();
  void OnAcceleratedPluginAllocatedIOSurface(int32 width,
                                             int32 height,
                                             uint32 surface_id);
  void OnAcceleratedPluginSwappedIOSurface();
#endif
#if defined(OS_WIN)
  void OnSetWindowlessData(HANDLE modal_loop_pump_messages_event,
                           gfx::NativeViewId dummy_activation_window);
  void OnNotifyIMEStatus(const int input_mode, const gfx::Rect& caret_rect);
#endif
  
  void SendUpdateGeometry(bool bitmaps_changed);

  
  
  void CopyFromBackBufferToFrontBuffer(const gfx::Rect& rect);

  
  
  void UpdateFrontBuffer(const gfx::Rect& rect, bool allow_buffer_flipping);

  
  void ResetWindowlessBitmaps();

  int front_buffer_index() const {
    return front_buffer_index_;
  }

  int back_buffer_index() const {
    return 1 - front_buffer_index_;
  }

  SkCanvas* front_buffer_canvas() const {
    return transport_stores_[front_buffer_index()].canvas.get();
  }

  SkCanvas* back_buffer_canvas() const {
    return transport_stores_[back_buffer_index()].canvas.get();
  }

  TransportDIB* front_buffer_dib() const {
    return transport_stores_[front_buffer_index()].dib.get();
  }

  TransportDIB* back_buffer_dib() const {
    return transport_stores_[back_buffer_index()].dib.get();
  }

#if !defined(OS_WIN)
  
  
  bool CreateLocalBitmap(std::vector<uint8>* memory,
                         scoped_ptr<SkCanvas>* canvas);
#endif

  
  bool CreateSharedBitmap(scoped_ptr<TransportDIB>* memory,
                          scoped_ptr<SkCanvas>* canvas);

  
  
  
  void WillDestroyWindow();

#if defined(OS_WIN)
  
  bool UseSynchronousGeometryUpdates();
#endif

  base::WeakPtr<RenderViewImpl> render_view_;
  RenderFrameImpl* render_frame_;
  WebPluginImpl* plugin_;
  bool uses_shared_bitmaps_;
#if defined(OS_MACOSX)
  bool uses_compositor_;
#elif defined(OS_WIN)
  
  gfx::NativeViewId dummy_activation_window_;
#endif
  gfx::PluginWindowHandle window_;
  scoped_refptr<PluginChannelHost> channel_host_;
  std::string mime_type_;
  int instance_id_;
  WebPluginInfo info_;

  gfx::Rect plugin_rect_;
  gfx::Rect clip_rect_;

  NPObject* npobject_;

  
  scoped_ptr<NPP_t> npp_;

  
  
  scoped_ptr<base::WaitableEvent> modal_loop_pump_messages_event_;

  
  SkBitmap* sad_plugin_;

  
  bool invalidate_pending_;

  
  bool transparent_;

  
  
  int front_buffer_index_;
  SharedBitmap transport_stores_[2];
  
  
  gfx::Rect transport_store_painted_;
  
  
  
  gfx::Rect front_buffer_diff_;

  
  GURL page_url_;

  DISALLOW_COPY_AND_ASSIGN(WebPluginDelegateProxy);
};

}  

#endif  

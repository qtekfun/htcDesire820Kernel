// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_WEBPLUGIN_H_
#define CONTENT_CHILD_NPAPI_WEBPLUGIN_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gl/gpu_preference.h"

typedef void* HANDLE;

class GURL;
struct NPObject;

namespace content {

class WebPluginResourceClient;
#if defined(OS_MACOSX)
class WebPluginAcceleratedSurface;
#endif

class WebPlugin {
 public:
  virtual ~WebPlugin() {}

  
  
  
  
  virtual void SetWindow(gfx::PluginWindowHandle window) = 0;

  
  virtual void SetAcceptsInputEvents(bool accepts) = 0;

  
  
  virtual void WillDestroyWindow(gfx::PluginWindowHandle window) = 0;

  
  virtual void CancelResource(unsigned long id) = 0;
  virtual void Invalidate() = 0;
  virtual void InvalidateRect(const gfx::Rect& rect) = 0;

  
  
  virtual NPObject* GetWindowScriptNPObject() = 0;

  
  
  virtual NPObject* GetPluginElement() = 0;

  
  virtual bool FindProxyForUrl(const GURL& url, std::string* proxy_list) = 0;

  
  virtual void SetCookie(const GURL& url,
                         const GURL& first_party_for_cookies,
                         const std::string& cookie) = 0;
  virtual std::string GetCookies(const GURL& url,
                                 const GURL& first_party_for_cookies) = 0;

  
  
  
  virtual void HandleURLRequest(const char* url,
                                const char* method,
                                const char* target,
                                const char* buf,
                                unsigned int len,
                                int notify_id,
                                bool popups_allowed,
                                bool notify_redirects) = 0;

  
  virtual void CancelDocumentLoad() = 0;

  
  virtual void InitiateHTTPRangeRequest(const char* url,
                                        const char* range_info,
                                        int range_request_id) = 0;

  virtual void DidStartLoading() = 0;
  virtual void DidStopLoading() = 0;

  
  virtual bool IsOffTheRecord() = 0;

  
  virtual void ResourceClientDeleted(
      WebPluginResourceClient* resource_client) {}

  
  
  virtual void SetDeferResourceLoading(unsigned long resource_id,
                                       bool defer) = 0;

  
  
  virtual void URLRedirectResponse(bool allow, int resource_id) = 0;

  
  
  virtual bool CheckIfRunInsecureContent(const GURL& url) = 0;

#if defined(OS_WIN)
  
  
  
  
  virtual void SetWindowlessData(HANDLE pump_messages_event,
                                 gfx::NativeViewId dummy_activation_window) = 0;
#endif

#if defined(OS_MACOSX)
  
  virtual void FocusChanged(bool focused) {}

  
  virtual void StartIme() {}

  
  virtual WebPluginAcceleratedSurface* GetAcceleratedSurface(
      gfx::GpuPreference gpu_preference) = 0;

  
  
  virtual void AcceleratedPluginEnabledRendering() = 0;
  virtual void AcceleratedPluginAllocatedIOSurface(int32 width,
                                                   int32 height,
                                                   uint32 surface_id) = 0;
  virtual void AcceleratedPluginSwappedIOSurface() = 0;
#endif
};

}  

#endif  

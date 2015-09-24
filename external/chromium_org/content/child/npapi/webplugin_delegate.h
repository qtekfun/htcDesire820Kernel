// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_WEBPLUGIN_DELEGATE_H_
#define CONTENT_CHILD_NPAPI_WEBPLUGIN_DELEGATE_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "build/build_config.h"
#include "third_party/npapi/bindings/npapi.h"
#include "ui/gfx/native_widget_types.h"
#include "webkit/common/cursors/webcursor.h"

class GURL;
class SkCanvas;
struct NPObject;

namespace blink {
class WebInputEvent;
}

namespace gfx {
class Rect;
}

namespace content {

class WebPluginResourceClient;

class WebPluginDelegate {
 public:
  virtual ~WebPluginDelegate() {}

  
  
  
  
  
  
  
  
  
  virtual bool Initialize(const GURL& url,
                          const std::vector<std::string>& arg_names,
                          const std::vector<std::string>& arg_values,
                          bool load_manually) = 0;

  
  
  
  virtual void PluginDestroyed() = 0;

  
  
  
  
  
  virtual void UpdateGeometry(const gfx::Rect& window_rect,
                              const gfx::Rect& clip_rect) = 0;

  
  
  virtual void Paint(SkCanvas* canvas, const gfx::Rect& rect) = 0;

  
  
  virtual void SetFocus(bool focused) = 0;

  
  
  
  virtual bool HandleInputEvent(const blink::WebInputEvent& event,
                                WebCursor::CursorInfo* cursor) = 0;

  
  virtual NPObject* GetPluginScriptableObject() = 0;

  
  virtual struct _NPP* GetPluginNPP() = 0;

  
  
  virtual bool GetFormValue(base::string16* value) = 0;

  
  
  virtual void DidFinishLoadWithReason(const GURL& url, NPReason reason,
                                       int notify_id) = 0;

  
  virtual int GetProcessId() = 0;

  
  
  virtual void SendJavaScriptStream(const GURL& url,
                                    const std::string& result,
                                    bool success,
                                    int notify_id) = 0;

  
  virtual void DidReceiveManualResponse(const GURL& url,
                                        const std::string& mime_type,
                                        const std::string& headers,
                                        uint32 expected_length,
                                        uint32 last_modified) = 0;

  
  virtual void DidReceiveManualData(const char* buffer, int length) = 0;

  
  virtual void DidFinishManualLoading() = 0;

  
  virtual void DidManualLoadFail() = 0;

  
  virtual WebPluginResourceClient* CreateResourceClient(
      unsigned long resource_id,
      const GURL& url,
      int notify_id) = 0;

  
  
  virtual WebPluginResourceClient* CreateSeekableResourceClient(
      unsigned long resource_id, int range_request_id) = 0;

  
  
  
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
                        int render_view_id) = 0;

};

}  

#endif  

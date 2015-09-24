// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_CHILD_NPAPI_PLUGIN_INSTANCE_H_
#define CONTENT_CHILD_NPAPI_PLUGIN_INSTANCE_H_

#include <map>
#include <stack>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "third_party/npapi/bindings/npapi.h"
#include "third_party/npapi/bindings/nphostapi.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

namespace base {
class MessageLoop;
}

namespace content {

class PluginLib;
class PluginHost;
class PluginStream;
class PluginStreamUrl;
class WebPlugin;
class WebPluginResourceClient;

#if defined(OS_MACOSX)
class ScopedCurrentPluginEvent;
#endif

class PluginInstance : public base::RefCountedThreadSafe<PluginInstance> {
 public:
  
  
  PluginInstance(PluginLib* plugin, const std::string &mime_type);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Start(const GURL& url,
             char** const param_names,
             char** const param_values,
             int param_count,
             bool load_manually);

  
  NPP npp() { return npp_; }

  
  gfx::PluginWindowHandle window_handle() const { return window_handle_; }
  void set_window_handle(gfx::PluginWindowHandle value) {
    window_handle_ = value;
  }

  
  
  bool windowless() { return windowless_; }
  void set_windowless(bool value) { windowless_ = value; }

  
  
  
  
  
  bool transparent() { return transparent_; }
  void set_transparent(bool value) { transparent_ = value; }

  
  WebPlugin* webplugin() { return webplugin_; }
  void set_web_plugin(WebPlugin* webplugin) {
    webplugin_ = webplugin;
  }

  
  const std::string &mime_type() { return mime_type_; }

  PluginLib* plugin_lib() { return plugin_.get(); }

#if defined(OS_MACOSX)
  
  NPDrawingModel drawing_model() { return drawing_model_; }
  void set_drawing_model(NPDrawingModel value) { drawing_model_ = value; }
  NPEventModel event_model() { return event_model_; }
  void set_event_model(NPEventModel value) { event_model_ = value; }
  
  
  void set_plugin_origin(const gfx::Point& origin) { plugin_origin_ = origin; }
  
  
  void set_window_frame(const gfx::Rect& frame) {
    containing_window_frame_ = frame;
  }
#endif

  
  
  
  
  
  PluginStreamUrl* CreateStream(unsigned long resource_id,
                                const GURL& url,
                                const std::string& mime_type,
                                int notify_id);

  
  
  
  
  
  void AddStream(PluginStream* stream);

  
  
  void RemoveStream(PluginStream* stream);

  
  void CloseStreams();

  
  
  WebPluginResourceClient* GetRangeRequest(int id);

  
  NPObject* GetPluginScriptableObject();

  
  bool GetFormValue(base::string16* value);

  
  
  void DidFinishLoadWithReason(const GURL& url, NPReason reason, int notify_id);

  
  bool use_mozilla_user_agent() { return use_mozilla_user_agent_; }
  void set_use_mozilla_user_agent() { use_mozilla_user_agent_ = true; }

  
  
  unsigned GetBackingTextureId();

  
  void PluginThreadAsyncCall(void (*func)(void *),
                             void* userData);

  uint32 ScheduleTimer(uint32 interval,
                       NPBool repeat,
                       void (*func)(NPP id, uint32 timer_id));

  void UnscheduleTimer(uint32 timer_id);

  bool ConvertPoint(double source_x, double source_y,
                    NPCoordinateSpace source_space,
                    double* dest_x, double* dest_y,
                    NPCoordinateSpace dest_space);

  NPError PopUpContextMenu(NPMenu* menu);

  
  
  
  NPError NPP_New(unsigned short, short, char *[], char *[]);
  NPError NPP_SetWindow(NPWindow*);
  NPError NPP_NewStream(NPMIMEType, NPStream*, NPBool, unsigned short*);
  NPError NPP_DestroyStream(NPStream*, NPReason);
  int NPP_WriteReady(NPStream*);
  int NPP_Write(NPStream*, int, int, void*);
  void NPP_StreamAsFile(NPStream*, const char*);
  void NPP_URLNotify(const char*, NPReason, void*);
  NPError NPP_GetValue(NPPVariable, void*);
  NPError NPP_SetValue(NPNVariable, void*);
  short NPP_HandleEvent(void*);
  void NPP_Destroy();
  bool NPP_Print(NPPrint* platform_print);
  void NPP_URLRedirectNotify(const char* url, int32_t status,
                             void* notify_data);

  void SendJavaScriptStream(const GURL& url,
                            const std::string& result,
                            bool success,
                            int notify_id);

  void DidReceiveManualResponse(const GURL& url,
                                const std::string& mime_type,
                                const std::string& headers,
                                uint32 expected_length,
                                uint32 last_modified);
  void DidReceiveManualData(const char* buffer, int length);
  void DidFinishManualLoading();
  void DidManualLoadFail();

  void PushPopupsEnabledState(bool enabled);
  void PopPopupsEnabledState();

  bool popups_allowed() const {
    return popups_enabled_stack_.empty() ? false : popups_enabled_stack_.top();
  }

  
  void RequestRead(NPStream* stream, NPByteRange* range_list);

  
  
  void RequestURL(const char* url,
                  const char* method,
                  const char* target,
                  const char* buf,
                  unsigned int len,
                  bool notify,
                  void* notify_data);

  
  
  void URLRedirectResponse(bool allow, void* notify_data);

  bool handles_url_redirects() const { return handles_url_redirects_; }

 private:
  friend class base::RefCountedThreadSafe<PluginInstance>;

#if defined(OS_MACOSX)
  friend class ScopedCurrentPluginEvent;
  
  
  
  
  void set_currently_handled_event(NPCocoaEvent* event) {
    currently_handled_event_ = event;
  }
#endif

  ~PluginInstance();
  void OnPluginThreadAsyncCall(void (*func)(void *), void* userData);
  void OnTimerCall(void (*func)(NPP id, uint32 timer_id),
                   NPP id, uint32 timer_id);
  bool IsValidStream(const NPStream* stream);
  void GetNotifyData(int notify_id, bool* notify, void** notify_data);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  uint8                                    zero_padding_[96];
  scoped_refptr<PluginLib>                 plugin_;
  NPP                                      npp_;
  scoped_refptr<PluginHost>                host_;
  NPPluginFuncs*                           npp_functions_;
  std::vector<scoped_refptr<PluginStream> > open_streams_;
  gfx::PluginWindowHandle                  window_handle_;
  bool                                     windowless_;
  bool                                     transparent_;
  WebPlugin*                               webplugin_;
  std::string                              mime_type_;
  GURL                                     get_url_;
  intptr_t                                 get_notify_data_;
  bool                                     use_mozilla_user_agent_;
#if defined(OS_MACOSX)
  NPDrawingModel                           drawing_model_;
  NPEventModel                             event_model_;
  gfx::Point                               plugin_origin_;
  gfx::Rect                                containing_window_frame_;
  NPCocoaEvent*                            currently_handled_event_;  
#endif
  base::MessageLoop*                       message_loop_;
  scoped_refptr<PluginStreamUrl>           plugin_data_stream_;

  
  
  bool                                     load_manually_;

  
  
  std::stack<bool>                         popups_enabled_stack_;

  
  bool in_close_streams_;

  
  
  std::vector<base::FilePath> files_created_;

  
  uint32 next_timer_id_;

  
  struct TimerInfo {
    uint32 interval;
    bool repeat;
  };
  typedef std::map<uint32, TimerInfo> TimerMap;
  TimerMap timers_;

  
  
  typedef std::map<int, void*> PendingRequestMap;
  PendingRequestMap pending_requests_;
  int next_notify_id_;

  
  
  typedef std::map<int, scoped_refptr<PluginStream> > PendingRangeRequestMap;
  PendingRangeRequestMap pending_range_requests_;
  int next_range_request_id_;
  
  
  bool handles_url_redirects_;

  DISALLOW_COPY_AND_ASSIGN(PluginInstance);
};

#if defined(OS_MACOSX)
class ScopedCurrentPluginEvent {
 public:
  ScopedCurrentPluginEvent(PluginInstance* instance, NPCocoaEvent* event);
  ~ScopedCurrentPluginEvent();

 private:
  scoped_refptr<PluginInstance> instance_;
  DISALLOW_COPY_AND_ASSIGN(ScopedCurrentPluginEvent);
};
#endif

}  

#endif  

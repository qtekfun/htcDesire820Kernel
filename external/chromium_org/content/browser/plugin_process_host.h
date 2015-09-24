// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_PLUGIN_PROCESS_HOST_H_
#define CONTENT_BROWSER_PLUGIN_PROCESS_HOST_H_

#include "build/build_config.h"

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/public/browser/browser_child_process_host_delegate.h"
#include "content/public/browser/browser_child_process_host_iterator.h"
#include "content/public/common/process_type.h"
#include "content/public/common/webplugininfo.h"
#include "ipc/ipc_channel_proxy.h"
#include "ui/gfx/native_widget_types.h"
#include "webkit/common/resource_type.h"

struct ResourceHostMsg_Request;

namespace gfx {
class Rect;
}

namespace IPC {
struct ChannelHandle;
}

namespace net {
class URLRequestContext;
}

namespace content {
class BrowserChildProcessHostImpl;
class ResourceContext;

class CONTENT_EXPORT PluginProcessHost : public BrowserChildProcessHostDelegate,
                                         public IPC::Sender {
 public:
  class Client {
   public:
    
    
    virtual int ID() = 0;
    
    virtual ResourceContext* GetResourceContext() = 0;
    virtual bool OffTheRecord() = 0;
    virtual void SetPluginInfo(const WebPluginInfo& info) = 0;
    virtual void OnFoundPluginProcessHost(PluginProcessHost* host) = 0;
    virtual void OnSentPluginChannelRequest() = 0;
    
    virtual void OnChannelOpened(const IPC::ChannelHandle& handle) = 0;
    virtual void OnError() = 0;

   protected:
    virtual ~Client() {}
  };

  PluginProcessHost();
  virtual ~PluginProcessHost();

  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  
  bool Init(const WebPluginInfo& info);

  
  void ForceShutdown();

  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  
  
  
  void OpenChannelToPlugin(Client* client);

  
  void CancelPendingRequest(Client* client);

  
  void CancelSentRequest(Client* client);

  
  
  
  void OnModalDialogResponse(const std::string& json_retval,
                             IPC::Message* sync_result);

#if defined(OS_MACOSX)
  
  
  void OnAppActivation();
#endif

  const WebPluginInfo& info() const { return info_; }

#if defined(OS_WIN)
  
  void AddWindow(HWND window);
#endif

 private:
  
  
  void RequestPluginChannel(Client* client);

  
  void OnChannelCreated(const IPC::ChannelHandle& channel_handle);
  void OnChannelDestroyed(int renderer_id);

#if defined(OS_WIN)
  void OnPluginWindowDestroyed(HWND window, HWND parent);
#endif

#if defined(USE_X11)
  void OnMapNativeViewId(gfx::NativeViewId id, gfx::PluginWindowHandle* output);
#endif

#if defined(OS_MACOSX)
  void OnPluginSelectWindow(uint32 window_id, gfx::Rect window_rect,
                            bool modal);
  void OnPluginShowWindow(uint32 window_id, gfx::Rect window_rect,
                          bool modal);
  void OnPluginHideWindow(uint32 window_id, gfx::Rect window_rect);
  void OnPluginSetCursorVisibility(bool visible);
#endif

  virtual bool CanShutdown() OVERRIDE;
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;

  void CancelRequests();

  
  void GetContexts(const ResourceHostMsg_Request& request,
                   ResourceContext** resource_context,
                   net::URLRequestContext** request_context);

  
  
  std::vector<Client*> pending_requests_;

  
  
  std::list<Client*> sent_requests_;

  
  WebPluginInfo info_;

#if defined(OS_WIN)
  
  std::set<HWND> plugin_parent_windows_set_;
#endif
#if defined(OS_MACOSX)
  
  std::set<uint32> plugin_visible_windows_set_;
  
  std::set<uint32> plugin_fullscreen_windows_set_;
  
  std::set<uint32> plugin_modal_windows_set_;
  
  bool plugin_cursor_visible_;
#endif

  
  
  
  
  struct ResourceContextEntry {
    ResourceContext* resource_context;
    int ref_count;
  };
  typedef std::map<int, ResourceContextEntry> ResourceContextMap;
  ResourceContextMap resource_context_map_;

  scoped_ptr<BrowserChildProcessHostImpl> process_;

  DISALLOW_COPY_AND_ASSIGN(PluginProcessHost);
};

class PluginProcessHostIterator
    : public BrowserChildProcessHostTypeIterator<PluginProcessHost> {
 public:
  PluginProcessHostIterator()
      : BrowserChildProcessHostTypeIterator<PluginProcessHost>(
          PROCESS_TYPE_PLUGIN) {}
};

}  

#endif  

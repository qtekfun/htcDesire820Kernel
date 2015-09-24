// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_GUEST_MANAGER_H_
#define CONTENT_BROWSER_BROWSER_PLUGIN_BROWSER_PLUGIN_GUEST_MANAGER_H_

#include "base/basictypes.h"
#include "base/supports_user_data.h"
#include "base/values.h"
#include "content/common/content_export.h"
#include "ipc/ipc_message.h"

struct BrowserPluginHostMsg_Attach_Params;
struct BrowserPluginHostMsg_ResizeGuest_Params;
class GURL;

namespace gfx {
class Point;
}

namespace IPC {
class Message;
}  

namespace content {

class BrowserPluginGuest;
class BrowserPluginHostFactory;
class RenderProcessHostImpl;
class RenderWidgetHostImpl;
class SiteInstance;
class WebContents;
class WebContentsImpl;

class CONTENT_EXPORT BrowserPluginGuestManager :
    public base::SupportsUserData::Data {
 public:
  virtual ~BrowserPluginGuestManager();

  static BrowserPluginGuestManager* Create();

  
  
  static void set_factory_for_testing(BrowserPluginHostFactory* factory) {
    content::BrowserPluginGuestManager::factory_ = factory;
  }

  
  int get_next_instance_id() { return ++next_instance_id_; }

  
  
  
  
  BrowserPluginGuest* CreateGuest(
      SiteInstance* embedder_site_instance,
      int instance_id,
      const BrowserPluginHostMsg_Attach_Params& params,
      scoped_ptr<base::DictionaryValue> extra_params);

  
  
  
  BrowserPluginGuest* GetGuestByInstanceID(
      int instance_id,
      int embedder_render_process_id) const;

  
  virtual void AddGuest(int instance_id, WebContentsImpl* guest_web_contents);

  
  
  void RemoveGuest(int instance_id);

  
  
  bool CanEmbedderAccessInstanceIDMaybeKill(int embedder_render_process_id,
                                            int instance_id) const;

  typedef base::Callback<bool(BrowserPluginGuest*)> GuestCallback;
  bool ForEachGuest(WebContentsImpl* embedder_web_contents,
                    const GuestCallback& callback);

  void OnMessageReceived(const IPC::Message& message, int render_process_id);

 private:
  friend class TestBrowserPluginGuestManager;

  BrowserPluginGuestManager();

  
  
  static bool CanEmbedderAccessGuest(int embedder_render_process_id,
                                     BrowserPluginGuest* guest);

  
  
  
  
  
  
  bool CanEmbedderAccessInstanceID(int embedder_render_process_id,
                                   int instance_id) const;

  
  
  SiteInstance* GetGuestSiteInstance(const GURL& guest_site);

  
  void OnUnhandledSwapBuffersACK(int instance_id,
                                 int route_id,
                                 int gpu_host_id,
                                 const std::string& mailbox_name,
                                 uint32 sync_point);

  
  static BrowserPluginHostFactory* factory_;

  
  typedef std::map<int, WebContentsImpl*> GuestInstanceMap;
  GuestInstanceMap guest_web_contents_by_instance_id_;
  int next_instance_id_;

  DISALLOW_COPY_AND_ASSIGN(BrowserPluginGuestManager);
};

}  

#endif  


// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_BROWSER_CONNECTION_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_BROWSER_CONNECTION_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/files/file_path.h"
#include "content/public/renderer/render_frame_observer.h"
#include "content/public/renderer/render_frame_observer_tracker.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"

class GURL;

namespace content {

class PepperBrowserConnection
    : public RenderFrameObserver,
      public RenderFrameObserverTracker<PepperBrowserConnection> {
 public:
  typedef base::Callback<void(const std::vector<int>&)>
      PendingResourceIDCallback;
  explicit PepperBrowserConnection(RenderFrame* render_frame);
  virtual ~PepperBrowserConnection();

  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  

  
  
  
  void SendBrowserCreate(PP_Instance instance,
                         int child_process_id,
                         const std::vector<IPC::Message>& create_messages,
                         const PendingResourceIDCallback& callback);

  
  void DidCreateInProcessInstance(PP_Instance instance,
                                  int render_view_id,
                                  const GURL& document_url,
                                  const GURL& plugin_url);

  
  void DidDeleteInProcessInstance(PP_Instance instance);

 private:
  
  void OnMsgCreateResourceHostsFromHostReply(
      int32_t sequence_number,
      const std::vector<int>& pending_resource_host_ids);

  
  int32_t GetNextSequence();

  
  int32_t next_sequence_number_;

  
  std::map<int32_t, PendingResourceIDCallback> pending_create_map_;
  DISALLOW_COPY_AND_ASSIGN(PepperBrowserConnection);
};

}  

#endif  

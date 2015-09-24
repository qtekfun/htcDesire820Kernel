// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_RESOURCE_MESSAGE_FILTER_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_RESOURCE_MESSAGE_FILTER_H_

#include <map>
#include <string>
#include <vector>

#include "base/atomicops.h"
#include "base/lazy_instance.h"
#include "ipc/ipc_channel_proxy.h"
#include "net/base/completion_callback.h"

class GURL;
class URLRequestAutomationJob;

namespace content {
class BrowserMessageFilter;
}  

namespace net {
class URLRequestContext;
}  

class AutomationResourceMessageFilter
    : public IPC::ChannelProxy::MessageFilter,
      public IPC::Sender {
 public:
  
  struct AutomationDetails {
    AutomationDetails();
    AutomationDetails(int tab, AutomationResourceMessageFilter* flt,
                      bool pending_view);
    ~AutomationDetails();

    int tab_handle;
    int ref_count;
    scoped_refptr<AutomationResourceMessageFilter> filter;
    
    
    bool is_pending_render_view;
  };

  
  AutomationResourceMessageFilter();
  virtual ~AutomationResourceMessageFilter();

  
  
  int NewAutomationRequestId() {
    return base::subtle::Barrier_AtomicIncrement(&unique_request_id_, 1);
  }

  
  virtual void OnFilterAdded(IPC::Channel* channel);
  virtual void OnFilterRemoved();

  
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelClosing() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual bool Send(IPC::Message* message);

  
  virtual bool RegisterRequest(URLRequestAutomationJob* job);

  
  virtual void UnRegisterRequest(URLRequestAutomationJob* job);

  
  
  
  
  static bool RegisterRenderView(int renderer_pid,
                                 int renderer_id,
                                 int tab_handle,
                                 AutomationResourceMessageFilter* filter,
                                 bool pending_view);
  static void UnRegisterRenderView(int renderer_pid, int renderer_id);

  
  
  
  static bool ResumePendingRenderView(int renderer_pid,
                                      int renderer_id,
                                      int tab_handle,
                                      AutomationResourceMessageFilter* filter);

  
  static bool LookupRegisteredRenderView(
      int renderer_pid, int renderer_id, AutomationDetails* details);

  
  bool SendDownloadRequestToHost(int routing_id, int tab_handle,
                                 int request_id);

  
  
  static void GetCookiesForUrl(content::BrowserMessageFilter* filter,
                               net::URLRequestContext* context,
                               int render_process_id,
                               IPC::Message* reply_msg,
                               const GURL& url);

 protected:
  
  
  
  bool GetAutomationRequestId(int request_id, int* automation_request_id);

  static void RegisterRenderViewInIOThread(int renderer_pid, int renderer_id,
      int tab_handle, AutomationResourceMessageFilter* filter,
      bool pending_view);
  static void UnRegisterRenderViewInIOThread(int renderer_pid, int renderer_id);

  static void ResumePendingRenderViewInIOThread(
      int renderer_pid, int renderer_id, int tab_handle,
      AutomationResourceMessageFilter* filter);

 private:
  
  
  static void ResumeJobsForPendingView(
      int tab_handle,
      AutomationResourceMessageFilter* old_filter,
      AutomationResourceMessageFilter* new_filter);

  static int GetNextCompletionCallbackId() {
    return ++next_completion_callback_id_;
  }

  
  
  struct RendererId {
    int pid_;
    int id_;

    RendererId() : pid_(0), id_(0) {}
    RendererId(int pid, int id) : pid_(pid), id_(id) {}

    bool operator < (const RendererId& rhs) const {
      return ((pid_ == rhs.pid_) ? (id_ < rhs.id_) : (pid_ < rhs.pid_));
    }
  };

  typedef std::map<RendererId, AutomationDetails> RenderViewMap;
  typedef std::map<int, URLRequestAutomationJob*> RequestMap;

  
  
  IPC::Channel* channel_;

  
  static int unique_request_id_;

  
  RequestMap request_map_;

  
  
  RequestMap pending_request_map_;

  
  static base::LazyInstance<RenderViewMap> filtered_render_views_;

  
  
  static int next_completion_callback_id_;

  DISALLOW_COPY_AND_ASSIGN(AutomationResourceMessageFilter);
};

#endif  

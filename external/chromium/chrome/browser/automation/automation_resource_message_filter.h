// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOMATION_AUTOMATION_RESOURCE_MESSAGE_FILTER_H_
#define CHROME_BROWSER_AUTOMATION_AUTOMATION_RESOURCE_MESSAGE_FILTER_H_
#pragma once

#include <map>

#include "base/atomicops.h"
#include "base/lazy_instance.h"
#include "ipc/ipc_channel_proxy.h"
#include "net/base/completion_callback.h"
#include "net/base/cookie_store.h"

class URLRequestAutomationJob;
class GURL;

namespace net {
class CookieStore;
}  

class AutomationResourceMessageFilter
    : public IPC::ChannelProxy::MessageFilter,
      public IPC::Message::Sender {
 public:
  
  struct AutomationDetails {
    AutomationDetails();
    AutomationDetails(int tab, AutomationResourceMessageFilter* flt,
                      bool pending_view);
    ~AutomationDetails();

    void set_cookie_store(net::CookieStore* cookie_store) {
      cookie_store_ = cookie_store;
    }

    net::CookieStore* cookie_store() {
      return cookie_store_.get();
    }

    int tab_handle;
    int ref_count;
    scoped_refptr<AutomationResourceMessageFilter> filter;
    
    
    bool is_pending_render_view;

    
    scoped_refptr<net::CookieStore> cookie_store_;
  };

  
  AutomationResourceMessageFilter();
  virtual ~AutomationResourceMessageFilter();

  
  
  int NewAutomationRequestId() {
    return base::subtle::Barrier_AtomicIncrement(&unique_request_id_, 1);
  }

  
  virtual void OnFilterAdded(IPC::Channel* channel);
  virtual void OnFilterRemoved();

  virtual void OnChannelConnected(int32 peer_pid);
  virtual void OnChannelClosing();
  virtual bool OnMessageReceived(const IPC::Message& message);

  
  virtual bool Send(IPC::Message* message);

  
  virtual bool RegisterRequest(URLRequestAutomationJob* job);

  
  virtual void UnRegisterRequest(URLRequestAutomationJob* job);

  
  
  
  
  static bool RegisterRenderView(int renderer_pid, int renderer_id,
      int tab_handle, AutomationResourceMessageFilter* filter,
      bool pending_view);
  static void UnRegisterRenderView(int renderer_pid, int renderer_id);

  
  
  
  static bool ResumePendingRenderView(int renderer_pid, int renderer_id,
      int tab_handle, AutomationResourceMessageFilter* filter);

  
  static bool LookupRegisteredRenderView(
      int renderer_pid, int renderer_id, AutomationDetails* details);

  
  bool SendDownloadRequestToHost(int routing_id, int tab_handle,
                                 int request_id);

  
  
  
  static bool GetCookiesForUrl(const GURL& url,
                               net::CompletionCallback* callback);

  
  static bool SetCookiesForUrl(const GURL& url, const std::string& cookie_line,
                               net::CompletionCallback* callback);

  
  
  
  
  
  void OnGetCookiesHostResponse(int tab_handle, bool success, const GURL& url,
                                const std::string& cookies, int cookie_id);

 protected:
  
  
  
  bool GetAutomationRequestId(int request_id, int* automation_request_id);

  static void RegisterRenderViewInIOThread(int renderer_pid, int renderer_id,
      int tab_handle, AutomationResourceMessageFilter* filter,
      bool pending_view);
  static void UnRegisterRenderViewInIOThread(int renderer_pid, int renderer_id);

  static bool ResumePendingRenderViewInIOThread(
      int renderer_pid, int renderer_id, int tab_handle,
      AutomationResourceMessageFilter* filter);

  
  
  static void OnGetCookiesHostResponseInternal(
      int tab_handle, bool success, const GURL& url,
      const std::string& cookies, net::CompletionCallback* callback,
      net::CookieStore* cookie_store);

 private:
  void OnSetFilteredInet(bool enable);
  void OnGetFilteredInetHitCount(int* hit_count);
  void OnRecordHistograms(const std::vector<std::string>& histogram_list);

  
  
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
  typedef std::map<int, scoped_refptr<URLRequestAutomationJob> > RequestMap;

  
  
  IPC::Channel* channel_;

  
  static int unique_request_id_;

  
  RequestMap request_map_;

  
  
  RequestMap pending_request_map_;

  
  static base::LazyInstance<RenderViewMap> filtered_render_views_;

  
  
  struct CookieCompletionInfo;

  
  
  
  
  
  typedef std::map<int, CookieCompletionInfo> CompletionCallbackMap;
  static base::LazyInstance<CompletionCallbackMap> completion_callback_map_;

  
  
  static int next_completion_callback_id_;

  DISALLOW_COPY_AND_ASSIGN(AutomationResourceMessageFilter);
};

#endif  


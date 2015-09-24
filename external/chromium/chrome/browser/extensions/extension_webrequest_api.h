// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_WEBREQUEST_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_WEBREQUEST_API_H_
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/memory/singleton.h"
#include "chrome/browser/extensions/extension_function.h"
#include "chrome/browser/profiles/profile.h"
#include "ipc/ipc_message.h"
#include "net/base/completion_callback.h"
#include "webkit/glue/resource_type.h"

class ExtensionEventRouterForwarder;
class GURL;

namespace net {
class HttpRequestHeaders;
class URLRequest;
}

class ExtensionWebRequestEventRouter {
 public:
  struct RequestFilter;
  struct ExtraInfoSpec;

  static ExtensionWebRequestEventRouter* GetInstance();

  
  
  
  int OnBeforeRequest(ProfileId profile_id,
                      ExtensionEventRouterForwarder* event_router,
                      net::URLRequest* request,
                      net::CompletionCallback* callback,
                      GURL* new_url);

  
  
  
  
  int OnBeforeSendHeaders(ProfileId profile_id,
                          ExtensionEventRouterForwarder* event_router,
                          uint64 request_id,
                          net::CompletionCallback* callback,
                          net::HttpRequestHeaders* headers);

  void OnURLRequestDestroyed(ProfileId profile_id, net::URLRequest* request);

  
  
  void OnEventHandled(
      ProfileId profile_id,
      const std::string& extension_id,
      const std::string& event_name,
      const std::string& sub_event_name,
      uint64 request_id,
      bool cancel,
      const GURL& new_url);

  
  
  
  
  void AddEventListener(
      ProfileId profile_id,
      const std::string& extension_id,
      const std::string& event_name,
      const std::string& sub_event_name,
      const RequestFilter& filter,
      int extra_info_spec);

  
  void RemoveEventListener(
      ProfileId profile_id,
      const std::string& extension_id,
      const std::string& sub_event_name);

 private:
  friend struct DefaultSingletonTraits<ExtensionWebRequestEventRouter>;
  struct EventListener;
  struct BlockedRequest;
  typedef std::map<std::string, std::set<EventListener> > ListenerMapForProfile;
  typedef std::map<ProfileId, ListenerMapForProfile> ListenerMap;
  typedef std::map<uint64, BlockedRequest> BlockedRequestMap;
  typedef std::map<uint64, net::URLRequest*> HttpRequestMap;

  ExtensionWebRequestEventRouter();
  ~ExtensionWebRequestEventRouter();

  bool DispatchEvent(
      ProfileId profile_id,
      ExtensionEventRouterForwarder* event_router,
      net::URLRequest* request,
      net::CompletionCallback* callback,
      const std::vector<const EventListener*>& listeners,
      const ListValue& args);

  
  
  std::vector<const EventListener*> GetMatchingListeners(
      ProfileId profile_id,
      const std::string& event_name,
      const GURL& url,
      int tab_id,
      int window_id,
      ResourceType::Type resource_type);

  
  std::vector<const EventListener*> GetMatchingListeners(
      ProfileId profile_id,
      const std::string& event_name,
      net::URLRequest* request);

  
  
  
  void DecrementBlockCount(uint64 request_id, bool cancel, const GURL& new_url);

  void OnRequestDeleted(net::URLRequest* request);

  
  
  ListenerMap listeners_;

  
  
  BlockedRequestMap blocked_requests_;

  
  
  HttpRequestMap http_requests_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionWebRequestEventRouter);
};

class WebRequestAddEventListener : public SyncExtensionFunction {
 public:
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("experimental.webRequest.addEventListener");
};

class WebRequestEventHandled : public SyncExtensionFunction {
 public:
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("experimental.webRequest.eventHandled");
};

#endif  

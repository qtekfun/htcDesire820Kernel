// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_EVENT_ROUTER_FORWARDER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_EVENT_ROUTER_FORWARDER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/profiles/profile.h"
#include "content/browser/browser_thread.h"

class GURL;

class ExtensionEventRouterForwarder
    : public base::RefCountedThreadSafe<ExtensionEventRouterForwarder> {
 public:
  ExtensionEventRouterForwarder();

  
  
  
  
  void BroadcastEventToRenderers(const std::string& event_name,
                                 const std::string& event_args,
                                 const GURL& event_url);

  
  
  
  
  
  void BroadcastEventToExtension(const std::string& extension_id,
                                 const std::string& event_name,
                                 const std::string& event_args,
                                 const GURL& event_url);

  
  
  
  
  void DispatchEventToRenderers(const std::string& event_name,
                                const std::string& event_args,
                                ProfileId profile_id,
                                bool use_profile_to_restrict_events,
                                const GURL& event_url);

  
  
  
  
  void DispatchEventToExtension(const std::string& extension_id,
                                const std::string& event_name,
                                const std::string& event_args,
                                ProfileId profile_id,
                                bool use_profile_to_restrict_events,
                                const GURL& event_url);

 protected:
  
  virtual ~ExtensionEventRouterForwarder();

  
  
  
  
  virtual void CallExtensionEventRouter(Profile* profile,
                                        const std::string& extension_id,
                                        const std::string& event_name,
                                        const std::string& event_args,
                                        Profile* restrict_to_profile,
                                        const GURL& event_url);

 private:
  friend class base::RefCountedThreadSafe<ExtensionEventRouterForwarder>;

  
  void HandleEvent(const std::string& extension_id,
                   const std::string& event_name,
                   const std::string& event_args,
                   ProfileId profile_id,
                   bool use_profile_to_restrict_events,
                   const GURL& event_url);

  DISALLOW_COPY_AND_ASSIGN(ExtensionEventRouterForwarder);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EVENT_ROUTER_FORWARDER_H_
#define CHROME_BROWSER_EXTENSIONS_EVENT_ROUTER_FORWARDER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "chrome/browser/profiles/profile.h"

class GURL;

namespace extensions {

class EventRouterForwarder
    : public base::RefCountedThreadSafe<EventRouterForwarder> {
 public:
  EventRouterForwarder();

  
  
  
  
  void BroadcastEventToRenderers(const std::string& event_name,
                                 scoped_ptr<base::ListValue> event_args,
                                 const GURL& event_url);

  
  
  
  
  
  void BroadcastEventToExtension(const std::string& extension_id,
                                 const std::string& event_name,
                                 scoped_ptr<base::ListValue> event_args,
                                 const GURL& event_url);

  
  
  
  
  void DispatchEventToRenderers(const std::string& event_name,
                                scoped_ptr<base::ListValue> event_args,
                                void* profile,
                                bool use_profile_to_restrict_events,
                                const GURL& event_url);

  
  
  
  
  void DispatchEventToExtension(const std::string& extension_id,
                                const std::string& event_name,
                                scoped_ptr<base::ListValue> event_args,
                                void* profile,
                                bool use_profile_to_restrict_events,
                                const GURL& event_url);

 protected:
  
  virtual ~EventRouterForwarder();

  
  
  virtual void HandleEvent(const std::string& extension_id,
                           const std::string& event_name,
                           scoped_ptr<base::ListValue> event_args,
                           void* profile,
                           bool use_profile_to_restrict_events,
                           const GURL& event_url);

  
  
  
  
  virtual void CallEventRouter(Profile* profile,
                               const std::string& extension_id,
                               const std::string& event_name,
                               scoped_ptr<base::ListValue> event_args,
                               Profile* restrict_to_profile,
                               const GURL& event_url);

 private:
  friend class base::RefCountedThreadSafe<EventRouterForwarder>;

  DISALLOW_COPY_AND_ASSIGN(EventRouterForwarder);
};

}  

#endif  

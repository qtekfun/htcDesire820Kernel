// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_EVENT_ROUTER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_EVENT_ROUTER_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class GURL;
class Extension;
class ExtensionDevToolsManager;
class Profile;
class RenderProcessHost;

class ExtensionEventRouter : public NotificationObserver {
 public:
  
  
  static bool CanCrossIncognito(Profile* profile,
                                const std::string& extension_id);
  static bool CanCrossIncognito(Profile* profile, const Extension* extension);

  explicit ExtensionEventRouter(Profile* profile);
  ~ExtensionEventRouter();

  
  
  
  
  void AddEventListener(const std::string& event_name,
                        RenderProcessHost* process,
                        const std::string& extension_id);
  void RemoveEventListener(const std::string& event_name,
                           RenderProcessHost* process,
                           const std::string& extension_id);

  
  bool HasEventListener(const std::string& event_name);

  
  bool ExtensionHasEventListener(const std::string& extension_id,
                                 const std::string& event_name);

  
  
  
  
  
  
  void DispatchEventToRenderers(
      const std::string& event_name, const std::string& event_args,
      Profile* restrict_to_profile, const GURL& event_url);

  
  void DispatchEventToExtension(
      const std::string& extension_id,
      const std::string& event_name, const std::string& event_args,
      Profile* restrict_to_profile, const GURL& event_url);

 protected:
  
  
  virtual void DispatchEventImpl(
      const std::string& extension_id,
      const std::string& event_name, const std::string& event_args,
      Profile* restrict_to_profile, const GURL& event_url);

 private:
  
  struct EventListener;

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  Profile* profile_;

  NotificationRegistrar registrar_;

  scoped_refptr<ExtensionDevToolsManager> extension_devtools_manager_;

  
  
  typedef std::map<std::string, std::set<EventListener> > ListenerMap;
  ListenerMap listeners_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionEventRouter);
};

#endif  

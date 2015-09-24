// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_PROCESS_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_PROCESS_MANAGER_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/memory/ref_counted.h"
#include "chrome/common/view_types.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class Browser;
class BrowsingInstance;
class Extension;
class ExtensionHost;
class GURL;
class Profile;
class RenderProcessHost;
class SiteInstance;

class ExtensionProcessManager : public NotificationObserver {
 public:
  static ExtensionProcessManager* Create(Profile* profile);
  virtual ~ExtensionProcessManager();

  
  
  
  virtual ExtensionHost* CreateView(const Extension* extension,
                            const GURL& url,
                            Browser* browser,
                            ViewType::Type view_type);
  ExtensionHost* CreateView(const GURL& url,
                            Browser* browser,
                            ViewType::Type view_type);
  ExtensionHost* CreatePopup(const Extension* extension,
                             const GURL& url,
                             Browser* browser);
  ExtensionHost* CreatePopup(const GURL& url, Browser* browser);
  ExtensionHost* CreateInfobar(const Extension* extension,
                               const GURL& url,
                               Browser* browser);
  ExtensionHost* CreateInfobar(const GURL& url,
                               Browser* browser);

  
  void OpenOptionsPage(const Extension* extension, Browser* browser);

  
  
  virtual void CreateBackgroundHost(const Extension* extension,
                                    const GURL& url);

  
  
  ExtensionHost* GetBackgroundHostForExtension(const Extension* extension);

  
  virtual SiteInstance* GetSiteInstanceForURL(const GURL& url);

  
  
  void RegisterExtensionProcess(const std::string& extension_id,
                                int process_id);

  
  void UnregisterExtensionProcess(int process_id);

  
  virtual RenderProcessHost* GetExtensionProcess(const GURL& url);

  
  RenderProcessHost* GetExtensionProcess(const std::string& extension_id);

  
  bool HasExtensionHost(ExtensionHost* host) const;

  typedef std::set<ExtensionHost*> ExtensionHostSet;
  typedef ExtensionHostSet::const_iterator const_iterator;
  const_iterator begin() const { return all_hosts_.begin(); }
  const_iterator end() const { return all_hosts_.end(); }

 protected:
  explicit ExtensionProcessManager(Profile* profile);

  
  void OnExtensionHostCreated(ExtensionHost* host, bool is_background);

  
  void CloseBackgroundHosts();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  NotificationRegistrar registrar_;

  
  ExtensionHostSet all_hosts_;

  
  ExtensionHostSet background_hosts_;

  
  
  scoped_refptr<BrowsingInstance> browsing_instance_;

  
  typedef std::map<std::string, int> ProcessIDMap;
  ProcessIDMap process_ids_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionProcessManager);
};

#endif  

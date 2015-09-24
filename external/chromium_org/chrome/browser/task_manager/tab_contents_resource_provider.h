// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TASK_MANAGER_TAB_CONTENTS_RESOURCE_PROVIDER_H_
#define CHROME_BROWSER_TASK_MANAGER_TAB_CONTENTS_RESOURCE_PROVIDER_H_

#include <map>

#include "base/basictypes.h"
#include "chrome/browser/task_manager/resource_provider.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class TaskManager;

namespace content {
class WebContents;
class NotificationSource;
class NotificationDetails;
}

namespace task_manager {

class TabContentsResource;

class TabContentsResourceProvider : public ResourceProvider,
                                    public content::NotificationObserver {
 public:
  explicit TabContentsResourceProvider(TaskManager* task_manager);

  virtual Resource* GetResource(int origin_pid,
                                int render_process_host_id,
                                int routing_id) OVERRIDE;
  virtual void StartUpdating() OVERRIDE;
  virtual void StopUpdating() OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

 private:
  virtual ~TabContentsResourceProvider();

  void Add(content::WebContents* web_contents);
  void Remove(content::WebContents* web_contents);

  void AddToTaskManager(content::WebContents* web_contents);

  
  
  bool updating_;

  TaskManager* task_manager_;

  
  
  std::map<content::WebContents*, TabContentsResource*> resources_;

  
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(TabContentsResourceProvider);
};

}  

#endif  

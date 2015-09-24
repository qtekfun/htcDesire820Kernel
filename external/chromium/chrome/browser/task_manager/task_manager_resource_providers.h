// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TASK_MANAGER_TASK_MANAGER_RESOURCE_PROVIDERS_H_
#define CHROME_BROWSER_TASK_MANAGER_TASK_MANAGER_RESOURCE_PROVIDERS_H_
#pragma once

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/process_util.h"
#include "chrome/browser/task_manager/task_manager.h"
#include "content/common/child_process_info.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebCache.h"

class BackgroundContents;
class BalloonHost;
class Extension;
class ExtensionHost;
class RenderViewHost;
class TabContentsWrapper;

namespace prerender {
class PrerenderContents;
}


class TaskManagerRendererResource : public TaskManager::Resource {
 public:
  TaskManagerRendererResource(base::ProcessHandle process,
                              RenderViewHost* render_view_host);
  virtual ~TaskManagerRendererResource();

  
  virtual base::ProcessHandle GetProcess() const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual bool ReportsCacheStats() const OVERRIDE;
  virtual WebKit::WebCache::ResourceTypeStats GetWebCoreCacheStats() const
      OVERRIDE;
  virtual bool ReportsV8MemoryStats() const OVERRIDE;
  virtual size_t GetV8MemoryAllocated() const OVERRIDE;
  virtual size_t GetV8MemoryUsed() const OVERRIDE;

  
  virtual bool SupportNetworkUsage() const;
  virtual void SetSupportNetworkUsage() { }

  virtual void Refresh();

  virtual void NotifyResourceTypeStats(
      const WebKit::WebCache::ResourceTypeStats& stats);

  virtual void NotifyV8HeapStats(size_t v8_memory_allocated,
                                 size_t v8_memory_used);

 private:
  base::ProcessHandle process_;
  int pid_;

  
  RenderViewHost* render_view_host_;
  
  
  WebKit::WebCache::ResourceTypeStats stats_;
  
  bool pending_stats_update_;

  
  size_t v8_memory_allocated_;
  size_t v8_memory_used_;
  bool pending_v8_memory_allocated_update_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerRendererResource);
};

class TaskManagerTabContentsResource : public TaskManagerRendererResource {
 public:
  explicit TaskManagerTabContentsResource(TabContentsWrapper* tab_contents);
  virtual ~TaskManagerTabContentsResource();

  
  virtual Type GetType() const OVERRIDE;
  virtual string16 GetTitle() const OVERRIDE;
  virtual SkBitmap GetIcon() const OVERRIDE;
  virtual TabContentsWrapper* GetTabContents() const OVERRIDE;
  virtual const Extension* GetExtension() const OVERRIDE;

 private:
  TabContentsWrapper* tab_contents_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerTabContentsResource);
};

class TaskManagerTabContentsResourceProvider
    : public TaskManager::ResourceProvider,
      public NotificationObserver {
 public:
  explicit TaskManagerTabContentsResourceProvider(TaskManager* task_manager);

  virtual TaskManager::Resource* GetResource(int origin_pid,
                                             int render_process_host_id,
                                             int routing_id);
  virtual void StartUpdating();
  virtual void StopUpdating();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  virtual ~TaskManagerTabContentsResourceProvider();

  void Add(TabContentsWrapper* tab_contents);
  void Remove(TabContentsWrapper* tab_contents);

  void AddToTaskManager(TabContentsWrapper* tab_contents);

  
  
  bool updating_;

  TaskManager* task_manager_;

  
  
  std::map<TabContentsWrapper*, TaskManagerTabContentsResource*> resources_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerTabContentsResourceProvider);
};

class TaskManagerPrerenderResource : public TaskManagerRendererResource {
 public:
  explicit TaskManagerPrerenderResource(RenderViewHost* render_view_host);
  virtual ~TaskManagerPrerenderResource();

  
  virtual Type GetType() const OVERRIDE;
  virtual string16 GetTitle() const OVERRIDE;
  virtual SkBitmap GetIcon() const OVERRIDE;

 private:
  static SkBitmap* default_icon_;
  std::pair<int, int> process_route_id_pair_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerPrerenderResource);
};

class TaskManagerPrerenderResourceProvider
    : public TaskManager::ResourceProvider,
      public NotificationObserver {
 public:
  explicit TaskManagerPrerenderResourceProvider(TaskManager* task_manager);

  virtual TaskManager::Resource* GetResource(int origin_pid,
                                             int render_process_host_id,
                                             int routing_id);
  virtual void StartUpdating();
  virtual void StopUpdating();

  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);
 private:
  virtual ~TaskManagerPrerenderResourceProvider();

  void Add(const std::pair<int, int>& process_route_id_pair);
  void Remove(const std::pair<int, int>& process_route_id_pair);

  void AddToTaskManager(const std::pair<int, int>& process_route_id_pair);

  bool updating_;
  TaskManager* task_manager_;
  typedef std::map<std::pair<int, int>, TaskManagerPrerenderResource*>
      ResourceMap;
  ResourceMap resources_;
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerPrerenderResourceProvider);
};

class TaskManagerBackgroundContentsResource
    : public TaskManagerRendererResource {
 public:
  TaskManagerBackgroundContentsResource(
      BackgroundContents* background_contents,
      const string16& application_name);
  virtual ~TaskManagerBackgroundContentsResource();

  
  virtual string16 GetTitle() const OVERRIDE;
  virtual SkBitmap GetIcon() const OVERRIDE;
  virtual bool IsBackground() const OVERRIDE;

  const string16& application_name() const { return application_name_; }
 private:
  BackgroundContents* background_contents_;

  string16 application_name_;

  
  
  
  static SkBitmap* default_icon_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerBackgroundContentsResource);
};

class TaskManagerBackgroundContentsResourceProvider
    : public TaskManager::ResourceProvider,
      public NotificationObserver {
 public:
  explicit TaskManagerBackgroundContentsResourceProvider(
      TaskManager* task_manager);

  virtual TaskManager::Resource* GetResource(int origin_pid,
                                             int render_process_host_id,
                                             int routing_id);
  virtual void StartUpdating();
  virtual void StopUpdating();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  virtual ~TaskManagerBackgroundContentsResourceProvider();

  void Add(BackgroundContents* background_contents, const string16& title);
  void Remove(BackgroundContents* background_contents);

  void AddToTaskManager(BackgroundContents* background_contents,
                        const string16& title);

  
  
  bool updating_;

  TaskManager* task_manager_;

  
  
  std::map<BackgroundContents*, TaskManagerBackgroundContentsResource*>
      resources_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerBackgroundContentsResourceProvider);
};

class TaskManagerChildProcessResource : public TaskManager::Resource {
 public:
  explicit TaskManagerChildProcessResource(const ChildProcessInfo& child_proc);
  virtual ~TaskManagerChildProcessResource();

  
  virtual string16 GetTitle() const OVERRIDE;
  virtual SkBitmap GetIcon() const OVERRIDE;
  virtual base::ProcessHandle GetProcess() const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual bool SupportNetworkUsage() const OVERRIDE;
  virtual void SetSupportNetworkUsage() OVERRIDE;

  
  int process_id() const { return pid_; }

 private:
  
  
  string16 GetLocalizedTitle() const;

  ChildProcessInfo child_process_;
  int pid_;
  mutable string16 title_;
  bool network_usage_support_;

  
  
  
  static SkBitmap* default_icon_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerChildProcessResource);
};

class TaskManagerChildProcessResourceProvider
    : public TaskManager::ResourceProvider,
      public NotificationObserver {
 public:
  explicit TaskManagerChildProcessResourceProvider(TaskManager* task_manager);

  virtual TaskManager::Resource* GetResource(int origin_pid,
                                             int render_process_host_id,
                                             int routing_id);
  virtual void StartUpdating();
  virtual void StopUpdating();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  virtual void RetrieveChildProcessInfo();

  
  virtual void ChildProcessInfoRetreived();

  
  
  bool updating_;

  
  std::vector<ChildProcessInfo> existing_child_process_info_;

 private:
  virtual ~TaskManagerChildProcessResourceProvider();

  void Add(const ChildProcessInfo& child_process_info);
  void Remove(const ChildProcessInfo& child_process_info);

  void AddToTaskManager(const ChildProcessInfo& child_process_info);

  TaskManager* task_manager_;

  
  
  std::map<ChildProcessInfo, TaskManagerChildProcessResource*> resources_;

  
  
  std::map<int, TaskManagerChildProcessResource*> pid_to_resources_;

  
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerChildProcessResourceProvider);
};

class TaskManagerExtensionProcessResource : public TaskManager::Resource {
 public:
  explicit TaskManagerExtensionProcessResource(ExtensionHost* extension_host);
  virtual ~TaskManagerExtensionProcessResource();

  
  virtual string16 GetTitle() const OVERRIDE;
  virtual SkBitmap GetIcon() const OVERRIDE;
  virtual base::ProcessHandle GetProcess() const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual bool SupportNetworkUsage() const OVERRIDE;
  virtual void SetSupportNetworkUsage() OVERRIDE;
  virtual const Extension* GetExtension() const OVERRIDE;

  
  int process_id() const { return pid_; }

  
  virtual bool IsBackground() const;

 private:
  
  static SkBitmap* default_icon_;

  ExtensionHost* extension_host_;

  
  base::ProcessHandle process_handle_;
  int pid_;
  string16 title_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerExtensionProcessResource);
};

class TaskManagerExtensionProcessResourceProvider
    : public TaskManager::ResourceProvider,
      public NotificationObserver {
 public:
  explicit TaskManagerExtensionProcessResourceProvider(
      TaskManager* task_manager);

  virtual TaskManager::Resource* GetResource(int origin_pid,
                                             int render_process_host_id,
                                             int routing_id);
  virtual void StartUpdating();
  virtual void StopUpdating();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  virtual ~TaskManagerExtensionProcessResourceProvider();

  void AddToTaskManager(ExtensionHost* extension_host);
  void RemoveFromTaskManager(ExtensionHost* extension_host);

  TaskManager* task_manager_;

  
  std::map<ExtensionHost*, TaskManagerExtensionProcessResource*> resources_;

  
  
  std::map<int, TaskManagerExtensionProcessResource*> pid_to_resources_;

  
  NotificationRegistrar registrar_;

  bool updating_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerExtensionProcessResourceProvider);
};

class TaskManagerNotificationResource : public TaskManager::Resource {
 public:
  explicit TaskManagerNotificationResource(BalloonHost* balloon_host);
  virtual ~TaskManagerNotificationResource();

  
  virtual string16 GetTitle() const OVERRIDE;
  virtual SkBitmap GetIcon() const OVERRIDE;
  virtual base::ProcessHandle GetProcess() const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual bool SupportNetworkUsage() const OVERRIDE;
  virtual void SetSupportNetworkUsage() OVERRIDE { }

 private:
  
  static SkBitmap* default_icon_;

  
  BalloonHost* balloon_host_;

  
  base::ProcessHandle process_handle_;
  int pid_;
  string16 title_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerNotificationResource);
};

class TaskManagerNotificationResourceProvider
    : public TaskManager::ResourceProvider,
      public NotificationObserver {
 public:
  explicit TaskManagerNotificationResourceProvider(TaskManager* task_manager);

  
  virtual TaskManager::Resource* GetResource(int origin_pid,
                                             int render_process_host_id,
                                             int routing_id);
  virtual void StartUpdating();
  virtual void StopUpdating();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

 private:
  virtual ~TaskManagerNotificationResourceProvider();

  void AddToTaskManager(BalloonHost* balloon_host);
  void RemoveFromTaskManager(BalloonHost* balloon_host);

  TaskManager* task_manager_;

  
  std::map<BalloonHost*, TaskManagerNotificationResource*> resources_;

  
  NotificationRegistrar registrar_;

  bool updating_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerNotificationResourceProvider);
};

class TaskManagerBrowserProcessResource : public TaskManager::Resource {
 public:
  TaskManagerBrowserProcessResource();
  virtual ~TaskManagerBrowserProcessResource();

  
  virtual string16 GetTitle() const OVERRIDE;
  virtual SkBitmap GetIcon() const OVERRIDE;
  virtual base::ProcessHandle GetProcess() const OVERRIDE;
  virtual Type GetType() const OVERRIDE;

  virtual bool SupportNetworkUsage() const OVERRIDE;
  virtual void SetSupportNetworkUsage() OVERRIDE;

  virtual bool ReportsSqliteMemoryUsed() const OVERRIDE;
  virtual size_t SqliteMemoryUsedBytes() const OVERRIDE;

 private:
  base::ProcessHandle process_;
  mutable string16 title_;

  static SkBitmap* default_icon_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerBrowserProcessResource);
};

class TaskManagerBrowserProcessResourceProvider
    : public TaskManager::ResourceProvider {
 public:
  explicit TaskManagerBrowserProcessResourceProvider(
      TaskManager* task_manager);

  virtual TaskManager::Resource* GetResource(int origin_pid,
                                             int render_process_host_id,
                                             int routing_id);
  virtual void StartUpdating();
  virtual void StopUpdating();

  
  
  bool updating_;

 private:
  virtual ~TaskManagerBrowserProcessResourceProvider();

  void AddToTaskManager(ChildProcessInfo child_process_info);

  TaskManager* task_manager_;
  TaskManagerBrowserProcessResource resource_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerBrowserProcessResourceProvider);
};

#endif  

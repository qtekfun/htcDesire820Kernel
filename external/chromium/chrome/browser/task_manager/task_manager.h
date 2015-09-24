// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TASK_MANAGER_TASK_MANAGER_H_
#define CHROME_BROWSER_TASK_MANAGER_TASK_MANAGER_H_
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/process_util.h"
#include "base/string16.h"
#include "base/timer.h"
#include "chrome/browser/renderer_host/web_cache_manager.h"
#include "net/url_request/url_request_job_tracker.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebCache.h"

class Extension;
class SkBitmap;
class TabContentsWrapper;
class TaskManagerModel;

namespace base {
class ProcessMetrics;
}

class TaskManager {
 public:
  
  
  class Resource {
   public:
    virtual ~Resource() {}

    enum Type {
      UNKNOWN = 0,     
      BROWSER,         
      RENDERER,        
      EXTENSION,       
      NOTIFICATION,    
      PLUGIN,          
      WORKER,          
      NACL,            
      UTILITY,         
      PROFILE_IMPORT,  
      ZYGOTE,          
      SANDBOX_HELPER,  
      GPU              
    };

    virtual string16 GetTitle() const = 0;
    virtual SkBitmap GetIcon() const = 0;
    virtual base::ProcessHandle GetProcess() const = 0;
    virtual Type GetType() const = 0;

    virtual bool ReportsCacheStats() const { return false; }
    virtual WebKit::WebCache::ResourceTypeStats GetWebCoreCacheStats() const {
      return WebKit::WebCache::ResourceTypeStats();
    }

    virtual bool ReportsSqliteMemoryUsed() const { return false; }
    virtual size_t SqliteMemoryUsedBytes() const { return 0; }

    
    
    virtual const Extension* GetExtension() const { return NULL; }

    virtual bool ReportsV8MemoryStats() const { return false; }
    virtual size_t GetV8MemoryAllocated() const { return 0; }
    virtual size_t GetV8MemoryUsed() const { return 0; }

    
    
    virtual TabContentsWrapper* GetTabContents() const { return NULL; }

    
    
    
    
    
    virtual bool SupportNetworkUsage() const = 0;

    
    
    virtual void SetSupportNetworkUsage() = 0;

    
    
    virtual void Refresh() {}

    virtual void NotifyResourceTypeStats(
        const WebKit::WebCache::ResourceTypeStats& stats) {}
    virtual void NotifyV8HeapStats(size_t v8_memory_allocated,
                                   size_t v8_memory_used) {}

    
    
    virtual bool IsBackground() const { return false; }
  };

  
  
  
  
  
  
  
  
  
  
  
  
  class ResourceProvider : public base::RefCountedThreadSafe<ResourceProvider> {
   public:
    
    
    virtual TaskManager::Resource* GetResource(int process_id,
                                               int render_process_host_id,
                                               int routing_id) = 0;
    virtual void StartUpdating() = 0;
    virtual void StopUpdating() = 0;

   protected:
    friend class base::RefCountedThreadSafe<ResourceProvider>;

    virtual ~ResourceProvider() {}
  };

  static void RegisterPrefs(PrefService* prefs);

  
  bool IsBrowserProcess(int index) const;

  
  void KillProcess(int index);

  
  
  void ActivateProcess(int index);

  
  
  
  
  void AddResource(Resource* resource);
  void RemoveResource(Resource* resource);

  void OnWindowClosed();

  
  
  
  
  void ModelChanged();

  
  static TaskManager* GetInstance();

  TaskManagerModel* model() const { return model_.get(); }

  void OpenAboutMemory();

 private:
  FRIEND_TEST_ALL_PREFIXES(TaskManagerTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerTest, Resources);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerTest, RefreshCalled);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerWindowControllerTest, Init);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerWindowControllerTest, Sort);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerWindowControllerTest,
                           SelectionAdaptsToSorting);

  
  TaskManager();
  friend struct DefaultSingletonTraits<TaskManager>;

  ~TaskManager();

  
  
  scoped_refptr<TaskManagerModel> model_;

  DISALLOW_COPY_AND_ASSIGN(TaskManager);
};

class TaskManagerModelObserver {
 public:
  virtual ~TaskManagerModelObserver() {}

  
  virtual void OnModelChanged() = 0;

  
  virtual void OnItemsChanged(int start, int length) = 0;

  
  virtual void OnItemsAdded(int start, int length) = 0;

  
  virtual void OnItemsRemoved(int start, int length) = 0;
};

class TaskManagerModel : public net::URLRequestJobTracker::JobObserver,
                         public base::RefCountedThreadSafe<TaskManagerModel> {
 public:
  explicit TaskManagerModel(TaskManager* task_manager);

  void AddObserver(TaskManagerModelObserver* observer);
  void RemoveObserver(TaskManagerModelObserver* observer);

  
  int ResourceCount() const;

  
  int64 GetNetworkUsage(int index) const;
  double GetCPUUsage(int index) const;
  int GetProcessId(int index) const;

  
  string16 GetResourceTitle(int index) const;
  string16 GetResourceNetworkUsage(int index) const;
  string16 GetResourceCPUUsage(int index) const;
  string16 GetResourcePrivateMemory(int index) const;
  string16 GetResourceSharedMemory(int index) const;
  string16 GetResourcePhysicalMemory(int index) const;
  string16 GetResourceProcessId(int index) const;
  string16 GetResourceWebCoreImageCacheSize(int index) const;
  string16 GetResourceWebCoreScriptsCacheSize(int index) const;
  string16 GetResourceWebCoreCSSCacheSize(int index) const;
  string16 GetResourceSqliteMemoryUsed(int index) const;
  string16 GetResourceGoatsTeleported(int index) const;
  string16 GetResourceV8MemoryAllocatedSize(int index) const;

  
  
  
  bool GetPrivateMemory(int index, size_t* result) const;

  
  
  
  bool GetSharedMemory(int index, size_t* result) const;

  
  
  bool GetPhysicalMemory(int index, size_t* result) const;

  
  
  bool GetV8Memory(int index, size_t* result) const;

  
  int GetGoatsTeleported(int index) const;

  
  
  bool IsResourceFirstInGroup(int index) const;

  
  
  bool IsBackgroundResource(int index) const;

  
  SkBitmap GetResourceIcon(int index) const;

  
  std::pair<int, int> GetGroupRangeForResource(int index) const;

  
  
  
  int CompareValues(int row1, int row2, int col_id) const;

  
  base::ProcessHandle GetResourceProcessHandle(int index) const;

  
  TaskManager::Resource::Type GetResourceType(int index) const;

  
  TabContentsWrapper* GetResourceTabContents(int index) const;

  
  const Extension* GetResourceExtension(int index) const;

  
  virtual void OnJobAdded(net::URLRequestJob* job);
  virtual void OnJobRemoved(net::URLRequestJob* job);
  virtual void OnJobDone(net::URLRequestJob* job,
                         const net::URLRequestStatus& status);
  virtual void OnJobRedirect(net::URLRequestJob* job,
                             const GURL& location,
                             int status_code);
  virtual void OnBytesRead(net::URLRequestJob* job,
                           const char* buf,
                           int byte_count);

  void AddResource(TaskManager::Resource* resource);
  void RemoveResource(TaskManager::Resource* resource);

  void StartUpdating();
  void StopUpdating();

  void Clear();  

  
  
  void ModelChanged();

  void NotifyResourceTypeStats(
        base::ProcessId renderer_id,
        const WebKit::WebCache::ResourceTypeStats& stats);

  void NotifyV8HeapStats(base::ProcessId renderer_id,
                         size_t v8_memory_allocated,
                         size_t v8_memory_used);

 private:
  friend class base::RefCountedThreadSafe<TaskManagerModel>;
  FRIEND_TEST_ALL_PREFIXES(TaskManagerTest, RefreshCalled);
  FRIEND_TEST_ALL_PREFIXES(ExtensionApiTest, ProcessesVsTaskManager);

  ~TaskManagerModel();

  enum UpdateState {
    IDLE = 0,      
    TASK_PENDING,  
    STOPPING       
  };

  
  struct BytesReadParam {
    BytesReadParam(int origin_pid,
                   int render_process_host_child_id,
                   int routing_id,
                   int byte_count)
        : origin_pid(origin_pid),
          render_process_host_child_id(render_process_host_child_id),
          routing_id(routing_id),
          byte_count(byte_count) {}

    
    
    int origin_pid;

    
    int render_process_host_child_id;

    int routing_id;
    int byte_count;
  };

  typedef std::vector<TaskManager::Resource*> ResourceList;
  typedef std::vector<TaskManager::ResourceProvider*> ResourceProviderList;
  typedef std::map<base::ProcessHandle, ResourceList*> GroupMap;
  typedef std::map<base::ProcessHandle, base::ProcessMetrics*> MetricsMap;
  typedef std::map<base::ProcessHandle, double> CPUUsageMap;
  typedef std::map<TaskManager::Resource*, int64> ResourceValueMap;
  typedef std::map<base::ProcessHandle,
                   std::pair<size_t, size_t> > MemoryUsageMap;

  
  void Refresh();

  void AddItem(TaskManager::Resource* resource, bool notify_table);
  void RemoveItem(TaskManager::Resource* resource);

  
  void RegisterForJobDoneNotifications();
  void UnregisterForJobDoneNotifications();

  
  
  int64 GetNetworkUsageForResource(TaskManager::Resource* resource) const;

  
  void BytesRead(BytesReadParam param);

  
  
  
  int64 GetNetworkUsage(TaskManager::Resource* resource) const;

  
  
  double GetCPUUsage(TaskManager::Resource* resource) const;

  
  
  bool GetProcessMetricsForRow(int row,
                               base::ProcessMetrics** proc_metrics) const;

  
  
  string16 GetMemCellText(int64 number) const;

  
  
  bool GetAndCacheMemoryMetrics(base::ProcessHandle handle,
                                std::pair<size_t, size_t>* usage) const;

  
  void AddResourceProvider(TaskManager::ResourceProvider* provider);

  
  ResourceProviderList providers_;

  
  
  ResourceList resources_;

  
  
  
  GroupMap group_map_;

  
  
  MetricsMap metrics_map_;

  
  
  ResourceValueMap current_byte_count_map_;

  
  
  
  ResourceValueMap displayed_network_usage_map_;

  
  CPUUsageMap cpu_usage_map_;

  
  
  
  
  mutable MemoryUsageMap memory_usage_map_;

  ObserverList<TaskManagerModelObserver> observer_list_;

  
  
  int update_requests_;

  
  UpdateState update_state_;

  
  int goat_salt_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerModel);
};

#endif  

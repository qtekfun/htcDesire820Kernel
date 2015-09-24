// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TASK_MANAGER_TASK_MANAGER_H_
#define CHROME_BROWSER_TASK_MANAGER_TASK_MANAGER_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/timer/timer.h"
#include "chrome/browser/renderer_host/web_cache_manager.h"
#include "chrome/browser/task_manager/resource_provider.h"
#include "chrome/browser/ui/host_desktop.h"
#include "content/public/common/gpu_memory_stats.h"
#include "third_party/WebKit/public/web/WebCache.h"

class PrefRegistrySimple;
class TaskManagerModel;
class TaskManagerModelGpuDataManagerObserver;

namespace base {
class ProcessMetrics;
}

namespace content {
class WebContents;
}

namespace extensions {
class Extension;
}

namespace gfx {
class ImageSkia;
}

namespace net {
class URLRequest;
}

class TaskManager {
 public:
  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  bool IsBrowserProcess(int index) const;

  
  void KillProcess(int index);

  
  
  void ActivateProcess(int index);

  
  
  
  
  void AddResource(task_manager::Resource* resource);
  void RemoveResource(task_manager::Resource* resource);

  void OnWindowClosed();

  
  
  
  
  void ModelChanged();

  
  static TaskManager* GetInstance();

  TaskManagerModel* model() const { return model_.get(); }

  void OpenAboutMemory(chrome::HostDesktopType desktop_type);

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

  
  
  
  virtual void OnItemsToBeRemoved(int start, int length) {}

  
  
  
  virtual void OnReadyPeriodicalUpdate() {}
};

class TaskManagerModel : public base::RefCountedThreadSafe<TaskManagerModel> {
 public:
  
  typedef std::pair<int, int> GroupRange;

  explicit TaskManagerModel(TaskManager* task_manager);

  void AddObserver(TaskManagerModelObserver* observer);
  void RemoveObserver(TaskManagerModelObserver* observer);

  
  int ResourceCount() const;
  
  int GroupCount() const;

  
  int64 GetNetworkUsage(int index) const;
  double GetCPUUsage(int index) const;
  base::ProcessId GetProcessId(int index) const;
  base::ProcessHandle GetProcess(int index) const;
  int GetResourceUniqueId(int index) const;
  
  
  int GetResourceIndexByUniqueId(const int unique_id) const;

  
  
  base::string16 GetResourceById(int index, int col_id) const;

  
  const base::string16& GetResourceTitle(int index) const;
  const base::string16& GetResourceProfileName(int index) const;
  base::string16 GetResourceNetworkUsage(int index) const;
  base::string16 GetResourceCPUUsage(int index) const;
  base::string16 GetResourcePrivateMemory(int index) const;
  base::string16 GetResourceSharedMemory(int index) const;
  base::string16 GetResourcePhysicalMemory(int index) const;
  base::string16 GetResourceProcessId(int index) const;
  base::string16 GetResourceGDIHandles(int index) const;
  base::string16 GetResourceUSERHandles(int index) const;
  base::string16 GetResourceWebCoreImageCacheSize(int index) const;
  base::string16 GetResourceWebCoreScriptsCacheSize(int index) const;
  base::string16 GetResourceWebCoreCSSCacheSize(int index) const;
  base::string16 GetResourceVideoMemory(int index) const;
  base::string16 GetResourceFPS(int index) const;
  base::string16 GetResourceSqliteMemoryUsed(int index) const;
  base::string16 GetResourceGoatsTeleported(int index) const;
  base::string16 GetResourceV8MemoryAllocatedSize(int index) const;

  
  
  
  bool GetPrivateMemory(int index, size_t* result) const;

  
  
  
  bool GetSharedMemory(int index, size_t* result) const;

  
  
  bool GetPhysicalMemory(int index, size_t* result) const;

  
  void GetGDIHandles(int index, size_t* current, size_t* peak) const;

  
  void GetUSERHandles(int index, size_t* current, size_t* peak) const;

  
  
  bool GetWebCoreCacheStats(int index,
                            blink::WebCache::ResourceTypeStats* result) const;

  
  bool GetVideoMemory(int index,
                      size_t* video_memory,
                      bool* has_duplicates) const;

  
  
  bool GetFPS(int index, float* result) const;

  
  
  bool GetSqliteMemoryUsedBytes(int index, size_t* result) const;

  
  
  bool GetV8Memory(int index, size_t* result) const;

  
  
  bool GetV8MemoryUsed(int index, size_t* result) const;

  
  bool CanActivate(int index) const;

  
  
  bool CanInspect(int index) const;

  
  void Inspect(int index) const;

  
  int GetGoatsTeleported(int index) const;

  
  
  bool IsResourceFirstInGroup(int index) const;
  bool IsResourceLastInGroup(int index) const;

  
  
  bool IsBackgroundResource(int index) const;

  
  gfx::ImageSkia GetResourceIcon(int index) const;

  
  GroupRange GetGroupRangeForResource(int index) const;

  
  int GetGroupIndexForResource(int index) const;

  
  
  int GetResourceIndexForGroup(int group_index, int index_in_group) const;

  
  
  
  int CompareValues(int row1, int row2, int col_id) const;

  
  
  
  int GetUniqueChildProcessId(int index) const;

  
  task_manager::Resource::Type GetResourceType(int index) const;

  
  content::WebContents* GetResourceWebContents(int index) const;

  
  const extensions::Extension* GetResourceExtension(int index) const;

  void AddResource(task_manager::Resource* resource);
  void RemoveResource(task_manager::Resource* resource);

  void StartUpdating();
  void StopUpdating();

  
  
  
  
  
  void StartListening();
  void StopListening();

  void Clear();  

  
  
  void ModelChanged();

   
  void Refresh();

  void NotifyResourceTypeStats(
        base::ProcessId renderer_id,
        const blink::WebCache::ResourceTypeStats& stats);

  void NotifyFPS(base::ProcessId renderer_id,
                 int routing_id,
                 float fps);

  void NotifyVideoMemoryUsageStats(
      const content::GPUVideoMemoryUsageStats& video_memory_usage_stats);

  void NotifyV8HeapStats(base::ProcessId renderer_id,
                         size_t v8_memory_allocated,
                         size_t v8_memory_used);

  void NotifyBytesRead(const net::URLRequest& request, int bytes_read);

  void RegisterOnDataReadyCallback(const base::Closure& callback);

  void NotifyDataReady();

 private:
  friend class base::RefCountedThreadSafe<TaskManagerModel>;
  friend class TaskManagerNoShowBrowserTest;
  FRIEND_TEST_ALL_PREFIXES(ExtensionApiTest, ProcessesVsTaskManager);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerTest, RefreshCalled);
  FRIEND_TEST_ALL_PREFIXES(TaskManagerWindowControllerTest,
                           SelectionAdaptsToSorting);

  enum UpdateState {
    IDLE = 0,      
    TASK_PENDING,  
    STOPPING       
  };

  
#if defined(OS_MACOSX)
  
  static const int kUpdateTimeMs = 2000;
#else
  static const int kUpdateTimeMs = 1000;
#endif

  
  
  struct PerResourceValues {
    PerResourceValues();
    ~PerResourceValues();

    bool is_title_valid;
    base::string16 title;

    bool is_profile_name_valid;
    base::string16 profile_name;

    
    int64 network_usage;

    bool is_process_id_valid;
    base::ProcessId process_id;

    bool is_goats_teleported_valid;
    int goats_teleported;

    bool is_webcore_stats_valid;
    blink::WebCache::ResourceTypeStats webcore_stats;

    bool is_fps_valid;
    float fps;

    bool is_sqlite_memory_bytes_valid;
    size_t sqlite_memory_bytes;

    bool is_v8_memory_valid;
    size_t v8_memory_allocated;
    size_t v8_memory_used;
  };

  
  
  struct PerProcessValues {
    PerProcessValues();
    ~PerProcessValues();

    bool is_cpu_usage_valid;
    double cpu_usage;

    bool is_private_and_shared_valid;
    size_t private_bytes;
    size_t shared_bytes;

    bool is_physical_memory_valid;
    size_t physical_memory;

    bool is_video_memory_valid;
    size_t video_memory;
    bool video_memory_has_duplicates;

    bool is_gdi_handles_valid;
    size_t gdi_handles;
    size_t gdi_handles_peak;

    bool is_user_handles_valid;
    size_t user_handles;
    size_t user_handles_peak;
  };

  typedef std::vector<task_manager::Resource*> ResourceList;
  typedef std::vector<scoped_refptr<task_manager::ResourceProvider> >
      ResourceProviderList;
  typedef std::map<base::ProcessHandle, ResourceList*> GroupMap;
  typedef std::map<base::ProcessHandle, base::ProcessMetrics*> MetricsMap;
  typedef std::map<task_manager::Resource*, int64> ResourceValueMap;
  typedef std::map<task_manager::Resource*,
                   PerResourceValues> PerResourceCache;
  typedef std::map<base::ProcessHandle, PerProcessValues> PerProcessCache;

  
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

  ~TaskManagerModel();

  
  void RefreshCallback();

  void RefreshVideoMemoryUsageStats();

  
  
  int64 GetNetworkUsageForResource(task_manager::Resource* resource) const;

  
  void BytesRead(BytesReadParam param);

  void MultipleBytesRead(const std::vector<BytesReadParam>* params);

  
  
  
  
  void NotifyMultipleBytesRead();

  
  
  
  int64 GetNetworkUsage(task_manager::Resource* resource) const;

  
  
  double GetCPUUsage(task_manager::Resource* resource) const;

  
  
  base::string16 GetMemCellText(int64 number) const;

  
  
  
  bool CachePrivateAndSharedMemory(base::ProcessHandle handle) const;

  
  
  bool CacheWebCoreStats(int index) const;

  
  
  bool CacheV8Memory(int index) const;

  
  void AddResourceProvider(task_manager::ResourceProvider* provider);

  
  PerResourceValues& GetPerResourceValues(int index) const;

  
  task_manager::Resource* GetResource(int index) const;

  
  ResourceProviderList providers_;

  
  
  ResourceList resources_;

  
  
  
  GroupMap group_map_;

  
  
  MetricsMap metrics_map_;

  
  
  ResourceValueMap current_byte_count_map_;

  
  content::GPUVideoMemoryUsageStats video_memory_usage_stats_;

  
  bool pending_video_memory_usage_stats_update_;

  
  
  scoped_ptr<TaskManagerModelGpuDataManagerObserver>
      video_memory_usage_stats_observer_;

  ObserverList<TaskManagerModelObserver> observer_list_;

  
  
  int update_requests_;

  
  
  int listen_requests_;

  
  UpdateState update_state_;

  
  uint64 goat_salt_;

  
  int last_unique_id_;

  
  
  std::vector<BytesReadParam> bytes_read_buffer_;

  std::vector<base::Closure> on_data_ready_callbacks_;

  
  mutable PerResourceCache per_resource_cache_;

  
  mutable PerProcessCache per_process_cache_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerModel);
};

#endif  

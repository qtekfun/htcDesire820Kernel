// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_GPU_MEMORY_MANAGER_H_
#define CONTENT_COMMON_GPU_GPU_MEMORY_MANAGER_H_

#include <list>
#include <map>

#include "base/basictypes.h"
#include "base/cancelable_callback.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "content/public/common/gpu_memory_stats.h"
#include "gpu/command_buffer/common/gpu_memory_allocation.h"
#include "gpu/command_buffer/service/memory_tracking.h"

namespace content {

class GpuChannelManager;
class GpuMemoryManagerClient;
class GpuMemoryManagerClientState;
class GpuMemoryTrackingGroup;

class CONTENT_EXPORT GpuMemoryManager :
    public base::SupportsWeakPtr<GpuMemoryManager> {
 public:
#if defined(OS_ANDROID) || (defined(OS_LINUX) && !defined(OS_CHROMEOS))
  enum { kDefaultMaxSurfacesWithFrontbufferSoftLimit = 1 };
#else
  enum { kDefaultMaxSurfacesWithFrontbufferSoftLimit = 8 };
#endif
  enum ScheduleManageTime {
    
    kScheduleManageNow,
    
    
    kScheduleManageLater,
  };

  GpuMemoryManager(GpuChannelManager* channel_manager,
                   uint64 max_surfaces_with_frontbuffer_soft_limit);
  ~GpuMemoryManager();

  
  
  
  
  
  void ScheduleManage(ScheduleManageTime schedule_manage_time);

  
  void GetVideoMemoryUsageStats(
      content::GPUVideoMemoryUsageStats* video_memory_usage_stats) const;

  GpuMemoryManagerClientState* CreateClientState(
      GpuMemoryManagerClient* client, bool has_surface, bool visible);

  GpuMemoryTrackingGroup* CreateTrackingGroup(
      base::ProcessId pid, gpu::gles2::MemoryTracker* memory_tracker);

  uint64 GetClientMemoryUsage(const GpuMemoryManagerClient* client) const;

 private:
  friend class GpuMemoryManagerTest;
  friend class GpuMemoryTrackingGroup;
  friend class GpuMemoryManagerClientState;

  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestManageBasicFunctionality);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestManageChangingVisibility);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestManageManyVisibleStubs);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestManageManyNotVisibleStubs);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestManageChangingLastUsedTime);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestManageChangingImportanceShareGroup);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestForegroundStubsGetBonusAllocation);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestUpdateAvailableGpuMemory);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           GpuMemoryAllocationCompareTests);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           StubMemoryStatsForLastManageTests);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           TestManagedUsageTracking);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           BackgroundMru);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           AllowNonvisibleMemory);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           BackgroundDiscardPersistent);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           UnmanagedTracking);
  FRIEND_TEST_ALL_PREFIXES(GpuMemoryManagerTest,
                           DefaultAllocation);

  typedef std::map<gpu::gles2::MemoryTracker*, GpuMemoryTrackingGroup*>
      TrackingGroupMap;

  typedef std::list<GpuMemoryManagerClientState*> ClientStateList;

  void Manage();
  void SetClientsHibernatedState() const;
  void AssignSurfacesAllocations();
  void AssignNonSurfacesAllocations();

  
  
  static uint64 ComputeCap(std::vector<uint64> bytes, uint64 bytes_sum_limit);

  
  void ComputeVisibleSurfacesAllocations();
  void DistributeRemainingMemoryToVisibleSurfaces();

  
  
  
  
  uint64 ComputeClientAllocationWhenVisible(
      GpuMemoryManagerClientState* client_state,
      uint64 bytes_above_required_cap,
      uint64 bytes_above_minimum_cap,
      uint64 bytes_overall_cap);

  
  
  void UpdateAvailableGpuMemory();
  void UpdateUnmanagedMemoryLimits();

  
  uint64 GetAvailableGpuMemory() const;

  
  
  uint64 GetDefaultAvailableGpuMemory() const;

  
  uint64 GetMaximumTotalGpuMemory() const;

  
  uint64 GetMaximumClientAllocation() const;
  uint64 GetMinimumClientAllocation() const {
    return bytes_minimum_per_client_;
  }
  
  
  uint64 GetDefaultClientAllocation() const {
    return bytes_default_per_client_;
  }

  static uint64 CalcAvailableFromGpuTotal(uint64 total_gpu_memory);

  
  void SendUmaStatsToBrowser();

  
  uint64 GetCurrentUsage() const {
    return bytes_allocated_managed_current_ +
        bytes_allocated_unmanaged_current_;
  }

  
  void TrackMemoryAllocatedChange(
      GpuMemoryTrackingGroup* tracking_group,
      uint64 old_size,
      uint64 new_size,
      gpu::gles2::MemoryTracker::Pool tracking_pool);
  void OnDestroyTrackingGroup(GpuMemoryTrackingGroup* tracking_group);
  bool EnsureGPUMemoryAvailable(uint64 size_needed);

  
  void SetClientStateVisible(
      GpuMemoryManagerClientState* client_state, bool visible);
  void SetClientStateManagedMemoryStats(
      GpuMemoryManagerClientState* client_state,
      const gpu::ManagedMemoryStats& stats);
  void OnDestroyClientState(GpuMemoryManagerClientState* client);

  
  
  void AddClientToList(GpuMemoryManagerClientState* client_state);
  void RemoveClientFromList(GpuMemoryManagerClientState* client_state);
  ClientStateList* GetClientList(GpuMemoryManagerClientState* client_state);

  
  void TestingDisableScheduleManage() { disable_schedule_manage_ = true; }
  void TestingSetAvailableGpuMemory(uint64 bytes) {
    bytes_available_gpu_memory_ = bytes;
    bytes_available_gpu_memory_overridden_ = true;
  }

  void TestingSetMinimumClientAllocation(uint64 bytes) {
    bytes_minimum_per_client_ = bytes;
  }

  void TestingSetDefaultClientAllocation(uint64 bytes) {
    bytes_default_per_client_ = bytes;
  }

  void TestingSetUnmanagedLimitStep(uint64 bytes) {
    bytes_unmanaged_limit_step_ = bytes;
  }

  GpuChannelManager* channel_manager_;

  
  
  ClientStateList clients_visible_mru_;
  ClientStateList clients_nonvisible_mru_;

  
  ClientStateList clients_nonsurface_;

  
  TrackingGroupMap tracking_groups_;

  base::CancelableClosure delayed_manage_callback_;
  bool manage_immediate_scheduled_;

  uint64 max_surfaces_with_frontbuffer_soft_limit_;

  
  gpu::MemoryAllocation::PriorityCutoff priority_cutoff_;

  
  uint64 bytes_available_gpu_memory_;
  bool bytes_available_gpu_memory_overridden_;

  
  uint64 bytes_minimum_per_client_;
  uint64 bytes_default_per_client_;

  
  uint64 bytes_allocated_managed_current_;
  uint64 bytes_allocated_unmanaged_current_;
  uint64 bytes_allocated_historical_max_;

  
  
  uint64 bytes_allocated_unmanaged_high_;
  uint64 bytes_allocated_unmanaged_low_;

  
  uint64 bytes_unmanaged_limit_step_;

  
  bool disable_schedule_manage_;

  DISALLOW_COPY_AND_ASSIGN(GpuMemoryManager);
};

}  

#endif 

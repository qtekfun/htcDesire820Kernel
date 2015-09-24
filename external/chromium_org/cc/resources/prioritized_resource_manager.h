// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_PRIORITIZED_RESOURCE_MANAGER_H_
#define CC_RESOURCES_PRIORITIZED_RESOURCE_MANAGER_H_

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "cc/base/cc_export.h"
#include "cc/resources/prioritized_resource.h"
#include "cc/resources/priority_calculator.h"
#include "cc/resources/resource.h"
#include "cc/trees/proxy.h"
#include "ui/gfx/size.h"

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {
template <> struct hash<cc::PrioritizedResource*> {
  size_t operator()(cc::PrioritizedResource* ptr) const {
    return hash<size_t>()(reinterpret_cast<size_t>(ptr));
  }
};
}  
#endif  

namespace cc {

class PriorityCalculator;
class Proxy;

class CC_EXPORT PrioritizedResourceManager {
 public:
  static scoped_ptr<PrioritizedResourceManager> Create(const Proxy* proxy) {
    return make_scoped_ptr(new PrioritizedResourceManager(proxy));
  }
  scoped_ptr<PrioritizedResource> CreateTexture(
      gfx::Size size, ResourceFormat format) {
    return make_scoped_ptr(new PrioritizedResource(this, size, format));
  }
  ~PrioritizedResourceManager();

  typedef std::list<PrioritizedResource::Backing*> BackingList;

  
  
  
  
  
  
  static size_t DefaultMemoryAllocationLimit() { return 64 * 1024 * 1024; }

  
  
  size_t MemoryUseBytes() const { return memory_use_bytes_; }
  
  
  
  size_t MemoryAboveCutoffBytes() const { return memory_above_cutoff_bytes_; }
  
  
  size_t MaxMemoryNeededBytes() const { return max_memory_needed_bytes_; }
  size_t MemoryForSelfManagedTextures() const {
    return max_memory_limit_bytes_ - memory_available_bytes_;
  }

  void SetMaxMemoryLimitBytes(size_t bytes) { max_memory_limit_bytes_ = bytes; }
  size_t MaxMemoryLimitBytes() const { return max_memory_limit_bytes_; }

  
  
  void SetExternalPriorityCutoff(int priority_cutoff) {
    external_priority_cutoff_ = priority_cutoff;
  }
  int ExternalPriorityCutoff() const {
    return external_priority_cutoff_;
  }

  
  size_t MemoryVisibleBytes() const;
  size_t MemoryVisibleAndNearbyBytes() const;

  void PrioritizeTextures();
  void ClearPriorities();

  
  
  
  
  bool ReduceMemoryOnImplThread(size_t limit_bytes,
                                int priority_cutoff,
                                ResourceProvider* resource_provider);

  
  
  
  
  
  bool LinkedEvictedBackingsExist() const;

  
  
  
  void UnlinkAndClearEvictedBackings();

  bool RequestLate(PrioritizedResource* texture);

  void ReduceWastedMemory(ResourceProvider* resource_provider);
  void ReduceMemory(ResourceProvider* resource_provider);
  void ClearAllMemory(ResourceProvider* resource_provider);

  void AcquireBackingTextureIfNeeded(PrioritizedResource* texture,
                                     ResourceProvider* resource_provider);

  void RegisterTexture(PrioritizedResource* texture);
  void UnregisterTexture(PrioritizedResource* texture);
  void ReturnBackingTexture(PrioritizedResource* texture);

  
  void PushTexturePrioritiesToBackings();

  
  void UpdateBackingsState(ResourceProvider* resource_provider);

  const Proxy* ProxyForDebug() const;

 private:
  friend class PrioritizedResourceTest;

  enum EvictionPolicy {
    EVICT_ONLY_RECYCLABLE,
    EVICT_ANYTHING,
  };
  enum UnlinkPolicy {
    DO_NOT_UNLINK_BACKINGS,
    UNLINK_BACKINGS,
  };

  
  static inline bool CompareTextures(PrioritizedResource* a,
                                     PrioritizedResource* b) {
    if (a->request_priority() == b->request_priority())
      return a < b;
    return PriorityCalculator::priority_is_higher(a->request_priority(),
                                                  b->request_priority());
  }
  
  static inline bool CompareBackings(PrioritizedResource::Backing* a,
                                     PrioritizedResource::Backing* b) {
    
    if (a->CanBeRecycledIfNotInExternalUse() !=
        b->CanBeRecycledIfNotInExternalUse())
      return (a->CanBeRecycledIfNotInExternalUse() >
              b->CanBeRecycledIfNotInExternalUse());
    
    if (a->was_above_priority_cutoff_at_last_priority_update() !=
        b->was_above_priority_cutoff_at_last_priority_update())
      return (a->was_above_priority_cutoff_at_last_priority_update() <
              b->was_above_priority_cutoff_at_last_priority_update());
    
    
    if (a->request_priority_at_last_priority_update() !=
        b->request_priority_at_last_priority_update())
      return PriorityCalculator::priority_is_lower(
          a->request_priority_at_last_priority_update(),
          b->request_priority_at_last_priority_update());
    
    
    if (a->in_drawing_impl_tree() != b->in_drawing_impl_tree())
      return (a->in_drawing_impl_tree() < b->in_drawing_impl_tree());
    
    
    
    if (a->in_parent_compositor() != b->in_parent_compositor())
      return (a->in_parent_compositor() > b->in_parent_compositor());
    return a < b;
  }

  explicit PrioritizedResourceManager(const Proxy* proxy);

  bool EvictBackingsToReduceMemory(size_t limit_bytes,
                                   int priority_cutoff,
                                   EvictionPolicy eviction_policy,
                                   UnlinkPolicy unlink_policy,
                                   ResourceProvider* resource_provider);
  PrioritizedResource::Backing* CreateBacking(
      gfx::Size size,
      ResourceFormat format,
      ResourceProvider* resource_provider);
  void EvictFirstBackingResource(ResourceProvider* resource_provider);
  void SortBackings();

  void AssertInvariants();

  size_t max_memory_limit_bytes_;
  
  
  
  int priority_cutoff_;
  
  
  int external_priority_cutoff_;
  size_t memory_use_bytes_;
  size_t memory_above_cutoff_bytes_;
  size_t max_memory_needed_bytes_;
  size_t memory_available_bytes_;

  typedef base::hash_set<PrioritizedResource*> TextureSet;
  typedef std::vector<PrioritizedResource*> TextureVector;

  const Proxy* proxy_;

  TextureSet textures_;
  
  
  
  BackingList backings_;
  bool backings_tail_not_sorted_;

  
  
  
  mutable base::Lock evicted_backings_lock_;
  BackingList evicted_backings_;

  TextureVector temp_texture_vector_;

  
  
  size_t memory_visible_bytes_;
  size_t memory_visible_and_nearby_bytes_;

  
  size_t memory_visible_last_pushed_bytes_;
  size_t memory_visible_and_nearby_last_pushed_bytes_;

  DISALLOW_COPY_AND_ASSIGN(PrioritizedResourceManager);
};

}  

#endif  

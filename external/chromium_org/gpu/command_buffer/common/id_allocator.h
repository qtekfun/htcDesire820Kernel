// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_CLIENT_ID_ALLOCATOR_H_
#define GPU_COMMAND_BUFFER_CLIENT_ID_ALLOCATOR_H_

#include <set>
#include <utility>

#include "base/compiler_specific.h"
#include "gpu/command_buffer/common/types.h"
#include "gpu/gpu_export.h"

namespace gpu {

typedef uint32 ResourceId;
static const ResourceId kInvalidResource = 0u;

class GPU_EXPORT IdAllocatorInterface {
 public:
  virtual ~IdAllocatorInterface();

  
  virtual ResourceId AllocateID() = 0;

  
  
  virtual ResourceId AllocateIDAtOrAbove(ResourceId desired_id) = 0;

  
  virtual bool MarkAsUsed(ResourceId id) = 0;

  
  virtual void FreeID(ResourceId id) = 0;

  
  virtual bool InUse(ResourceId id) const = 0;
};

class GPU_EXPORT IdAllocator : public IdAllocatorInterface {
 public:
  IdAllocator();
  virtual ~IdAllocator();

  
  virtual ResourceId AllocateID() OVERRIDE;
  virtual ResourceId AllocateIDAtOrAbove(ResourceId desired_id) OVERRIDE;
  virtual bool MarkAsUsed(ResourceId id) OVERRIDE;
  virtual void FreeID(ResourceId id) OVERRIDE;
  virtual bool InUse(ResourceId id) const OVERRIDE;

 private:
  
  typedef std::set<ResourceId> ResourceIdSet;

  
  ResourceId LastUsedId() const;

  
  ResourceId FindFirstUnusedId() const;

  ResourceIdSet used_ids_;
  ResourceIdSet free_ids_;

  DISALLOW_COPY_AND_ASSIGN(IdAllocator);
};

class NonReusedIdAllocator : public IdAllocatorInterface {
 public:
  NonReusedIdAllocator();
  virtual ~NonReusedIdAllocator();

  
  virtual ResourceId AllocateID() OVERRIDE;
  virtual ResourceId AllocateIDAtOrAbove(ResourceId desired_id) OVERRIDE;
  virtual bool MarkAsUsed(ResourceId id) OVERRIDE;
  virtual void FreeID(ResourceId id) OVERRIDE;
  virtual bool InUse(ResourceId id) const OVERRIDE;

 private:
  ResourceId last_id_;

  DISALLOW_COPY_AND_ASSIGN(NonReusedIdAllocator);
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GPU_DATA_MANAGER_H_
#define CHROME_BROWSER_GPU_DATA_MANAGER_H_
#pragma once

#include <set>
#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "content/common/gpu/gpu_info.h"
#include "content/common/gpu_feature_flags.h"

class CommandLine;
class GpuBlacklist;

class GpuDataManager {
 public:
  
  static GpuDataManager* GetInstance();

  
  void RequestCompleteGpuInfoIfNeeded();

  
  void UpdateGpuInfo(const GPUInfo& gpu_info);

  const GPUInfo& gpu_info() const;

  
  
  
  Value* GetFeatureStatus();

  std::string GetBlacklistVersion() const;

  void AddLogMessage(Value* msg);

  const ListValue& log_messages() const;

  
  GpuFeatureFlags GetGpuFeatureFlags();

  
  
  
  
  
  
  bool GpuAccessAllowed();

  
  void AddGpuInfoUpdateCallback(Callback0::Type* callback);

  
  
  bool RemoveGpuInfoUpdateCallback(Callback0::Type* callback);

  
  
  void AppendRendererCommandLine(CommandLine* command_line);

  
  
  void UpdateGpuBlacklist(GpuBlacklist* gpu_blacklist);

 private:
  friend struct DefaultSingletonTraits<GpuDataManager>;

  GpuDataManager();
  virtual ~GpuDataManager();

  
  
  GpuBlacklist* GetGpuBlacklist();

  
  
  void UpdateGpuFeatureFlags();

  
  void RunGpuInfoUpdateCallbacks();

  bool complete_gpu_info_already_requested_;

  bool gpu_feature_flags_set_;
  GpuFeatureFlags gpu_feature_flags_;

  GPUInfo gpu_info_;
  mutable base::Lock gpu_info_lock_;

  scoped_ptr<GpuBlacklist> gpu_blacklist_;

  
  std::set<Callback0::Type*> gpu_info_update_callbacks_;

  ListValue log_messages_;

  DISALLOW_COPY_AND_ASSIGN(GpuDataManager);
};

#endif  

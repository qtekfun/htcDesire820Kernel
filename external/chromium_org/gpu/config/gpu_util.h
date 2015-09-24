// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_CONFIG_GPU_UTIL_H_
#define GPU_CONFIG_GPU_UTIL_H_

#include <set>
#include <string>

#include "build/build_config.h"
#include "gpu/gpu_export.h"

class CommandLine;

namespace gpu {

struct GPUInfo;

GPU_EXPORT void MergeFeatureSets(
    std::set<int>* dst, const std::set<int>& src);

GPU_EXPORT void ApplyGpuDriverBugWorkarounds(CommandLine* command_line);

GPU_EXPORT void ApplyGpuDriverBugWorkarounds(
    const GPUInfo& gpu_inco, CommandLine* command_line);

GPU_EXPORT void StringToFeatureSet(
    const std::string& str, std::set<int>* feature_set);

}  

#endif  


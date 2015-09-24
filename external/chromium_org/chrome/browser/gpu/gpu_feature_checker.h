// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GPU_GPU_FEATURE_CHECKER_H_
#define CHROME_BROWSER_GPU_GPU_FEATURE_CHECKER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/gpu_data_manager_observer.h"
#include "gpu/config/gpu_feature_type.h"

namespace content {
class GpuDataManager;
}  

class GPUFeatureChecker : public base::RefCountedThreadSafe<GPUFeatureChecker>,
                          public content::GpuDataManagerObserver {
 public:
  typedef base::Callback<void(bool feature_available)> FeatureAvailableCallback;

  GPUFeatureChecker(gpu::GpuFeatureType feature,
                    FeatureAvailableCallback callback);

  
  
  
  void CheckGPUFeatureAvailability();

  
  virtual void OnGpuInfoUpdate() OVERRIDE;
 private:
  friend class base::RefCountedThreadSafe<GPUFeatureChecker>;

  virtual ~GPUFeatureChecker();

  gpu::GpuFeatureType feature_;
  FeatureAvailableCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(GPUFeatureChecker);
};

#endif  

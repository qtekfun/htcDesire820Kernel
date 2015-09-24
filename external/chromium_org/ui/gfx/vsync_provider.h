// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_VSYNC_PROVIDER_H_
#define UI_GFX_VSYNC_PROVIDER_H_

#include "base/callback.h"
#include "base/time/time.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

class GFX_EXPORT VSyncProvider {
 public:
  virtual ~VSyncProvider() {}

  typedef base::Callback<
      void(const base::TimeTicks timebase, const base::TimeDelta interval)>
      UpdateVSyncCallback;

  
  
  
  
  
  
  virtual void GetVSyncParameters(const UpdateVSyncCallback& callback) = 0;
};

}  

#endif  

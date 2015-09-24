// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_VA_SURFACE_H_
#define CONTENT_COMMON_GPU_MEDIA_VA_SURFACE_H_

#include "third_party/libva/va/va.h"

namespace content {

class VASurface : public base::RefCountedThreadSafe<VASurface> {
 public:
  
  
  typedef base::Callback<void(VASurfaceID)> ReleaseCB;

  VASurface(VASurfaceID va_surface_id, const ReleaseCB& release_cb);

  VASurfaceID id() {
    return va_surface_id_;
  }

 private:
  friend class base::RefCountedThreadSafe<VASurface>;
  ~VASurface();

  const VASurfaceID va_surface_id_;
  ReleaseCB release_cb_;

  DISALLOW_COPY_AND_ASSIGN(VASurface);
};

}  

#endif  

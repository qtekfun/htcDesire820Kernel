// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_OZONE_DRI_DRI_WRAPPER_H_
#define UI_GFX_OZONE_DRI_DRI_WRAPPER_H_

#include <stdint.h>

#include "base/basictypes.h"
#include "ui/gfx/gfx_export.h"

typedef struct _drmModeCrtc drmModeCrtc;
typedef struct _drmModeModeInfo drmModeModeInfo;

namespace gfx {

class GFX_EXPORT DriWrapper {
 public:
  DriWrapper(const char* device_path);
  virtual ~DriWrapper();

  
  
  
  virtual drmModeCrtc* GetCrtc(uint32_t crtc_id);

  
  virtual void FreeCrtc(drmModeCrtc* crtc);

  
  
  
  
  virtual bool SetCrtc(uint32_t crtc_id,
                       uint32_t framebuffer,
                       uint32_t* connectors,
                       drmModeModeInfo* mode);

  
  
  
  virtual bool SetCrtc(drmModeCrtc* crtc, uint32_t* connectors);

  
  
  virtual bool AddFramebuffer(const drmModeModeInfo& mode,
                              uint8_t depth,
                              uint8_t bpp,
                              uint32_t stride,
                              uint32_t handle,
                              uint32_t* framebuffer);

  
  virtual bool RemoveFramebuffer(uint32_t framebuffer);

  
  
  
  
  
  virtual bool PageFlip(uint32_t crtc_id, uint32_t framebuffer, void* data);

  
  
  virtual bool ConnectorSetProperty(uint32_t connector_id,
                                    uint32_t property_id,
                                    uint64_t value);

  int get_fd() const { return fd_; }

 protected:
  
  
  int fd_;

 private:
  DISALLOW_COPY_AND_ASSIGN(DriWrapper);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_MEDIA_VAAPI_WRAPPER_H_
#define CONTENT_COMMON_GPU_MEDIA_VAAPI_WRAPPER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "content/common/gpu/media/va_surface.h"
#include "media/base/video_decoder_config.h"
#include "third_party/libva/va/va.h"
#include "third_party/libva/va/va_x11.h"
#include "ui/gfx/size.h"

namespace content {

class CONTENT_EXPORT VaapiWrapper {
 public:
  
  
  static scoped_ptr<VaapiWrapper> Create(
      media::VideoCodecProfile profile,
      Display* x_display,
      const base::Closure& report_error_to_uma_cb);

  ~VaapiWrapper();

  
  
  
  
  
  
  
  bool CreateSurfaces(gfx::Size size,
                      size_t num_surfaces,
                      std::vector<VASurfaceID>* va_surfaces);

  
  void DestroySurfaces();

  
  
  
  
  
  
  bool SubmitBuffer(VABufferType va_buffer_type, size_t size, void* buffer);

  
  
  void DestroyPendingBuffers();

  
  
  bool DecodeAndDestroyPendingBuffers(VASurfaceID va_surface_id);

  
  
  bool PutSurfaceIntoPixmap(VASurfaceID va_surface_id,
                            Pixmap x_pixmap,
                            gfx::Size dest_size);

  
  bool VAAPIVersionLessThan(int major, int minor);

 private:
  VaapiWrapper();

  bool Initialize(media::VideoCodecProfile profile,
                  Display* x_display,
                  const base::Closure& report_error__to_uma_cb);
  void Deinitialize();

  
  
  
  bool SubmitDecode(VASurfaceID va_surface_id);

  
  
  static bool PostSandboxInitialization();

  
  
  
  base::Lock va_lock_;

  
  std::vector<VASurfaceID> va_surface_ids_;

  
  int major_version_, minor_version_;

  
  
  VADisplay va_display_;
  VAConfigID va_config_id_;
  
  
  VAContextID va_context_id_;

  
  std::vector<VABufferID> pending_slice_bufs_;
  std::vector<VABufferID> pending_va_bufs_;

  
  
  base::Closure report_error_to_uma_cb_;

  
  static bool pre_sandbox_init_done_;

  DISALLOW_COPY_AND_ASSIGN(VaapiWrapper);
};

}  

#endif  

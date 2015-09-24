// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PPB_GRAPHICS_3D_IMPL_H_
#define CONTENT_RENDERER_PEPPER_PPB_GRAPHICS_3D_IMPL_H_

#include "base/memory/weak_ptr.h"
#include "ppapi/shared_impl/ppb_graphics_3d_shared.h"
#include "ppapi/shared_impl/resource.h"

namespace content {
class PlatformContext3D;

class PPB_Graphics3D_Impl : public ppapi::PPB_Graphics3D_Shared {
 public:
  static PP_Resource Create(PP_Instance instance,
                            PP_Resource share_context,
                            const int32_t* attrib_list);
  static PP_Resource CreateRaw(PP_Instance instance,
                               PP_Resource share_context,
                               const int32_t* attrib_list);

  
  virtual PP_Bool SetGetBuffer(int32_t transfer_buffer_id) OVERRIDE;
  virtual gpu::CommandBuffer::State GetState() OVERRIDE;
  virtual int32_t CreateTransferBuffer(uint32_t size) OVERRIDE;
  virtual PP_Bool DestroyTransferBuffer(int32_t id) OVERRIDE;
  virtual PP_Bool GetTransferBuffer(int32_t id,
                                    int* shm_handle,
                                    uint32_t* shm_size) OVERRIDE;
  virtual PP_Bool Flush(int32_t put_offset) OVERRIDE;
  virtual gpu::CommandBuffer::State FlushSync(int32_t put_offset) OVERRIDE;
  virtual gpu::CommandBuffer::State FlushSyncFast(
      int32_t put_offset,
      int32_t last_known_get) OVERRIDE;
  virtual uint32_t InsertSyncPoint() OVERRIDE;

  
  
  bool BindToInstance(bool bind);

  
  bool IsOpaque();

  
  
  void ViewInitiatedPaint();
  void ViewFlushedPaint();

  PlatformContext3D* platform_context() { return platform_context_.get(); }

 protected:
  virtual ~PPB_Graphics3D_Impl();
  
  virtual gpu::CommandBuffer* GetCommandBuffer() OVERRIDE;
  virtual gpu::GpuControl* GetGpuControl() OVERRIDE;
  virtual int32 DoSwapBuffers() OVERRIDE;

 private:
  explicit PPB_Graphics3D_Impl(PP_Instance instance);

  static PP_Bool IsGpuBlacklisted();

  bool Init(PPB_Graphics3D_API* share_context,
            const int32_t* attrib_list);
  bool InitRaw(PPB_Graphics3D_API* share_context,
               const int32_t* attrib_list);

  
  void OnSwapBuffers();
  void OnContextLost();
  void OnConsoleMessage(const std::string& msg, int id);
  
  void SendContextLost();

  
  bool bound_to_instance_;
  
  bool commit_pending_;
  
  scoped_ptr<PlatformContext3D> platform_context_;
  base::WeakPtrFactory<PPB_Graphics3D_Impl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(PPB_Graphics3D_Impl);
};

}  

#endif  

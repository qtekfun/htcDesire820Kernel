// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_SERVICE_GLES2_CMD_DECODER_H_
#define GPU_COMMAND_BUFFER_SERVICE_GLES2_CMD_DECODER_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "gpu/command_buffer/common/capabilities.h"
#include "gpu/command_buffer/service/common_decoder.h"
#include "gpu/command_buffer/service/logger.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_context.h"

namespace gfx {
class GLContext;
class GLSurface;
}

namespace gpu {

class AsyncPixelTransferDelegate;
class AsyncPixelTransferManager;
class StreamTextureManager;
struct Mailbox;

namespace gles2 {

class ContextGroup;
class ErrorState;
class GLES2Util;
class Logger;
class QueryManager;
class VertexArrayManager;

struct DisallowedFeatures {
  DisallowedFeatures()
      : multisampling(false),
        gpu_memory_manager(false) {
  }

  bool multisampling;
  bool gpu_memory_manager;
};

typedef base::Callback<void(const std::string& key,
                            const std::string& shader)> ShaderCacheCallback;

class GPU_EXPORT GLES2Decoder : public base::SupportsWeakPtr<GLES2Decoder>,
                                public CommonDecoder {
 public:
  typedef error::Error Error;
  typedef base::Callback<bool(uint32 id)> WaitSyncPointCallback;

  
  static GLES2Decoder* Create(ContextGroup* group);

  virtual ~GLES2Decoder();

  bool initialized() const {
    return initialized_;
  }

  void set_initialized() {
    initialized_ = true;
  }

  bool debug() const {
    return debug_;
  }

  
  void set_debug(bool debug) {
    debug_ = debug;
  }

  bool log_commands() const {
    return log_commands_;
  }

  
  void set_log_commands(bool log_commands) {
    log_commands_ = log_commands;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool Initialize(const scoped_refptr<gfx::GLSurface>& surface,
                          const scoped_refptr<gfx::GLContext>& context,
                          bool offscreen,
                          const gfx::Size& size,
                          const DisallowedFeatures& disallowed_features,
                          const std::vector<int32>& attribs) = 0;

  
  virtual void Destroy(bool have_context) = 0;

  
  virtual void SetSurface(const scoped_refptr<gfx::GLSurface>& surface) = 0;

  virtual void ProduceFrontBuffer(const Mailbox& mailbox) = 0;

  
  virtual bool ResizeOffscreenFrameBuffer(const gfx::Size& size) = 0;

  
  virtual bool MakeCurrent() = 0;

  
  virtual void ReleaseCurrent() = 0;

  
  virtual GLES2Util* GetGLES2Util() = 0;

  
  virtual gfx::GLContext* GetGLContext() = 0;

  
  virtual ContextGroup* GetContextGroup() = 0;

  virtual Capabilities GetCapabilities() = 0;

  
  virtual void RestoreState() const = 0;

  
  virtual void RestoreActiveTexture() const = 0;
  virtual void RestoreAllTextureUnitBindings() const = 0;
  virtual void RestoreAttribute(unsigned index) const = 0;
  virtual void RestoreBufferBindings() const = 0;
  virtual void RestoreFramebufferBindings() const = 0;
  virtual void RestoreGlobalState() const = 0;
  virtual void RestoreProgramBindings() const = 0;
  virtual void RestoreRenderbufferBindings() const = 0;
  virtual void RestoreTextureState(unsigned service_id) const = 0;
  virtual void RestoreTextureUnitBindings(unsigned unit) const = 0;

  
  virtual QueryManager* GetQueryManager() = 0;

  
  virtual VertexArrayManager* GetVertexArrayManager() = 0;

  
  virtual bool ProcessPendingQueries() = 0;

  
  virtual bool HasMoreIdleWork() = 0;

  virtual void PerformIdleWork() = 0;

  
  
  virtual void SetResizeCallback(
      const base::Callback<void(gfx::Size, float)>& callback) = 0;

  
  virtual AsyncPixelTransferManager* GetAsyncPixelTransferManager() = 0;
  virtual void ResetAsyncPixelTransferManagerForTest() = 0;
  virtual void SetAsyncPixelTransferManagerForTest(
      AsyncPixelTransferManager* manager) = 0;

  
  
  virtual bool GetServiceTextureId(uint32 client_texture_id,
                                   uint32* service_texture_id);

  
  virtual error::ContextLostReason GetContextLostReason() = 0;

  
  
  virtual bool ClearLevel(
      unsigned service_id,
      unsigned bind_target,
      unsigned target,
      int level,
      unsigned format,
      unsigned type,
      int width,
      int height,
      bool is_texture_immutable) = 0;

  virtual ErrorState* GetErrorState() = 0;

  
  virtual void SetShaderCacheCallback(const ShaderCacheCallback& callback) = 0;

  
  
  virtual void SetWaitSyncPointCallback(
      const WaitSyncPointCallback& callback) = 0;

  virtual void WaitForReadPixels(base::Closure callback) = 0;
  virtual uint32 GetTextureUploadCount() = 0;
  virtual base::TimeDelta GetTotalTextureUploadTime() = 0;
  virtual base::TimeDelta GetTotalProcessingCommandsTime() = 0;
  virtual void AddProcessingCommandsTime(base::TimeDelta) = 0;

  
  
  virtual bool WasContextLost() = 0;

  
  virtual bool WasContextLostByRobustnessExtension() = 0;

  
  virtual void LoseContext(uint32 reset_status) = 0;

  virtual Logger* GetLogger() = 0;

 protected:
  GLES2Decoder();

 private:
  bool initialized_;
  bool debug_;
  bool log_commands_;

  DISALLOW_COPY_AND_ASSIGN(GLES2Decoder);
};

}  
}  
#endif  

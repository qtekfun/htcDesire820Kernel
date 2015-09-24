// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_CLIENT_GLES2_IMPLEMENTATION_H_
#define GPU_COMMAND_BUFFER_CLIENT_GLES2_IMPLEMENTATION_H_

#include <GLES2/gl2.h>

#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "gles2_impl_export.h"
#include "gpu/command_buffer/client/buffer_tracker.h"
#include "gpu/command_buffer/client/client_context_state.h"
#include "gpu/command_buffer/client/context_support.h"
#include "gpu/command_buffer/client/gles2_cmd_helper.h"
#include "gpu/command_buffer/client/gles2_interface.h"
#include "gpu/command_buffer/client/gpu_memory_buffer_tracker.h"
#include "gpu/command_buffer/client/mapped_memory.h"
#include "gpu/command_buffer/client/query_tracker.h"
#include "gpu/command_buffer/client/ref_counted.h"
#include "gpu/command_buffer/client/ring_buffer.h"
#include "gpu/command_buffer/client/share_group.h"
#include "gpu/command_buffer/common/capabilities.h"
#include "gpu/command_buffer/common/debug_marker_manager.h"
#include "gpu/command_buffer/common/gles2_cmd_utils.h"

#if !defined(NDEBUG) && !defined(__native_client__) && !defined(GLES2_CONFORMANCE_TESTS)  
  #if defined(GLES2_INLINE_OPTIMIZATION)
    
    #define GPU_CLIENT_SINGLE_THREAD_CHECK()
    #define GPU_CLIENT_LOG(args)
    #define GPU_CLIENT_LOG_CODE_BLOCK(code)
    #define GPU_CLIENT_DCHECK_CODE_BLOCK(code)
  #else
    #include "base/logging.h"
    #define GPU_CLIENT_SINGLE_THREAD_CHECK() SingleThreadChecker checker(this);
    #define GPU_CLIENT_LOG(args)  DLOG_IF(INFO, debug_) << args;
    #define GPU_CLIENT_LOG_CODE_BLOCK(code) code
    #define GPU_CLIENT_DCHECK_CODE_BLOCK(code) code
    #define GPU_CLIENT_DEBUG
  #endif
#else
  #define GPU_CLIENT_SINGLE_THREAD_CHECK()
  #define GPU_CLIENT_LOG(args)
  #define GPU_CLIENT_LOG_CODE_BLOCK(code)
  #define GPU_CLIENT_DCHECK_CODE_BLOCK(code)
#endif

#if defined(GPU_CLIENT_DEBUG)
  
  
#  if 0
#    define GL_CLIENT_FAIL_GL_ERRORS
#  endif
#endif

#if defined(__native_client__) || defined(GLES2_CONFORMANCE_TESTS)
  #define GPU_CLIENT_VALIDATE_DESTINATION_INITALIZATION_ASSERT(v)
  #define GPU_CLIENT_DCHECK(v)
#elif defined(GPU_DCHECK)
  #define GPU_CLIENT_VALIDATE_DESTINATION_INITALIZATION_ASSERT(v) GPU_DCHECK(v)
  #define GPU_CLIENT_DCHECK(v) GPU_DCHECK(v)
#elif defined(DCHECK)
  #define GPU_CLIENT_VALIDATE_DESTINATION_INITALIZATION_ASSERT(v) DCHECK(v)
  #define GPU_CLIENT_DCHECK(v) DCHECK(v)
#else
  #define GPU_CLIENT_VALIDATE_DESTINATION_INITALIZATION_ASSERT(v) ASSERT(v)
  #define GPU_CLIENT_DCHECK(v) ASSERT(v)
#endif

#define GPU_CLIENT_VALIDATE_DESTINATION_INITALIZATION(type, ptr) \
    GPU_CLIENT_VALIDATE_DESTINATION_INITALIZATION_ASSERT(ptr && \
        (ptr[0] == static_cast<type>(0) || ptr[0] == static_cast<type>(-1)));

#define GPU_CLIENT_VALIDATE_DESTINATION_OPTIONAL_INITALIZATION(type, ptr) \
    GPU_CLIENT_VALIDATE_DESTINATION_INITALIZATION_ASSERT(!ptr || \
        (ptr[0] == static_cast<type>(0) || ptr[0] == static_cast<type>(-1)));

struct GLUniformDefinitionCHROMIUM;

namespace gpu {

class GpuControl;
class ScopedTransferBufferPtr;
class TransferBufferInterface;

namespace gles2 {

class ImageFactory;
class VertexArrayObjectManager;

class GLES2_IMPL_EXPORT GLES2Implementation
    : NON_EXPORTED_BASE(public GLES2Interface),
      NON_EXPORTED_BASE(public ContextSupport) {
 public:
  enum MappedMemoryLimit {
    kNoLimit = MappedMemoryManager::kNoLimit,
  };
  class ErrorMessageCallback {
   public:
    virtual ~ErrorMessageCallback() { }
    virtual void OnErrorMessage(const char* msg, int id) = 0;
  };

  
  struct GLES2_IMPL_EXPORT GLStaticState {
    GLStaticState();
    ~GLStaticState();

    struct GLES2_IMPL_EXPORT IntState {
      IntState();
      GLint max_combined_texture_image_units;
      GLint max_cube_map_texture_size;
      GLint max_fragment_uniform_vectors;
      GLint max_renderbuffer_size;
      GLint max_texture_image_units;
      GLint max_texture_size;
      GLint max_varying_vectors;
      GLint max_vertex_attribs;
      GLint max_vertex_texture_image_units;
      GLint max_vertex_uniform_vectors;
      GLint num_compressed_texture_formats;
      GLint num_shader_binary_formats;
    };
    IntState int_state;

    typedef std::pair<GLenum,GLenum> ShaderPrecisionKey;
    typedef std::map<ShaderPrecisionKey,
                     cmds::GetShaderPrecisionFormat::Result>
        ShaderPrecisionMap;
    ShaderPrecisionMap shader_precisions;
  };

  
  static const size_t kMaxSizeOfSimpleResult = 16 * sizeof(uint32);  

  
  static const unsigned int kStartingOffset = kMaxSizeOfSimpleResult;

  
  static const unsigned int kSizeToFlush = 256 * 1024;

  
  static const uint32 kResultBucketId = 1;

  
  static const unsigned int kAlignment = 4;

  
  static const GLuint kClientSideArrayId = 0xFEDCBA98u;
  static const GLuint kClientSideElementArrayId = 0xFEDCBA99u;

  
  static const size_t kMaxSwapBuffers = 2;

  GLES2Implementation(
      GLES2CmdHelper* helper,
      ShareGroup* share_group,
      TransferBufferInterface* transfer_buffer,
      bool bind_generates_resource,
      bool free_everything_when_invisible,
      GpuControl* gpu_control);

  virtual ~GLES2Implementation();

  bool Initialize(
      unsigned int starting_transfer_buffer_size,
      unsigned int min_transfer_buffer_size,
      unsigned int max_transfer_buffer_size,
      unsigned int mapped_memory_limit);

  
  
  GLES2CmdHelper* helper() const;

  
  GLenum GetClientSideGLError();

  
  
  
  #include "gpu/command_buffer/client/gles2_implementation_autogen.h"

  virtual void DisableVertexAttribArray(GLuint index) OVERRIDE;
  virtual void EnableVertexAttribArray(GLuint index) OVERRIDE;
  virtual void GetVertexAttribfv(
      GLuint index, GLenum pname, GLfloat* params) OVERRIDE;
  virtual void GetVertexAttribiv(
      GLuint index, GLenum pname, GLint* params) OVERRIDE;

  
  virtual void Swap() OVERRIDE;
  virtual void PartialSwapBuffers(gfx::Rect sub_buffer) OVERRIDE;
  virtual void SetSwapBuffersCompleteCallback(
      const base::Closure& swap_buffers_complete_callback)
          OVERRIDE;

  void GetProgramInfoCHROMIUMHelper(GLuint program, std::vector<int8>* result);
  GLint GetAttribLocationHelper(GLuint program, const char* name);
  GLint GetUniformLocationHelper(GLuint program, const char* name);
  bool GetActiveAttribHelper(
      GLuint program, GLuint index, GLsizei bufsize, GLsizei* length,
      GLint* size, GLenum* type, char* name);
  bool GetActiveUniformHelper(
      GLuint program, GLuint index, GLsizei bufsize, GLsizei* length,
      GLint* size, GLenum* type, char* name);

  void FreeUnusedSharedMemory();
  void FreeEverything();

  
  virtual void SignalSyncPoint(uint32 sync_point,
                               const base::Closure& callback) OVERRIDE;
  virtual void SignalQuery(uint32 query,
                           const base::Closure& callback) OVERRIDE;
  virtual void SetSurfaceVisible(bool visible) OVERRIDE;
  virtual void SendManagedMemoryStats(const ManagedMemoryStats& stats)
      OVERRIDE;

  void SetErrorMessageCallback(ErrorMessageCallback* callback) {
    error_message_callback_ = callback;
  }

  ShareGroup* share_group() const {
    return share_group_.get();
  }

  const Capabilities& capabilities() const {
    return capabilities_;
  }

  GpuControl* gpu_control() {
    return gpu_control_;
  }

 private:
  friend class GLES2ImplementationTest;
  friend class VertexArrayObjectManager;

  
  enum ExtensionStatus {
      kAvailableExtensionStatus,
      kUnavailableExtensionStatus,
      kUnknownExtensionStatus
  };

  
  struct MappedResource {
    MappedResource(GLenum _access, int _shm_id, void* mem, unsigned int offset)
        : access(_access),
          shm_id(_shm_id),
          shm_memory(mem),
          shm_offset(offset) {
    }

    
    GLenum access;

    
    int shm_id;

    
    void* shm_memory;

    
    unsigned int shm_offset;
  };

  
  struct MappedTexture : public MappedResource {
    MappedTexture(
        GLenum access,
        int shm_id,
        void* shm_mem,
        unsigned int shm_offset,
        GLenum _target,
        GLint _level,
        GLint _xoffset,
        GLint _yoffset,
        GLsizei _width,
        GLsizei _height,
        GLenum _format,
        GLenum _type)
        : MappedResource(access, shm_id, shm_mem, shm_offset),
          target(_target),
          level(_level),
          xoffset(_xoffset),
          yoffset(_yoffset),
          width(_width),
          height(_height),
          format(_format),
          type(_type) {
    }

    
    GLenum target;
    GLint level;
    GLint xoffset;
    GLint yoffset;
    GLsizei width;
    GLsizei height;
    GLenum format;
    GLenum type;
  };

  
  struct MappedBuffer : public MappedResource {
    MappedBuffer(
        GLenum access,
        int shm_id,
        void* shm_mem,
        unsigned int shm_offset,
        GLenum _target,
        GLintptr _offset,
        GLsizeiptr _size)
        : MappedResource(access, shm_id, shm_mem, shm_offset),
          target(_target),
          offset(_offset),
          size(_size) {
    }

    
    GLenum target;
    GLintptr offset;
    GLsizeiptr size;
  };

  struct TextureUnit {
    TextureUnit()
        : bound_texture_2d(0),
          bound_texture_cube_map(0),
          bound_texture_external_oes(0) {}

    
    GLuint bound_texture_2d;

    
    
    GLuint bound_texture_cube_map;

    
    
    GLuint bound_texture_external_oes;
  };

  
  class SingleThreadChecker {
   public:
    SingleThreadChecker(GLES2Implementation* gles2_implementation);
    ~SingleThreadChecker();

   private:
    GLES2Implementation* gles2_implementation_;
  };

  
  template <typename T>
  T GetResultAs() {
    return static_cast<T>(GetResultBuffer());
  }

  void* GetResultBuffer();
  int32 GetResultShmId();
  uint32 GetResultShmOffset();

  bool QueryAndCacheStaticState();

  
  
  struct GetMultipleIntegervState {
    GetMultipleIntegervState(const GLenum* pnames, GLuint pnames_count,
                             GLint* results, GLsizeiptr results_size)
       : pnames(pnames),
         pnames_count(pnames_count),
         results(results),
         results_size(results_size)
    { }
    
    const GLenum* pnames;
    GLuint pnames_count;
    
    GLint* results;
    GLsizeiptr results_size;
    
    int num_results;
    int transfer_buffer_size_needed;
    void* buffer;
    void* results_buffer;
  };
  bool GetMultipleIntegervSetup(
      GetMultipleIntegervState* state);
  void GetMultipleIntegervRequest(
      GetMultipleIntegervState* state);
  void GetMultipleIntegervOnCompleted(
      GetMultipleIntegervState* state);

  
  
  struct GetAllShaderPrecisionFormatsState {
    GetAllShaderPrecisionFormatsState(
        const GLenum (*precision_params)[2],
        int precision_params_count)
        : precision_params(precision_params),
          precision_params_count(precision_params_count)
    { }
    const GLenum (*precision_params)[2];
    int precision_params_count;
    int transfer_buffer_size_needed;
    void* results_buffer;
  };
  void GetAllShaderPrecisionFormatsSetup(
      GetAllShaderPrecisionFormatsState* state);
  void GetAllShaderPrecisionFormatsRequest(
      GetAllShaderPrecisionFormatsState* state);
  void GetAllShaderPrecisionFormatsOnCompleted(
      GetAllShaderPrecisionFormatsState* state);

  
  bool IsAnglePackReverseRowOrderAvailable();
  bool IsChromiumFramebufferMultisampleAvailable();

  bool IsExtensionAvailableHelper(
      const char* extension, ExtensionStatus* status);

  
  GLenum GetGLError();

  
  void SetGLError(GLenum error, const char* function_name, const char* msg);
  void SetGLErrorInvalidEnum(
      const char* function_name, GLenum value, const char* label);

  
  const std::string& GetLastError() {
    return last_error_;
  }

  
  void WaitForCmd();

  
  
  

  
  bool GetBucketContents(uint32 bucket_id, std::vector<int8>* data);

  
  void SetBucketContents(uint32 bucket_id, const void* data, size_t size);

  
  void SetBucketAsCString(uint32 bucket_id, const char* str);

  
  
  bool GetBucketAsString(uint32 bucket_id, std::string* str);

  
  void SetBucketAsString(uint32 bucket_id, const std::string& str);

  
  bool IsBufferReservedId(GLuint id);
  bool IsFramebufferReservedId(GLuint id) { return false;  }
  bool IsRenderbufferReservedId(GLuint id) { return false; }
  bool IsTextureReservedId(GLuint id) { return false; }
  bool IsVertexArrayReservedId(GLuint id) { return false; }

  bool BindBufferHelper(GLenum target, GLuint texture);
  bool BindFramebufferHelper(GLenum target, GLuint texture);
  bool BindRenderbufferHelper(GLenum target, GLuint texture);
  bool BindTextureHelper(GLenum target, GLuint texture);
  bool BindVertexArrayHelper(GLuint array);

  void GenBuffersHelper(GLsizei n, const GLuint* buffers);
  void GenFramebuffersHelper(GLsizei n, const GLuint* framebuffers);
  void GenRenderbuffersHelper(GLsizei n, const GLuint* renderbuffers);
  void GenTexturesHelper(GLsizei n, const GLuint* textures);
  void GenVertexArraysOESHelper(GLsizei n, const GLuint* arrays);
  void GenQueriesEXTHelper(GLsizei n, const GLuint* queries);

  void DeleteBuffersHelper(GLsizei n, const GLuint* buffers);
  void DeleteFramebuffersHelper(GLsizei n, const GLuint* framebuffers);
  void DeleteRenderbuffersHelper(GLsizei n, const GLuint* renderbuffers);
  void DeleteTexturesHelper(GLsizei n, const GLuint* textures);
  bool DeleteProgramHelper(GLuint program);
  bool DeleteShaderHelper(GLuint shader);
  void DeleteQueriesEXTHelper(GLsizei n, const GLuint* queries);
  void DeleteVertexArraysOESHelper(GLsizei n, const GLuint* arrays);

  void DeleteBuffersStub(GLsizei n, const GLuint* buffers);
  void DeleteFramebuffersStub(GLsizei n, const GLuint* framebuffers);
  void DeleteRenderbuffersStub(GLsizei n, const GLuint* renderbuffers);
  void DeleteTexturesStub(GLsizei n, const GLuint* textures);
  void DeleteProgramStub(GLsizei n, const GLuint* programs);
  void DeleteShaderStub(GLsizei n, const GLuint* shaders);
  
  void DeleteQueriesStub(GLsizei n, const GLuint* queries);
  void DeleteVertexArraysOESStub(GLsizei n, const GLuint* arrays);

  void BufferDataHelper(
      GLenum target, GLsizeiptr size, const void* data, GLenum usage);
  void BufferSubDataHelper(
      GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
  void BufferSubDataHelperImpl(
      GLenum target, GLintptr offset, GLsizeiptr size, const void* data,
      ScopedTransferBufferPtr* buffer);

  GLuint CreateImageCHROMIUMHelper(
      GLsizei width, GLsizei height, GLenum internalformat);
  void DestroyImageCHROMIUMHelper(GLuint image_id);
  void* MapImageCHROMIUMHelper(GLuint image_id, GLenum access);
  void UnmapImageCHROMIUMHelper(GLuint image_id);
  void GetImageParameterivCHROMIUMHelper(
      GLuint image_id, GLenum pname, GLint* params);

  
  bool GetVertexAttribHelper(GLuint index, GLenum pname, uint32* param);

  GLuint GetMaxValueInBufferCHROMIUMHelper(
      GLuint buffer_id, GLsizei count, GLenum type, GLuint offset);

  void RestoreElementAndArrayBuffers(bool restore);
  void RestoreArrayBuffer(bool restrore);

  
  
  void TexSubImage2DImpl(
      GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
      GLsizei height, GLenum format, GLenum type, uint32 unpadded_row_size,
      const void* pixels, uint32 pixels_padded_row_size, GLboolean internal,
      ScopedTransferBufferPtr* buffer, uint32 buffer_padded_row_size);

  
  bool GetHelper(GLenum pname, GLint* params);
  bool GetBooleanvHelper(GLenum pname, GLboolean* params);
  bool GetBufferParameterivHelper(GLenum target, GLenum pname, GLint* params);
  bool GetFloatvHelper(GLenum pname, GLfloat* params);
  bool GetFramebufferAttachmentParameterivHelper(
      GLenum target, GLenum attachment, GLenum pname, GLint* params);
  bool GetIntegervHelper(GLenum pname, GLint* params);
  bool GetProgramivHelper(GLuint program, GLenum pname, GLint* params);
  bool GetRenderbufferParameterivHelper(
      GLenum target, GLenum pname, GLint* params);
  bool GetShaderivHelper(GLuint shader, GLenum pname, GLint* params);
  bool GetTexParameterfvHelper(GLenum target, GLenum pname, GLfloat* params);
  bool GetTexParameterivHelper(GLenum target, GLenum pname, GLint* params);
  const GLubyte* GetStringHelper(GLenum name);

  bool IsExtensionAvailable(const char* ext);

  
  bool SetCapabilityState(GLenum cap, bool enabled);

  IdHandlerInterface* GetIdHandler(int id_namespace) const;

  void FinishHelper();

  
  
  
  bool MustBeContextLost();

  void RunIfContextNotLost(const base::Closure& callback);

  void OnSwapBuffersComplete();

  bool GetBoundPixelTransferBuffer(
      GLenum target, const char* function_name, GLuint* buffer_id);
  BufferTracker::Buffer* GetBoundPixelUnpackTransferBufferIfValid(
      GLuint buffer_id,
      const char* function_name, GLuint offset, GLsizei size);

  const std::string& GetLogPrefix() const;

#if defined(GL_CLIENT_FAIL_GL_ERRORS)
  void CheckGLError();
  void FailGLError(GLenum error);
#else
  void CheckGLError() { }
  void FailGLError(GLenum ) { }
#endif

  GLES2Util util_;
  GLES2CmdHelper* helper_;
  TransferBufferInterface* transfer_buffer_;
  std::string last_error_;
  DebugMarkerManager debug_marker_manager_;
  std::string this_in_hex_;

  std::queue<int32> swap_buffers_tokens_;
  std::queue<int32> rate_limit_tokens_;

  ExtensionStatus angle_pack_reverse_row_order_status_;
  ExtensionStatus chromium_framebuffer_multisample_;

  GLStaticState static_state_;
  ClientContextState state_;

  
  GLint pack_alignment_;

  
  GLint unpack_alignment_;

  
  bool unpack_flip_y_;

  
  GLint unpack_row_length_;

  
  GLint unpack_skip_rows_;

  
  GLint unpack_skip_pixels_;

  
  bool pack_reverse_row_order_;

  scoped_ptr<TextureUnit[]> texture_units_;

  
  GLuint active_texture_unit_;

  GLuint bound_framebuffer_;
  GLuint bound_read_framebuffer_;
  GLuint bound_renderbuffer_;

  
  GLuint current_program_;

  
  GLuint bound_array_buffer_id_;

  
  GLuint bound_pixel_pack_transfer_buffer_id_;
  GLuint bound_pixel_unpack_transfer_buffer_id_;

  
  
  scoped_ptr<VertexArrayObjectManager> vertex_array_object_manager_;

  GLuint reserved_ids_[2];

  
  uint32 error_bits_;

  
  bool debug_;

  
  int use_count_;

  
  
  typedef std::map<uint32, std::set<std::string> > GLStringMap;
  GLStringMap gl_strings_;

  
  
  std::set<std::string> requestable_extensions_set_;

  typedef std::map<const void*, MappedBuffer> MappedBufferMap;
  MappedBufferMap mapped_buffers_;

  typedef std::map<const void*, MappedTexture> MappedTextureMap;
  MappedTextureMap mapped_textures_;

  scoped_ptr<MappedMemoryManager> mapped_memory_;

  scoped_refptr<ShareGroup> share_group_;

  scoped_ptr<QueryTracker> query_tracker_;
  QueryTracker::Query* current_query_;

  scoped_ptr<BufferTracker> buffer_tracker_;

  scoped_ptr<GpuMemoryBufferTracker> gpu_memory_buffer_tracker_;

  ErrorMessageCallback* error_message_callback_;

  scoped_ptr<std::string> current_trace_name_;

  GpuControl* gpu_control_;

  bool surface_visible_;
  bool free_everything_when_invisible_;

  Capabilities capabilities_;

  bool use_echo_for_swap_ack_;
  base::Closure swap_buffers_complete_callback_;

  base::WeakPtrFactory<GLES2Implementation> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(GLES2Implementation);
};

inline bool GLES2Implementation::GetBufferParameterivHelper(
    GLenum , GLenum , GLint* ) {
  return false;
}

inline bool GLES2Implementation::GetFramebufferAttachmentParameterivHelper(
    GLenum ,
    GLenum ,
    GLenum ,
    GLint* ) {
  return false;
}

inline bool GLES2Implementation::GetRenderbufferParameterivHelper(
    GLenum , GLenum , GLint* ) {
  return false;
}

inline bool GLES2Implementation::GetShaderivHelper(
    GLuint , GLenum , GLint* ) {
  return false;
}

inline bool GLES2Implementation::GetTexParameterfvHelper(
    GLenum , GLenum , GLfloat* ) {
  return false;
}

inline bool GLES2Implementation::GetTexParameterivHelper(
    GLenum , GLenum , GLint* ) {
  return false;
}

}  
}  

#endif  

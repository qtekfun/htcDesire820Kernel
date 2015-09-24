// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_FRAMEBUFFER_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_FRAMEBUFFER_MANAGER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/gpu_export.h"

namespace gpu {
namespace gles2 {

class FramebufferManager;
class Renderbuffer;
class RenderbufferManager;
class Texture;
class TextureRef;
class TextureManager;

class GPU_EXPORT Framebuffer : public base::RefCounted<Framebuffer> {
 public:
  class Attachment : public base::RefCounted<Attachment> {
   public:
    virtual GLsizei width() const = 0;
    virtual GLsizei height() const = 0;
    virtual GLenum internal_format() const = 0;
    virtual GLenum texture_type() const = 0;
    virtual GLsizei samples() const = 0;
    virtual GLuint object_name() const = 0;
    virtual bool cleared() const = 0;
    virtual void SetCleared(
        RenderbufferManager* renderbuffer_manager,
        TextureManager* texture_manager,
        bool cleared) = 0;
    virtual bool IsTexture(TextureRef* texture) const = 0;
    virtual bool IsRenderbuffer(
        Renderbuffer* renderbuffer) const = 0;
    virtual bool CanRenderTo() const = 0;
    virtual void DetachFromFramebuffer(Framebuffer* framebuffer) const = 0;
    virtual bool ValidForAttachmentType(
        GLenum attachment_type, uint32 max_color_attachments) = 0;
    virtual void AddToSignature(
        TextureManager* texture_manager, std::string* signature) const = 0;
    virtual void OnWillRenderTo() const = 0;
    virtual void OnDidRenderTo() const = 0;

   protected:
    friend class base::RefCounted<Attachment>;
    virtual ~Attachment() {}
  };

  Framebuffer(FramebufferManager* manager, GLuint service_id);

  GLuint service_id() const {
    return service_id_;
  }

  bool HasUnclearedAttachment(GLenum attachment) const;

  void MarkAttachmentAsCleared(
    RenderbufferManager* renderbuffer_manager,
    TextureManager* texture_manager,
    GLenum attachment,
    bool cleared);

  
  
  void AttachRenderbuffer(
      GLenum attachment, Renderbuffer* renderbuffer);

  
  void AttachTexture(
      GLenum attachment, TextureRef* texture_ref, GLenum target,
      GLint level, GLsizei samples);

  
  void UnbindRenderbuffer(
      GLenum target, Renderbuffer* renderbuffer);

  
  void UnbindTexture(
      GLenum target, TextureRef* texture_ref);

  const Attachment* GetAttachment(GLenum attachment) const;

  bool IsDeleted() const {
    return deleted_;
  }

  void MarkAsValid() {
    has_been_bound_ = true;
  }

  bool IsValid() const {
    return has_been_bound_ && !IsDeleted();
  }

  bool HasDepthAttachment() const;
  bool HasStencilAttachment() const;
  GLenum GetColorAttachmentFormat() const;
  
  
  GLenum GetColorAttachmentTextureType() const;

  
  
  
  
  
  
  
  GLenum IsPossiblyComplete() const;

  
  GLenum GetStatus(TextureManager* texture_manager, GLenum target) const;

  
  bool IsCleared() const;

  GLenum GetDrawBuffer(GLenum draw_buffer) const;

  void SetDrawBuffers(GLsizei n, const GLenum* bufs);

  
  bool HasAlphaMRT() const;

  static void ClearFramebufferCompleteComboMap();

  static bool AllowFramebufferComboCompleteMapForTesting() {
    return allow_framebuffer_combo_complete_map_;
  }

  void OnTextureRefDetached(TextureRef* texture);
  void OnWillRenderTo() const;
  void OnDidRenderTo() const;

 private:
  friend class FramebufferManager;
  friend class base::RefCounted<Framebuffer>;

  ~Framebuffer();

  void MarkAsDeleted();

  void MarkAttachmentsAsCleared(
    RenderbufferManager* renderbuffer_manager,
    TextureManager* texture_manager,
    bool cleared);

  void MarkAsComplete(unsigned state_id) {
    framebuffer_complete_state_count_id_ = state_id;
  }

  unsigned framebuffer_complete_state_count_id() const {
    return framebuffer_complete_state_count_id_;
  }

  
  FramebufferManager* manager_;

  bool deleted_;

  
  GLuint service_id_;

  
  bool has_been_bound_;

  
  unsigned framebuffer_complete_state_count_id_;

  
  typedef base::hash_map<GLenum, scoped_refptr<Attachment> > AttachmentMap;
  AttachmentMap attachments_;

  
  
  typedef base::hash_map<std::string, bool> FramebufferComboCompleteMap;
  static FramebufferComboCompleteMap* framebuffer_combo_complete_map_;
  static bool allow_framebuffer_combo_complete_map_;

  scoped_ptr<GLenum[]> draw_buffers_;

  DISALLOW_COPY_AND_ASSIGN(Framebuffer);
};

struct DecoderFramebufferState {
  DecoderFramebufferState();
  ~DecoderFramebufferState();

  
  
  bool clear_state_dirty;

  
  scoped_refptr<Framebuffer> bound_read_framebuffer;
  scoped_refptr<Framebuffer> bound_draw_framebuffer;
};

class GPU_EXPORT FramebufferManager {
 public:
  class GPU_EXPORT TextureDetachObserver {
   public:
    TextureDetachObserver();
    virtual ~TextureDetachObserver();

    virtual void OnTextureRefDetachedFromFramebuffer(TextureRef* texture) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(TextureDetachObserver);
  };

  FramebufferManager(uint32 max_draw_buffers, uint32 max_color_attachments);
  ~FramebufferManager();

  
  void Destroy(bool have_context);

  
  void CreateFramebuffer(GLuint client_id, GLuint service_id);

  
  Framebuffer* GetFramebuffer(GLuint client_id);

  
  void RemoveFramebuffer(GLuint client_id);

  
  bool GetClientId(GLuint service_id, GLuint* client_id) const;

  void MarkAttachmentsAsCleared(
    Framebuffer* framebuffer,
    RenderbufferManager* renderbuffer_manager,
    TextureManager* texture_manager);

  void MarkAsComplete(Framebuffer* framebuffer);

  bool IsComplete(Framebuffer* framebuffer);

  void IncFramebufferStateChangeCount() {
    
    framebuffer_state_change_count_ =
        (framebuffer_state_change_count_ + 1) | 0x80000000U;
  }

  void AddObserver(TextureDetachObserver* observer) {
    texture_detach_observers_.push_back(observer);
  }

  void RemoveObserver(TextureDetachObserver* observer) {
    texture_detach_observers_.erase(
        std::remove(texture_detach_observers_.begin(),
                    texture_detach_observers_.end(),
                    observer),
        texture_detach_observers_.end());
  }

 private:
  friend class Framebuffer;

  void StartTracking(Framebuffer* framebuffer);
  void StopTracking(Framebuffer* framebuffer);

  void OnTextureRefDetached(TextureRef* texture);

  
  typedef base::hash_map<GLuint, scoped_refptr<Framebuffer> >
      FramebufferMap;
  FramebufferMap framebuffers_;

  
  
  unsigned framebuffer_state_change_count_;

  
  
  unsigned int framebuffer_count_;

  bool have_context_;

  uint32 max_draw_buffers_;
  uint32 max_color_attachments_;

  typedef std::vector<TextureDetachObserver*> TextureDetachObserverVector;
  TextureDetachObserverVector texture_detach_observers_;

  DISALLOW_COPY_AND_ASSIGN(FramebufferManager);
};

}  
}  

#endif  

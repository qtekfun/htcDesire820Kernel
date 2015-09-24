// Copyright 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_TEXTURE_LAYER_H_
#define CC_LAYERS_TEXTURE_LAYER_H_

#include <string>

#include "base/callback.h"
#include "base/synchronization/lock.h"
#include "cc/base/cc_export.h"
#include "cc/layers/layer.h"
#include "cc/resources/texture_mailbox.h"

namespace cc {
class BlockingTaskRunner;
class SingleReleaseCallback;
class TextureLayerClient;

class CC_EXPORT TextureLayer : public Layer {
 public:
  class CC_EXPORT MailboxHolder
      : public base::RefCountedThreadSafe<MailboxHolder> {
   public:
    class CC_EXPORT MainThreadReference {
     public:
      explicit MainThreadReference(MailboxHolder* holder);
      ~MainThreadReference();
      MailboxHolder* holder() { return holder_.get(); }

     private:
      scoped_refptr<MailboxHolder> holder_;
      DISALLOW_COPY_AND_ASSIGN(MainThreadReference);
    };

    const TextureMailbox& mailbox() const { return mailbox_; }
    void Return(unsigned sync_point, bool is_lost);

    
    
    scoped_ptr<SingleReleaseCallback> GetCallbackForImplThread();

   protected:
    friend class TextureLayer;

    
    static scoped_ptr<MainThreadReference> Create(
        const TextureMailbox& mailbox,
        scoped_ptr<SingleReleaseCallback> release_callback);
    virtual ~MailboxHolder();

   private:
    friend class base::RefCountedThreadSafe<MailboxHolder>;
    friend class MainThreadReference;
    explicit MailboxHolder(const TextureMailbox& mailbox,
                           scoped_ptr<SingleReleaseCallback> release_callback);

    void InternalAddRef();
    void InternalRelease();
    void ReturnAndReleaseOnImplThread(unsigned sync_point, bool is_lost);

    
    const scoped_refptr<BlockingTaskRunner> message_loop_;

    
    
    unsigned internal_references_;
    TextureMailbox mailbox_;
    scoped_ptr<SingleReleaseCallback> release_callback_;

    
    
    
    
    base::Lock arguments_lock_;
    unsigned sync_point_;
    bool is_lost_;
    DISALLOW_COPY_AND_ASSIGN(MailboxHolder);
  };

  
  
  
  static scoped_refptr<TextureLayer> Create(TextureLayerClient* client);

  
  static scoped_refptr<TextureLayer> CreateForMailbox(
      TextureLayerClient* client);

  void ClearClient();

  virtual scoped_ptr<LayerImpl> CreateLayerImpl(LayerTreeImpl* tree_impl)
      OVERRIDE;

  
  
  void SetFlipped(bool flipped);

  
  void SetUV(gfx::PointF top_left, gfx::PointF bottom_right);

  
  
  void SetVertexOpacity(float bottom_left,
                        float top_left,
                        float top_right,
                        float bottom_right);

  
  
  void SetPremultipliedAlpha(bool premultiplied_alpha);

  
  
  void SetBlendBackgroundColor(bool blend);

  
  
  
  void SetRateLimitContext(bool rate_limit);

  
  
  void SetTextureId(unsigned texture_id);

  
  bool uses_mailbox() const { return uses_mailbox_; }
  void SetTextureMailbox(const TextureMailbox& mailbox,
                         scoped_ptr<SingleReleaseCallback> release_callback);

  void WillModifyTexture();

  virtual void SetNeedsDisplayRect(const gfx::RectF& dirty_rect) OVERRIDE;

  virtual void SetLayerTreeHost(LayerTreeHost* layer_tree_host) OVERRIDE;
  virtual bool DrawsContent() const OVERRIDE;
  virtual bool Update(ResourceUpdateQueue* queue,
                      const OcclusionTracker* occlusion) OVERRIDE;
  virtual void PushPropertiesTo(LayerImpl* layer) OVERRIDE;
  virtual Region VisibleContentOpaqueRegion() const OVERRIDE;

 protected:
  TextureLayer(TextureLayerClient* client, bool uses_mailbox);
  virtual ~TextureLayer();

 private:
  void SetTextureMailboxInternal(
      const TextureMailbox& mailbox,
      scoped_ptr<SingleReleaseCallback> release_callback,
      bool requires_commit);

  TextureLayerClient* client_;
  bool uses_mailbox_;

  bool flipped_;
  gfx::PointF uv_top_left_;
  gfx::PointF uv_bottom_right_;
  
  float vertex_opacity_[4];
  bool premultiplied_alpha_;
  bool blend_background_color_;
  bool rate_limit_context_;
  bool content_committed_;

  unsigned texture_id_;
  scoped_ptr<MailboxHolder::MainThreadReference> holder_ref_;
  bool needs_set_mailbox_;

  DISALLOW_COPY_AND_ASSIGN(TextureLayer);
};

}  
#endif  

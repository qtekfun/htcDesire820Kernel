// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_H_
#define UI_COMPOSITOR_LAYER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "cc/animation/animation_events.h"
#include "cc/animation/layer_animation_event_observer.h"
#include "cc/base/scoped_ptr_vector.h"
#include "cc/layers/content_layer_client.h"
#include "cc/layers/layer_client.h"
#include "cc/layers/texture_layer_client.h"
#include "cc/resources/texture_mailbox.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkRegion.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/layer_animation_delegate.h"
#include "ui/compositor/layer_delegate.h"
#include "ui/compositor/layer_type.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/transform.h"

class SkCanvas;

namespace cc {
class ContentLayer;
class CopyOutputRequest;
class DelegatedFrameProvider;
class DelegatedRendererLayer;
class Layer;
class ResourceUpdateQueue;
class SolidColorLayer;
class TextureLayer;
struct ReturnedResource;
typedef std::vector<ReturnedResource> ReturnedResourceArray;
}

namespace ui {

class Compositor;
class LayerAnimator;
class Texture;

class COMPOSITOR_EXPORT Layer
    : public LayerAnimationDelegate,
      NON_EXPORTED_BASE(public cc::ContentLayerClient),
      NON_EXPORTED_BASE(public cc::TextureLayerClient),
      NON_EXPORTED_BASE(public cc::LayerClient),
      NON_EXPORTED_BASE(public cc::LayerAnimationEventObserver) {
 public:
  Layer();
  explicit Layer(LayerType type);
  virtual ~Layer();

  
  
  Compositor* GetCompositor();

  
  
  void SetCompositor(Compositor* compositor);

  LayerDelegate* delegate() { return delegate_; }
  void set_delegate(LayerDelegate* delegate) { delegate_ = delegate; }

  
  void Add(Layer* child);

  
  void Remove(Layer* child);

  
  void StackAtTop(Layer* child);

  
  
  
  void StackAbove(Layer* child, Layer* other);

  
  void StackAtBottom(Layer* child);

  
  
  void StackBelow(Layer* child, Layer* other);

  
  const std::vector<Layer*>& children() const { return children_; }

  
  const Layer* parent() const { return parent_; }
  Layer* parent() { return parent_; }

  LayerType type() const { return type_; }

  
  bool Contains(const Layer* other) const;

  
  
  
  void SetAnimator(LayerAnimator* animator);

  
  
  LayerAnimator* GetAnimator();

  
  void SetTransform(const gfx::Transform& transform);
  gfx::Transform transform() const;

  
  
  gfx::Transform GetTargetTransform() const;

  
  void SetBounds(const gfx::Rect& bounds);
  const gfx::Rect& bounds() const { return bounds_; }

  
  
  gfx::Rect GetTargetBounds() const;

  
  
  void SetMasksToBounds(bool masks_to_bounds);
  bool GetMasksToBounds() const;

  
  
  float opacity() const;
  void SetOpacity(float opacity);

  
  
  float GetCombinedOpacity() const;

  
  
  int background_blur() const { return background_blur_radius_; }
  void SetBackgroundBlur(int blur_radius);

  
  
  
  float layer_saturation() const { return layer_saturation_; }
  void SetLayerSaturation(float saturation);

  
  
  
  float layer_brightness() const { return layer_brightness_; }
  void SetLayerBrightness(float brightness);

  
  
  float GetTargetBrightness() const;

  
  
  
  float layer_grayscale() const { return layer_grayscale_; }
  void SetLayerGrayscale(float grayscale);

  
  
  float GetTargetGrayscale() const;

  
  
  void SetBackgroundZoom(float zoom, int inset);

  
  bool layer_inverted() const { return layer_inverted_; }
  void SetLayerInverted(bool inverted);

  
  
  float GetTargetOpacity() const;

  
  
  
  
  
  void SetMaskLayer(Layer* layer_mask);
  Layer* layer_mask_layer() { return layer_mask_; }

  
  
  void SetVisible(bool visible);
  bool visible() const { return visible_; }

  
  
  bool GetTargetVisibility() const;

  
  
  bool IsDrawn() const;

  
  
  bool ShouldDraw() const;

  
  
  
  static void ConvertPointToLayer(const Layer* source,
                                  const Layer* target,
                                  gfx::Point* point);

  
  
  
  bool GetTargetTransformRelativeTo(const Layer* ancestor,
                                    gfx::Transform* transform) const;

  
  
  static gfx::Transform ConvertTransformToCCTransform(
      const gfx::Transform& transform,
      float device_scale_factor);

  
  void SetFillsBoundsOpaquely(bool fills_bounds_opaquely);
  bool fills_bounds_opaquely() const { return fills_bounds_opaquely_; }

  const std::string& name() const { return name_; }
  void set_name(const std::string& name) { name_ = name; }

  const ui::Texture* texture() const { return texture_.get(); }

  
  
  void SetExternalTexture(ui::Texture* texture);
  ui::Texture* external_texture() { return texture_.get(); }

  
  
  void SetTextureMailbox(const cc::TextureMailbox& mailbox,
                         scoped_ptr<cc::SingleReleaseCallback> release_callback,
                         float scale_factor);
  cc::TextureMailbox GetTextureMailbox(float* scale_factor);

  
  void SetShowDelegatedContent(cc::DelegatedFrameProvider* frame_provider,
                               gfx::Size frame_size_in_dip);

  bool has_external_content() {
    return texture_layer_.get() || delegated_renderer_layer_.get();
  }

  void SetShowPaintedContent();

  
  void SetColor(SkColor color);

  
  
  bool SchedulePaint(const gfx::Rect& invalid_rect);

  
  
  
  void ScheduleDraw();

  
  
  void SendDamagedRects();

  const SkRegion& damaged_region() const { return damaged_region_; }

  
  
  void SuppressPaint();

  
  void OnDeviceScaleFactorChanged(float device_scale_factor);

  
  
  
  
  
  
  void set_scale_content(bool scale_content) { scale_content_ = scale_content; }

  
  bool scale_content() const { return scale_content_; }

  
  
  bool layer_updated_externally() const { return layer_updated_externally_; }

  
  void RequestCopyOfOutput(scoped_ptr<cc::CopyOutputRequest> request);

  
  virtual void PaintContents(
      SkCanvas* canvas, gfx::Rect clip, gfx::RectF* opaque) OVERRIDE;
  virtual void DidChangeLayerCanUseLCDText() OVERRIDE {}

  cc::Layer* cc_layer() { return cc_layer_; }

  
  virtual unsigned PrepareTexture() OVERRIDE;
  virtual bool PrepareTextureMailbox(
      cc::TextureMailbox* mailbox,
      scoped_ptr<cc::SingleReleaseCallback>* release_callback,
      bool use_shared_memory) OVERRIDE;

  float device_scale_factor() const { return device_scale_factor_; }

  
  
  void SetForceRenderSurface(bool force);
  bool force_render_surface() const { return force_render_surface_; }

  
  virtual std::string DebugName() OVERRIDE;

  virtual scoped_refptr<base::debug::ConvertableToTraceFormat>
      TakeDebugInfo() OVERRIDE;

  
  virtual void OnAnimationStarted(const cc::AnimationEvent& event) OVERRIDE;

  
  bool HasPendingThreadedAnimations() {
    return pending_threaded_animations_.size() != 0;
  }

  
  void SwitchCCLayerForTest();

 private:
  
  
  void StackRelativeTo(Layer* child, Layer* other, bool above);

  bool ConvertPointForAncestor(const Layer* ancestor, gfx::Point* point) const;
  bool ConvertPointFromAncestor(const Layer* ancestor, gfx::Point* point) const;

  
  
  void SetBoundsImmediately(const gfx::Rect& bounds);
  void SetTransformImmediately(const gfx::Transform& transform);
  void SetOpacityImmediately(float opacity);
  void SetVisibilityImmediately(bool visibility);
  void SetBrightnessImmediately(float brightness);
  void SetGrayscaleImmediately(float grayscale);
  void SetColorImmediately(SkColor color);

  
  virtual void SetBoundsFromAnimation(const gfx::Rect& bounds) OVERRIDE;
  virtual void SetTransformFromAnimation(
      const gfx::Transform& transform) OVERRIDE;
  virtual void SetOpacityFromAnimation(float opacity) OVERRIDE;
  virtual void SetVisibilityFromAnimation(bool visibility) OVERRIDE;
  virtual void SetBrightnessFromAnimation(float brightness) OVERRIDE;
  virtual void SetGrayscaleFromAnimation(float grayscale) OVERRIDE;
  virtual void SetColorFromAnimation(SkColor color) OVERRIDE;
  virtual void ScheduleDrawForAnimation() OVERRIDE;
  virtual const gfx::Rect& GetBoundsForAnimation() const OVERRIDE;
  virtual gfx::Transform GetTransformForAnimation() const OVERRIDE;
  virtual float GetOpacityForAnimation() const OVERRIDE;
  virtual bool GetVisibilityForAnimation() const OVERRIDE;
  virtual float GetBrightnessForAnimation() const OVERRIDE;
  virtual float GetGrayscaleForAnimation() const OVERRIDE;
  virtual SkColor GetColorForAnimation() const OVERRIDE;
  virtual float GetDeviceScaleFactor() const OVERRIDE;
  virtual void AddThreadedAnimation(
      scoped_ptr<cc::Animation> animation) OVERRIDE;
  virtual void RemoveThreadedAnimation(int animation_id) OVERRIDE;

  void CreateWebLayer();
  void RecomputeCCTransformFromTransform(const gfx::Transform& transform);
  void RecomputeDrawsContentAndUVRect();
  void RecomputePosition();

  
  void SetLayerFilters();

  
  void SetLayerBackgroundFilters();

  void UpdateIsDrawn();

  void SwitchToLayer(scoped_refptr<cc::Layer> new_layer);

  
  
  
  
  void SendPendingThreadedAnimations();

  const LayerType type_;

  Compositor* compositor_;

  scoped_refptr<ui::Texture> texture_;

  Layer* parent_;

  
  std::vector<Layer*> children_;

  gfx::Rect bounds_;

  
  bool visible_;

  bool force_render_surface_;

  bool fills_bounds_opaquely_;

  
  bool layer_updated_externally_;

  
  
  SkRegion damaged_region_;

  int background_blur_radius_;

  
  
  float layer_saturation_;
  float layer_brightness_;
  float layer_grayscale_;
  bool layer_inverted_;

  
  Layer* layer_mask_;
  
  
  
  Layer* layer_mask_back_link_;

  
  
  float zoom_;

  
  int zoom_inset_;

  std::string name_;

  LayerDelegate* delegate_;

  scoped_refptr<LayerAnimator> animator_;

  
  
  cc::ScopedPtrVector<cc::Animation> pending_threaded_animations_;

  
  
  scoped_refptr<cc::ContentLayer> content_layer_;
  scoped_refptr<cc::TextureLayer> texture_layer_;
  scoped_refptr<cc::SolidColorLayer> solid_color_layer_;
  scoped_refptr<cc::DelegatedRendererLayer> delegated_renderer_layer_;
  cc::Layer* cc_layer_;

  
  
  bool scale_content_;

  
  float device_scale_factor_;

  
  cc::TextureMailbox mailbox_;

  
  float mailbox_scale_factor_;

  
  
  gfx::Size delegated_frame_size_in_dip_;

  DISALLOW_COPY_AND_ASSIGN(Layer);
};

}  

#endif  

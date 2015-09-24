// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_AURA_REFLECTOR_IMPL_H_
#define CONTENT_BROWSER_AURA_REFLECTOR_IMPL_H_

#include "base/id_map.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/aura/image_transport_factory.h"
#include "ui/compositor/reflector.h"
#include "ui/gfx/size.h"

namespace base { class MessageLoopProxy; }

namespace gfx { class Rect; }

namespace ui {
class Compositor;
class Layer;
}

namespace content {

class BrowserCompositorOutputSurface;

class ReflectorImpl : public ImageTransportFactoryObserver,
                      public base::SupportsWeakPtr<ReflectorImpl>,
                      public ui::Reflector {
 public:
  ReflectorImpl(
      ui::Compositor* mirrored_compositor,
      ui::Layer* mirroring_layer,
      IDMap<BrowserCompositorOutputSurface>* output_surface_map,
      int surface_id);

  ui::Compositor* mirrored_compositor() {
    return mirrored_compositor_;
  }

  void InitOnImplThread();
  void Shutdown();
  void ShutdownOnImplThread();

  
  
  void AttachToOutputSurface(BrowserCompositorOutputSurface* surface);

  
  virtual void OnMirroringCompositorResized() OVERRIDE;

  
  virtual void OnLostResources() OVERRIDE;

  
  
  void OnReshape(gfx::Size size);

  
  
  
  void OnSwapBuffers();

  
  
  
  void OnPostSubBuffer(gfx::Rect rect);

  
  
  
  
  void CreateSharedTexture();

  
  
  void OnSourceSurfaceReady(int surface_id);

 private:
  virtual ~ReflectorImpl();

  void UpdateTextureSizeOnMainThread(gfx::Size size);

  
  void FullRedrawOnMainThread(gfx::Size size);

  void UpdateSubBufferOnMainThread(gfx::Size size, gfx::Rect rect);

  
  
  void FullRedrawContentOnMainThread();

  
  
  static void DeleteOnMainThread(scoped_refptr<ReflectorImpl> reflector) {}

  
  
  
  
  
  
  int texture_id_;
  gfx::Size texture_size_;

  
  IDMap<BrowserCompositorOutputSurface>* output_surface_map_;
  scoped_ptr<GLHelper> gl_helper_;

  
  ui::Compositor* mirrored_compositor_;
  ui::Compositor* mirroring_compositor_;
  ui::Layer* mirroring_layer_;
  scoped_refptr<ui::Texture> shared_texture_;
  scoped_refptr<base::MessageLoopProxy> impl_message_loop_;
  scoped_refptr<base::MessageLoopProxy> main_message_loop_;
  int surface_id_;
};

}  

#endif  

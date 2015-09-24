// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_ANDROID_COMPOSITOR_H_
#define CONTENT_PUBLIC_BROWSER_ANDROID_COMPOSITOR_H_

#include "base/callback.h"
#include "cc/resources/ui_resource_bitmap.h"
#include "cc/resources/ui_resource_client.h"
#include "content/common/content_export.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

#include "third_party/WebKit/public/platform/WebGraphicsContext3D.h"

namespace cc {
class Layer;
}

namespace gfx {
class JavaBitmap;
}

namespace content {
class CompositorClient;

class CONTENT_EXPORT Compositor {
 public:
  virtual ~Compositor() {}

  
  
  static void Initialize();

  
  
  static Compositor* Create(CompositorClient* client,
                            gfx::NativeWindow root_window);

  
  virtual void SetRootLayer(scoped_refptr<cc::Layer> root) = 0;

  
  virtual void setDeviceScaleFactor(float factor) = 0;

  
  virtual void SetWindowBounds(const gfx::Size& size) = 0;

  
  
  
  virtual void SetVisible(bool visible) = 0;

  
  
  virtual void SetWindowSurface(ANativeWindow* window) = 0;

  
  virtual void SetSurface(jobject surface) = 0;

  
  
  
  virtual bool CompositeAndReadback(void *pixels, const gfx::Rect& rect) = 0;

  
  virtual void Composite() = 0;

  
  virtual cc::UIResourceId GenerateUIResource(
      const cc::UIResourceBitmap& bitmap) = 0;

  
  virtual void DeleteUIResource(cc::UIResourceId resource_id) = 0;

  
  
  virtual blink::WebGLId GenerateTexture(gfx::JavaBitmap& bitmap) = 0;

  
  
  virtual blink::WebGLId GenerateCompressedTexture(gfx::Size& size,
                                                    int data_size,
                                                    void* data) = 0;

  
  virtual void DeleteTexture(blink::WebGLId texture_id) = 0;

  
  
  virtual bool CopyTextureToBitmap(blink::WebGLId texture_id,
                                   gfx::JavaBitmap& bitmap) = 0;

  
  
  
  
  virtual bool CopyTextureToBitmap(blink::WebGLId texture_id,
                                   const gfx::Rect& src_rect,
                                   gfx::JavaBitmap& bitmap) = 0;
 protected:
  Compositor() {}
};

}  

#endif  

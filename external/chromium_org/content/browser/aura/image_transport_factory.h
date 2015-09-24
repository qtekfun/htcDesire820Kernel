// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_AURA_IMAGE_TRANSPORT_FACTORY_H_
#define CONTENT_BROWSER_AURA_IMAGE_TRANSPORT_FACTORY_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Size;
}

namespace ui {
class ContextFactory;
class Texture;
}

namespace blink {
class WebGraphicsContext3D;
}

namespace content {
class GLHelper;

class CONTENT_EXPORT ImageTransportFactoryObserver {
 public:
  virtual ~ImageTransportFactoryObserver() {}

  
  
  
  
  
  
  virtual void OnLostResources() = 0;
};

class CONTENT_EXPORT ImageTransportFactory {
 public:
  virtual ~ImageTransportFactory() {}

  
  static void Initialize();

  
  
  static void InitializeForUnitTests(
      scoped_ptr<ui::ContextFactory> test_factory);

  
  static void Terminate();

  
  static ImageTransportFactory* GetInstance();

  
  virtual ui::ContextFactory* AsContextFactory() = 0;

  
  
  
  virtual gfx::GLSurfaceHandle CreateSharedSurfaceHandle() = 0;

  
  virtual void DestroySharedSurfaceHandle(gfx::GLSurfaceHandle surface) = 0;

  
  virtual scoped_refptr<ui::Texture> CreateTransportClient(
      float device_scale_factor) = 0;

  
  
  virtual scoped_refptr<ui::Texture> CreateOwnedTexture(
      const gfx::Size& size,
      float device_scale_factor,
      unsigned int texture_id) = 0;

  
  
  
  virtual GLHelper* GetGLHelper() = 0;

  
  virtual uint32 InsertSyncPoint() = 0;

  
  virtual void WaitSyncPoint(uint32 sync_point) = 0;

  virtual void AddObserver(ImageTransportFactoryObserver* observer) = 0;
  virtual void RemoveObserver(ImageTransportFactoryObserver* observer) = 0;
};

}  

#endif  

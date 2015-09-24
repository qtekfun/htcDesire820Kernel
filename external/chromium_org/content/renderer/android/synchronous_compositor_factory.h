// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_ANDROID_SYNCHRONOUS_COMPOSITOR_FACTORY_H_
#define CONTENT_RENDERER_ANDROID_SYNCHRONOUS_COMPOSITOR_FACTORY_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class MessageLoopProxy;
}

namespace cc {
class ContextProvider;
class OutputSurface;
}

namespace content {

class InputHandlerManagerClient;
class StreamTextureFactory;

class SynchronousCompositorFactory {
 public:
  
  
  static void SetInstance(SynchronousCompositorFactory* instance);
  static SynchronousCompositorFactory* GetInstance();

  virtual scoped_refptr<base::MessageLoopProxy>
      GetCompositorMessageLoop() = 0;
  virtual scoped_ptr<cc::OutputSurface> CreateOutputSurface(
      int routing_id) = 0;

  
  virtual InputHandlerManagerClient* GetInputHandlerManagerClient() = 0;

  virtual scoped_refptr<cc::ContextProvider>
      GetOffscreenContextProviderForMainThread() = 0;
  virtual scoped_refptr<cc::ContextProvider>
      GetOffscreenContextProviderForCompositorThread() = 0;
  virtual scoped_ptr<StreamTextureFactory> CreateStreamTextureFactory(
      int view_id) = 0;

 protected:
  SynchronousCompositorFactory() {}
  virtual ~SynchronousCompositorFactory() {}
};

}

#endif  

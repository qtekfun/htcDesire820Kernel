// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_LAYERS_TEXTURE_LAYER_CLIENT_H_
#define CC_LAYERS_TEXTURE_LAYER_CLIENT_H_

#include "cc/resources/single_release_callback.h"

namespace cc {
class ResourceUpdateQueue;
class TextureMailbox;

class TextureLayerClient {
 public:
  
  
  virtual unsigned PrepareTexture() = 0;

  
  
  
  virtual bool PrepareTextureMailbox(
      TextureMailbox* mailbox,
      scoped_ptr<SingleReleaseCallback>* release_callback,
      bool use_shared_memory) = 0;

 protected:
  virtual ~TextureLayerClient() {}
};

}  

#endif  

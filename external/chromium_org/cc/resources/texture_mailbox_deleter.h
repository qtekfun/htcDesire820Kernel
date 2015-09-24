// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_TEXTURE_MAILBOX_DELETER_H_
#define CC_RESOURCES_TEXTURE_MAILBOX_DELETER_H_

#include "base/memory/weak_ptr.h"
#include "cc/base/cc_export.h"
#include "cc/base/scoped_ptr_vector.h"

namespace cc {
class ContextProvider;
class SingleReleaseCallback;

class CC_EXPORT TextureMailboxDeleter {
 public:
  TextureMailboxDeleter();
  ~TextureMailboxDeleter();

  
  
  
  
  
  
  
  scoped_ptr<SingleReleaseCallback> GetReleaseCallback(
      const scoped_refptr<ContextProvider>& context_provider,
      unsigned texture_id);

 private:
  
  
  void RunDeleteTextureOnImplThread(
      SingleReleaseCallback* impl_callback,
      unsigned sync_point,
      bool is_lost);

  ScopedPtrVector<SingleReleaseCallback> impl_callbacks_;
  base::WeakPtrFactory<TextureMailboxDeleter> weak_ptr_factory_;
};

}  

#endif  

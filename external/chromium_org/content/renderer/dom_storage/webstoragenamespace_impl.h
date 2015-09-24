// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_DOM_STORAGE_WEBSTORAGENAMESPACE_IMPL_H_
#define CONTENT_RENDERER_DOM_STORAGE_WEBSTORAGENAMESPACE_IMPL_H_

#include "base/basictypes.h"
#include "third_party/WebKit/public/platform/WebStorageNamespace.h"

namespace content {

class WebStorageNamespaceImpl : public blink::WebStorageNamespace {
 public:
  
  
  WebStorageNamespaceImpl();
  explicit WebStorageNamespaceImpl(int64 namespace_id);
  virtual ~WebStorageNamespaceImpl();

  
  virtual blink::WebStorageArea* createStorageArea(
      const blink::WebString& origin);
  virtual blink::WebStorageNamespace* copy();
  virtual bool isSameNamespace(const WebStorageNamespace&) const;

 private:
  int64 namespace_id_;
};

}  

#endif  

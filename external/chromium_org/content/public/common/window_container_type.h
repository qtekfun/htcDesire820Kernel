// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_WINDOW_CONTAINER_TYPE_H_
#define CONTENT_PUBLIC_COMMON_WINDOW_CONTAINER_TYPE_H_

namespace blink {

struct WebWindowFeatures;

}

enum WindowContainerType {
  
  WINDOW_CONTAINER_TYPE_NORMAL = 0,

  
  WINDOW_CONTAINER_TYPE_BACKGROUND,

  
  
  
  WINDOW_CONTAINER_TYPE_PERSISTENT,

  WINDOW_CONTAINER_TYPE_MAX_VALUE
};

WindowContainerType WindowFeaturesToContainerType(
    const blink::WebWindowFeatures& window_features);

#endif  

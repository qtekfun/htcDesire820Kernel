// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RENDER_WIDGET_HOST_ITERATOR_H_
#define CONTENT_PUBLIC_BROWSER_RENDER_WIDGET_HOST_ITERATOR_H_

namespace content {

class RenderWidgetHost;

class RenderWidgetHostIterator {
 public:
  virtual ~RenderWidgetHostIterator() {}

  
  
  virtual RenderWidgetHost* GetNextHost() = 0;
};

}  

#endif  

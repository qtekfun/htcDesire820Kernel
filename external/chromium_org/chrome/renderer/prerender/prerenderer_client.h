// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_PRERENDER_PRERENDERER_CLIENT_H_
#define CHROME_RENDERER_PRERENDER_PRERENDERER_CLIENT_H_

#include "base/compiler_specific.h"
#include "content/public/renderer/render_view_observer.h"
#include "third_party/WebKit/public/web/WebPrerendererClient.h"

namespace prerender {

class PrerendererClient : public content::RenderViewObserver,
                          public blink::WebPrerendererClient {
 public:
  explicit PrerendererClient(content::RenderView* render_view);

 private:
  virtual ~PrerendererClient();

  
  virtual void willAddPrerender(blink::WebPrerender* prerender) OVERRIDE;
};

}  

#endif  


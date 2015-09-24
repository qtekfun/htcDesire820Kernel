// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_BROWSER_PLUGIN_MOCK_BROWSER_PLUGIN_H_
#define CONTENT_RENDERER_BROWSER_PLUGIN_MOCK_BROWSER_PLUGIN_H_

#include "content/renderer/browser_plugin/browser_plugin.h"

namespace content {

class MockBrowserPlugin : public BrowserPlugin {
 public:
  MockBrowserPlugin(RenderViewImpl* render_view,
                    blink::WebFrame* frame);

  virtual ~MockBrowserPlugin();

  
  using BrowserPlugin::OnAttachACK;
  using BrowserPlugin::guest_crashed_;
  using BrowserPlugin::pending_damage_buffer_;
  using BrowserPlugin::damage_buffer_sequence_id_;
};

}  

#endif  

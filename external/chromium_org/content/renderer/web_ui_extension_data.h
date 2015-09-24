// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_WEBUI_EXTENSION_DATA_H_
#define CONTENT_RENDERER_WEBUI_EXTENSION_DATA_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "content/public/renderer/render_view_observer.h"
#include "content/public/renderer/render_view_observer_tracker.h"

namespace content {

class WebUIExtensionData
    : public RenderViewObserver,
      public RenderViewObserverTracker<WebUIExtensionData> {
 public:
  explicit WebUIExtensionData(RenderView* render_view);
  virtual ~WebUIExtensionData();

  
  
  std::string GetValue(const std::string& key) const;

 private:
  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void OnSetWebUIProperty(const std::string& name, const std::string& value);

  std::map<std::string, std::string> variable_map_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(WebUIExtensionData);
};

}  

#endif  

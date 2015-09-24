// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_PEPPER_RENDERER_INSTANCE_DATA_H_
#define CONTENT_COMMON_PEPPER_RENDERER_INSTANCE_DATA_H_

#include "url/gurl.h"

namespace content {

struct PepperRendererInstanceData {
  PepperRendererInstanceData();
  PepperRendererInstanceData(int render_process,
                             int render_view,
                             const GURL& document,
                             const GURL& plugin);
  ~PepperRendererInstanceData();
  int render_process_id;
  int render_view_id;
  GURL document_url;
  GURL plugin_url;
};

}  

#endif  

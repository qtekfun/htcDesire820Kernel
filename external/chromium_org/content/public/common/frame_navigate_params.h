// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_FRAME_NAVIGATE_PARAMS_H_
#define CONTENT_PUBLIC_COMMON_FRAME_NAVIGATE_PARAMS_H_

#include <string>
#include <vector>

#include "content/common/content_export.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/referrer.h"
#include "net/base/host_port_pair.h"
#include "url/gurl.h"

namespace content {

struct CONTENT_EXPORT FrameNavigateParams {
  FrameNavigateParams();
  ~FrameNavigateParams();

  
  
  
  
  int32 page_id;

  
  GURL url;

  
  
  
  
  
  GURL base_url;

  
  
  content::Referrer referrer;

  
  PageTransition transition;

  
  
  
  
  std::vector<GURL> redirects;

  
  
  bool should_update_history;

  
  GURL searchable_form_url;
  std::string searchable_form_encoding;

  
  std::string contents_mime_type;

  
  net::HostPortPair socket_address;
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_PUBLIC_BROWSER_PAGE_NAVIGATOR_H_
#define CONTENT_PUBLIC_BROWSER_PAGE_NAVIGATOR_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/ref_counted_memory.h"
#include "content/common/content_export.h"
#include "content/public/browser/global_request_id.h"
#include "content/public/common/page_transition_types.h"
#include "content/public/common/referrer.h"
#include "ui/base/window_open_disposition.h"
#include "url/gurl.h"

namespace content {

class WebContents;

struct CONTENT_EXPORT OpenURLParams {
  OpenURLParams(const GURL& url,
                const Referrer& referrer,
                WindowOpenDisposition disposition,
                PageTransition transition,
                bool is_renderer_initiated);
  OpenURLParams(const GURL& url,
                const Referrer& referrer,
                int64 source_frame_id,
                int64 frame_tree_node_id,
                WindowOpenDisposition disposition,
                PageTransition transition,
                bool is_renderer_initiated);
  ~OpenURLParams();

  
  GURL url;
  Referrer referrer;

  
  std::vector<GURL> redirect_chain;

  
  
  
  
  bool uses_post;

  
  scoped_refptr<base::RefCountedMemory> browser_initiated_post_data;

  
  
  
  std::string extra_headers;

  
  
  int64 source_frame_id;

  
  int64 frame_tree_node_id;

  
  WindowOpenDisposition disposition;

  
  PageTransition transition;

  
  bool is_renderer_initiated;

  
  
  GlobalRequestID transferred_global_request_id;

  
  
  bool should_replace_current_entry;

  
  
  bool user_gesture;

 private:
  OpenURLParams();
};

class PageNavigator {
 public:
  virtual ~PageNavigator() {}

  
  
  
  
  virtual WebContents* OpenURL(const OpenURLParams& params) = 0;
};

}  

#endif  

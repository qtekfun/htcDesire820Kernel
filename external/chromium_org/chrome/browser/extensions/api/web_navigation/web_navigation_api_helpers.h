// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_WEB_NAVIGATION_WEB_NAVIGATION_API_HELPERS_H_
#define CHROME_BROWSER_EXTENSIONS_API_WEB_NAVIGATION_WEB_NAVIGATION_API_HELPERS_H_

#include <string>

#include "base/basictypes.h"
#include "content/public/common/page_transition_types.h"

namespace content {
class BrowserContext;
class WebContents;
}

class GURL;

namespace extensions {

namespace web_navigation_api_helpers {

int GetFrameId(bool is_main_frame, int64 frame_id);

void DispatchOnBeforeNavigate(content::WebContents* web_contents,
                              int render_process_id,
                              int64 frame_id,
                              bool is_main_frame,
                              int64 parent_frame_id,
                              bool parent_is_main_frame,
                              const GURL& validated_url);

void DispatchOnCommitted(const std::string& event_name,
                         content::WebContents* web_contents,
                         int64 frame_id,
                         bool is_main_frame,
                         const GURL& url,
                         content::PageTransition transition_type);

void DispatchOnDOMContentLoaded(content::WebContents* web_contents,
                                const GURL& url,
                                bool is_main_frame,
                                int64 frame_id);

void DispatchOnCompleted(content::WebContents* web_contents,
                         const GURL& url,
                         bool is_main_frame,
                         int64 frame_id);

void DispatchOnCreatedNavigationTarget(
    content::WebContents* web_contents,
    content::BrowserContext* browser_context,
    int64 source_frame_id,
    bool source_frame_is_main_frame,
    content::WebContents* target_web_contents,
    const GURL& target_url);

void DispatchOnErrorOccurred(content::WebContents* web_contents,
                             int render_process_id,
                             const GURL& url,
                             int64 frame_id,
                             bool is_main_frame,
                             int error_code);

void DispatchOnTabReplaced(
    content::WebContents* old_web_contents,
    content::BrowserContext* browser_context,
    content::WebContents* new_web_contents);

}  

}  

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_SAVABLE_RESOURCES_H_
#define CONTENT_RENDERER_SAVABLE_RESOURCES_H_

#include <string>
#include <vector>

#include "content/common/content_export.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "url/gurl.h"

namespace blink {
class WebElement;
class WebString;
class WebView;
}

namespace content {

struct SavableResourcesResult {
  
  std::vector<GURL>* resources_list;
  
  
  std::vector<GURL>* referrer_urls_list;
  
  std::vector<blink::WebReferrerPolicy>* referrer_policies_list;
  
  std::vector<GURL>* frames_list;

  
  SavableResourcesResult(
      std::vector<GURL>* resources_list,
      std::vector<GURL>* referrer_urls_list,
      std::vector<blink::WebReferrerPolicy>* referrer_policies_list,
      std::vector<GURL>* frames_list)
      : resources_list(resources_list),
        referrer_urls_list(referrer_urls_list),
        referrer_policies_list(referrer_policies_list),
        frames_list(frames_list) { }

 private:
  DISALLOW_COPY_AND_ASSIGN(SavableResourcesResult);
};

CONTENT_EXPORT bool GetAllSavableResourceLinksForCurrentPage(
    blink::WebView* view,
    const GURL& page_url,
    SavableResourcesResult* savable_resources_result,
    const char** savable_schemes);

CONTENT_EXPORT blink::WebString GetSubResourceLinkFromElement(
    const blink::WebElement& element);

}  

#endif  

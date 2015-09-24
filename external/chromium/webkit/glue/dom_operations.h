// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_GLUE_DOM_OPERATIONS_H__
#define WEBKIT_GLUE_DOM_OPERATIONS_H__

#include <string>
#include <vector>

#include "googleurl/src/gurl.h"

namespace WebKit {
class WebDocument;
class WebElement;
class WebString;
class WebView;
}

namespace webkit_glue {

struct SavableResourcesResult {
  
  std::vector<GURL>* resources_list;
  
  
  std::vector<GURL>* referrers_list;
  
  std::vector<GURL>* frames_list;

  
  SavableResourcesResult(std::vector<GURL>* resources_list,
                         std::vector<GURL>* referrers_list,
                         std::vector<GURL>* frames_list)
      : resources_list(resources_list),
        referrers_list(referrers_list),
        frames_list(frames_list) { }

 private:
  DISALLOW_COPY_AND_ASSIGN(SavableResourcesResult);
};

bool GetAllSavableResourceLinksForCurrentPage(WebKit::WebView* view,
    const GURL& page_url, SavableResourcesResult* savable_resources_result,
    const char** savable_schemes);

bool PauseAnimationAtTimeOnElementWithId(WebKit::WebView* view,
                                         const std::string& animation_name,
                                         double time,
                                         const std::string& element_id);

bool PauseTransitionAtTimeOnElementWithId(WebKit::WebView* view,
                                          const std::string& property_name,
                                          double time,
                                          const std::string& element_id);

bool ElementDoesAutoCompleteForElementWithId(WebKit::WebView* view,
                                             const std::string& element_id);

int NumberOfActiveAnimations(WebKit::WebView* view);

WebKit::WebString GetSubResourceLinkFromElement(
    const WebKit::WebElement& element);

void GetMetaElementsWithAttribute(
    WebKit::WebDocument* document,
    const string16& attribute_name,
    const string16& atribute_value,
    std::vector<WebKit::WebElement>* meta_elements);

}  

#endif  

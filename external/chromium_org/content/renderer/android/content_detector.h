// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_ANDROID_CONTENT_DETECTOR_H_
#define CONTENT_RENDERER_ANDROID_CONTENT_DETECTOR_H_

#include "third_party/WebKit/public/web/WebRange.h"
#include "url/gurl.h"

namespace blink {
class WebHitTestResult;
}

namespace content {

class ContentDetector {
 public:
  
  struct Result {
    Result();
    Result(const blink::WebRange& content_boundaries,
           const std::string& text,
           const GURL& intent_url);
    ~Result();

    bool valid;
    blink::WebRange content_boundaries;
    std::string text; 
    GURL intent_url; 
  };

  virtual ~ContentDetector() {}

  
  
  Result FindTappedContent(const blink::WebHitTestResult& hit_test);

 protected:
  ContentDetector() {}

  
  
  
  
  virtual bool FindContent(const base::string16::const_iterator& begin,
                           const base::string16::const_iterator& end,
                           size_t* start_pos,
                           size_t* end_pos,
                           std::string* content_text) = 0;

  
  virtual GURL GetIntentURL(const std::string& content_text) = 0;

  
  
  virtual size_t GetMaximumContentLength() = 0;

  blink::WebRange FindContentRange(const blink::WebHitTestResult& hit_test,
                                    std::string* content_text);

  DISALLOW_COPY_AND_ASSIGN(ContentDetector);
};

}  

#endif  

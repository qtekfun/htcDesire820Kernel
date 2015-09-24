// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_MEDIA_TEST_RESPONSE_GENERATOR_H_
#define CONTENT_RENDERER_MEDIA_TEST_RESPONSE_GENERATOR_H_

#include "base/basictypes.h"
#include "third_party/WebKit/public/platform/WebURLError.h"
#include "third_party/WebKit/public/platform/WebURLResponse.h"
#include "url/gurl.h"

namespace content {

class TestResponseGenerator {
 public:
  enum Flags {
    kNormal = 0,
    kNoAcceptRanges = 1 << 0,   
    kNoContentRange = 1 << 1,   
    kNoContentLength = 1 << 2,  
    kNoContentRangeInstanceSize = 1 << 3,  
  };

  
  
  TestResponseGenerator(const GURL& gurl, int64 content_length);

  
  blink::WebURLError GenerateError();

  
  blink::WebURLResponse Generate200();

  
  
  blink::WebURLResponse Generate206(int64 first_byte_offset);

  
  
  
  blink::WebURLResponse Generate206(int64 first_byte_offset, Flags flags);

  
  blink::WebURLResponse Generate404();

  
  
  
  
  
  blink::WebURLResponse GenerateFileResponse(int64 first_byte_offset);

  int64 content_length() { return content_length_; }

 private:
  GURL gurl_;
  int64 content_length_;

  DISALLOW_COPY_AND_ASSIGN(TestResponseGenerator);
};

}  

#endif  

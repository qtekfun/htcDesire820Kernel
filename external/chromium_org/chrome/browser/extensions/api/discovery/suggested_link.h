// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DISCOVERY_SUGGESTED_LINK_H_
#define CHROME_BROWSER_EXTENSIONS_API_DISCOVERY_SUGGESTED_LINK_H_

#include <string>
#include <vector>

#include "base/memory/linked_ptr.h"

namespace extensions {

class SuggestedLink {
 public:
  SuggestedLink(const std::string& link_url, const std::string& link_text,
                const std::string& url_image, double score);
  ~SuggestedLink();

  const std::string& link_url() const { return link_url_; }
  const std::string& link_text() const { return link_text_; }
  const std::string& url_image() const { return url_image_; }
  double score() const { return score_; }

 private:
  std::string link_url_;
  std::string link_text_;
  std::string url_image_;

  
  
  
  
  double score_;

  DISALLOW_COPY_AND_ASSIGN(SuggestedLink);
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_NET_URL_FIXER_UPPER_H_
#define CHROME_COMMON_NET_URL_FIXER_UPPER_H_

#include <string>

#include "base/strings/string16.h"
#include "url/gurl.h"

namespace base {
class FilePath;
}

namespace url_parse {
struct Component;
struct Parsed;
}

namespace URLFixerUpper {

  
  
  
  
  
  std::string SegmentURL(const std::string& text, url_parse::Parsed* parts);
  base::string16 SegmentURL(const base::string16& text,
                            url_parse::Parsed* parts);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  GURL FixupURL(const std::string& text, const std::string& desired_tld);

  
  
  
  
  
  
  
  
  
  GURL FixupRelativeFile(const base::FilePath& base_dir,
                         const base::FilePath& text);

  
  
  
  
  
  
  void OffsetComponent(int offset, url_parse::Component* part);

  
  
  
  extern const char* home_directory_override;

}  

#endif  

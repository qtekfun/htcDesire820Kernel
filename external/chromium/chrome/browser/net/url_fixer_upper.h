// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_URL_FIXER_UPPER_H_
#define CHROME_BROWSER_NET_URL_FIXER_UPPER_H_
#pragma once

#include <string>

#include "base/string16.h"
#include "googleurl/src/gurl.h"

namespace url_parse {
  struct Component;
  struct Parsed;
}

class FilePath;

namespace URLFixerUpper {

  
  
  
  
  
  std::string SegmentURL(const std::string& text, url_parse::Parsed* parts);
  string16 SegmentURL(const string16& text, url_parse::Parsed* parts);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  GURL FixupURL(const std::string& text, const std::string& desired_tld);

  
  
  
  
  
  
  
  
  
  GURL FixupRelativeFile(const FilePath& base_dir, const FilePath& text);

  
  
  
  
  
  
  void OffsetComponent(int offset, url_parse::Component* part);

  
  
  
  extern const char* home_directory_override;
};

#endif  

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PARSERS_METADATA_PARSER_H_
#define CHROME_BROWSER_PARSERS_METADATA_PARSER_H_

#include <string>

namespace base {
class FilePath;
}

class MetadataPropertyIterator {
 public:
  MetadataPropertyIterator() {}
  virtual ~MetadataPropertyIterator() {}


  
  
  virtual bool GetNext(std::string* key, std::string* value) = 0;

  
  virtual int Length() = 0;

  
  virtual bool IsEnd() = 0;
};

class MetadataParser {
 public:
  explicit MetadataParser(const base::FilePath& path) {}
  virtual ~MetadataParser() {}


  static const char* kPropertyType;
  static const char* kPropertyFilesize;
  static const char* kPropertyTitle;

  
  virtual bool Parse() = 0;

  
  virtual bool GetProperty(const std::string& key, std::string* value) = 0;

  
  
  virtual MetadataPropertyIterator* GetPropertyIterator() = 0;
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_FILE_HIGHLIGHTER_H_
#define EXTENSIONS_BROWSER_FILE_HIGHLIGHTER_H_

#include <string>

#include "base/basictypes.h"

namespace base {
class DictionaryValue;
}

namespace extensions {

class FileHighlighter {
 public:
  virtual ~FileHighlighter();

  
  
  std::string GetBeforeFeature() const;

  
  std::string GetFeature() const;

  
  
  std::string GetAfterFeature() const;

  
  
  void SetHighlightedRegions(base::DictionaryValue* dict) const;

 protected:
  FileHighlighter(const std::string& contents);

  
  std::string contents_;

  
  size_t start_;

  
  size_t end_;

  DISALLOW_COPY_AND_ASSIGN(FileHighlighter);
};

class ManifestHighlighter : public FileHighlighter {
 public:
  ManifestHighlighter(const std::string& manifest,
                      const std::string& key,
                      const std::string& specific );
  virtual ~ManifestHighlighter();

 private:
  
  
  void Parse(const std::string& key, const std::string& specific);

  
  
  
  
  bool FindBounds(const std::string& feature, bool enforce_at_top_level);

  
  void FindBoundsEnd(const std::string& feature, size_t local_start);

  DISALLOW_COPY_AND_ASSIGN(ManifestHighlighter);
};

class SourceHighlighter : public FileHighlighter {
 public:
  SourceHighlighter(const std::string& source, size_t line_number);
  virtual ~SourceHighlighter();

 private:
  
  
  void Parse(size_t line_number);

  DISALLOW_COPY_AND_ASSIGN(SourceHighlighter);
};

}  

#endif  

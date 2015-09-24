// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_EXTENT_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_EXTENT_H_
#pragma once

#include <vector>

class GURL;
class URLPattern;

class ExtensionExtent {
 public:
  typedef std::vector<URLPattern> PatternList;

  ExtensionExtent();
  ExtensionExtent(const ExtensionExtent& rhs);
  ~ExtensionExtent();
  ExtensionExtent& operator=(const ExtensionExtent& rhs);

  bool is_empty() const;

  const PatternList& patterns() const { return patterns_; }
  void AddPattern(const URLPattern& pattern);
  void ClearPaths();

  
  bool ContainsURL(const GURL& url) const;

  
  bool OverlapsWith(const ExtensionExtent& other) const;

 private:
  
  PatternList patterns_;
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_ICON_SET_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_ICON_SET_H_

#include <map>
#include <string>

class ExtensionIconSet {
 public:
  
  
  enum MatchType {
    MATCH_EXACTLY,
    MATCH_BIGGER,
    MATCH_SMALLER
  };

  
  typedef std::map<int, std::string> IconMap;

  ExtensionIconSet();
  ~ExtensionIconSet();

  const IconMap& map() const { return map_; }
  bool empty() const { return map_.empty(); }

  
  void Clear();

  
  // present, it is overwritten.
  void Add(int size, const std::string& path);

  
  
  std::string Get(int size, MatchType match_type) const;

  
  bool ContainsPath(const std::string& path) const;

  
  int GetIconSizeFromPath(const std::string& path) const;

 private:
  IconMap map_;
};

#endif  

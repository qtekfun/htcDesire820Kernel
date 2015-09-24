// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_ICON_SET_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_ICON_SET_H_
#pragma once

#include <map>
#include <string>

class ExtensionIconSet {
 public:
  ExtensionIconSet();
  ~ExtensionIconSet();

  
  typedef std::map<int, std::string> IconMap;
  const IconMap& map() const { return map_; }

  
  void Clear();

  
  // overwritten.
  void Add(int size, const std::string& path);

  
  
  enum MatchType {
    MATCH_EXACTLY,
    MATCH_BIGGER,
    MATCH_SMALLER
  };
  std::string Get(int size, MatchType match_type) const;

  
  bool ContainsPath(const std::string& path) const;

 private:
  IconMap map_;
};

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_PATTERN_H_
#define TOOLS_GN_PATTERN_H_

#include <string>
#include <vector>

#include "tools/gn/value.h"

extern const char kPattern_Help[];

class Pattern {
 public:
  struct Subrange {
    enum Type {
      LITERAL,  
      ANYTHING,  
      PATH_BOUNDARY  
    };

    Subrange(Type t, const std::string& l = std::string())
        : type(t),
          literal(l) {
    }

    
    size_t MinSize() const {
      switch (type) {
        case LITERAL:
          return literal.size();
        case ANYTHING:
          return 0;
        case PATH_BOUNDARY:
          return 0;  
        default:
          return 0;
      }
    }

    Type type;

    
    std::string literal;
  };

  Pattern(const std::string& s);
  ~Pattern();

  
  bool MatchesString(const std::string& s) const;

 private:
  
  
  bool RecursiveMatch(const std::string& s,
                      size_t begin_char,
                      size_t subrange_index,
                      bool allow_implicit_path_boundary) const;

  std::vector<Subrange> subranges_;

  
  
  bool is_suffix_;
};

class PatternList {
 public:
  PatternList();
  ~PatternList();

  bool is_empty() const { return patterns_.empty(); }

  
  
  void SetFromValue(const Value& v, Err* err);

  bool MatchesString(const std::string& s) const;
  bool MatchesValue(const Value& v) const;

 private:
  std::vector<Pattern> patterns_;
};

#endif  

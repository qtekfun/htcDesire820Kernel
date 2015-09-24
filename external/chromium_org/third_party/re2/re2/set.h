// Copyright 2010 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.

#ifndef RE2_SET_H
#define RE2_SET_H

#include <utility>
#include <vector>

#include "re2/re2.h"

namespace re2 {
using std::vector;

class RE2::Set {
 public:
  Set(const RE2::Options& options, RE2::Anchor anchor);
  ~Set();

  
  
  
  
  
  
  
  int Add(const StringPiece& pattern, string* error);

  
  
  
  
  bool Compile();

  
  
  bool Match(const StringPiece& text, vector<int>* v) const;

 private:
  RE2::Options options_;
  RE2::Anchor anchor_;
  vector<re2::Regexp*> re_;
  re2::Prog* prog_;
  bool compiled_;
  
  Set(const Set&);
  void operator=(const Set&);
};

}  

#endif  

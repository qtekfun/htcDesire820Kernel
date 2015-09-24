// Copyright 2009 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_FILTERED_RE2_H_
#define RE2_FILTERED_RE2_H_

#include <vector>
#include "re2/re2.h"

namespace re2 {
using std::vector;

class PrefilterTree;

class FilteredRE2 {
 public:
  FilteredRE2();
  ~FilteredRE2();

  
  
  
  RE2::ErrorCode Add(const StringPiece& pattern,
                     const RE2::Options& options,
                     int *id);

  
  
  
  
  
  
  void Compile(vector<string>* strings_to_match);

  
  
  
  
  int SlowFirstMatch(const StringPiece& text) const;

  
  
  
  int FirstMatch(const StringPiece& text,
                 const vector<int>& atoms) const;

  
  
  bool AllMatches(const StringPiece& text,
                  const vector<int>& atoms,
                  vector<int>* matching_regexps) const;

  
  int NumRegexps() const { return re2_vec_.size(); }

 private:

  
  RE2* GetRE2(int regexpid) const { return re2_vec_[regexpid]; }

  
  void PrintPrefilter(int regexpid);

  
  void RegexpsGivenStrings(const vector<int>& matched_atoms,
                           vector<int>* passed_regexps);

  
  vector<RE2*> re2_vec_;

  
  bool compiled_;

  
  PrefilterTree* prefilter_tree_;

  
  FilteredRE2(const FilteredRE2&);
  void operator=(const FilteredRE2&);
};

}  

#endif  

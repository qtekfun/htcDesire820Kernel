// Copyright 2009 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.

#ifndef RE2_TESTING_EXHAUSTIVE_TESTER_H__
#define RE2_TESTING_EXHAUSTIVE_TESTER_H__

#include <string>
#include <vector>
#include "util/util.h"
#include "re2/testing/regexp_generator.h"
#include "re2/testing/string_generator.h"

namespace re2 {

class ExhaustiveTester : public RegexpGenerator {
 public:
  ExhaustiveTester(int maxatoms,
                   int maxops,
                   const vector<string>& alphabet,
                   const vector<string>& ops,
                   int maxstrlen,
                   const vector<string>& stralphabet,
                   const string& wrapper,
                   const string& topwrapper)
    : RegexpGenerator(maxatoms, maxops, alphabet, ops),
      strgen_(maxstrlen, stralphabet),
      wrapper_(wrapper),
      topwrapper_(topwrapper),
      regexps_(0), tests_(0), failures_(0),
      randomstrings_(0), stringseed_(0), stringcount_(0)  { }

  int regexps()  { return regexps_; }
  int tests()    { return tests_; }
  int failures() { return failures_; }

  
  void HandleRegexp(const string& regexp);

  
  void RandomStrings(int32 seed, int32 count) {
    randomstrings_ = true;
    stringseed_ = seed;
    stringcount_ = count;
  }

 private:
  StringGenerator strgen_;
  string wrapper_;      
  string topwrapper_;   
  int regexps_;   
  int tests_;     
  int failures_;  

  bool randomstrings_;  
  int32 stringseed_;    
  int stringcount_;     
  DISALLOW_EVIL_CONSTRUCTORS(ExhaustiveTester);
};

void ExhaustiveTest(int maxatoms, int maxops,
                    const vector<string>& alphabet,
                    const vector<string>& ops,
                    int maxstrlen, const vector<string>& stralphabet,
                    const string& wrapper,
                    const string& topwrapper);

void EgrepTest(int maxatoms, int maxops, const string& alphabet,
               int maxstrlen, const string& stralphabet,
               const string& wrapper);

}  

#endif  

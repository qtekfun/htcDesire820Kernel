// Copyright 2008 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_TESTING_STRING_GENERATOR_H__
#define RE2_TESTING_STRING_GENERATOR_H__

#include <string>
#include <vector>
#include "util/util.h"
#include "util/random.h"
#include "re2/stringpiece.h"

namespace re2 {

class StringGenerator {
 public:
  StringGenerator(int maxlen, const vector<string>& alphabet);
  ~StringGenerator();
  const StringPiece& Next();
  bool HasNext() { return hasnext_; }

  
  void Reset();

  
  void Random(int32 seed, int n);

  
  void GenerateNULL();

 private:
  bool IncrementDigits();
  bool RandomDigits();

  
  int maxlen_;               
  vector<string> alphabet_;  

  
  StringPiece sp_;           
  string s_;                 
  bool hasnext_;             
  vector<int> digits_;       
  bool generate_null_;       
  bool random_;              
  int nrandom_;              
  ACMRandom* acm_;           
  DISALLOW_EVIL_CONSTRUCTORS(StringGenerator);
};

}  

#endif  

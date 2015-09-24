// Copyright 2008 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_TESTING_REGEXP_GENERATOR_H__
#define RE2_TESTING_REGEXP_GENERATOR_H__

#include <string>
#include <vector>
#include "util/random.h"
#include "util/util.h"
#include "re2/stringpiece.h"

namespace re2 {

class RegexpGenerator {
 public:
  RegexpGenerator(int maxatoms, int maxops, const vector<string>& atoms,
                  const vector<string>& ops);
  virtual ~RegexpGenerator() {}

  
  void Generate();

  
  void GenerateRandom(int32 seed, int n);

  
  virtual void HandleRegexp(const string& regexp) = 0;

  
  static const vector<string>& EgrepOps();

 private:
  void RunPostfix(const vector<string>& post);
  void GeneratePostfix(vector<string>* post, int nstk, int ops, int lits);
  bool GenerateRandomPostfix(vector<string>* post, int nstk, int ops, int lits);

  int maxatoms_;           
  int maxops_;             
  vector<string> atoms_;   
  vector<string> ops_;     
  ACMRandom* acm_;         
  DISALLOW_EVIL_CONSTRUCTORS(RegexpGenerator);
};


vector<string> Explode(const StringPiece& s);

vector<string> Split(const StringPiece& sep, const StringPiece& s);

}  

#endif  

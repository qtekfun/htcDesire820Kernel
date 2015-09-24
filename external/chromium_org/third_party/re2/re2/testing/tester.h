// Copyright 2008 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_TESTING_TESTER_H__
#define RE2_TESTING_TESTER_H__

#include "re2/stringpiece.h"
#include "re2/prog.h"
#include "re2/regexp.h"
#include "re2/re2.h"
#include "util/pcre.h"

namespace re2 {

class Regexp;

enum Engine {
  kEngineBacktrack = 0,    
  kEngineNFA,              
  kEngineDFA,              
  kEngineDFA1,             
  kEngineOnePass,          
  kEngineBitState,         
  kEngineRE2,              
  kEngineRE2a,             
  kEngineRE2b,             
  kEnginePCRE,             

  kEngineMax,
};

static inline void operator++(Engine& e, int unused) {
  e = static_cast<Engine>(e+1);
}

static inline Engine operator+(Engine e, int i) {
  return static_cast<Engine>(static_cast<int>(e)+i);
}

class TestInstance {
 public:
  struct Result;

  TestInstance(const StringPiece& regexp, Prog::MatchKind kind,
               Regexp::ParseFlags flags);
  ~TestInstance();
  Regexp::ParseFlags flags() { return flags_; }
  bool error() { return error_; }

  
  
  bool RunCase(const StringPiece& text, const StringPiece& context,
               Prog::Anchor anchor);

 private:
  
  void RunSearch(Engine type,
                 const StringPiece& text, const StringPiece& context,
                 Prog::Anchor anchor,
                 Result *result);

  void LogMatch(const char* prefix, Engine e, const StringPiece& text,
                const StringPiece& context, Prog::Anchor anchor);

  const StringPiece& regexp_str_;   
  Prog::MatchKind kind_;            
  Regexp::ParseFlags flags_;        
  bool error_;                      

  Regexp* regexp_;                  
  int num_captures_;                
  Prog* prog_;                      
  Prog* rprog_;                     
  PCRE* re_;                        
  RE2* re2_;                        

  DISALLOW_EVIL_CONSTRUCTORS(TestInstance);
};

class Tester {
 public:
  explicit Tester(const StringPiece& regexp);
  ~Tester();

  bool error() { return error_; }

  
  
  bool TestCase(const StringPiece& text, const StringPiece& context,
                Prog::Anchor anchor);

  
  bool TestInput(const StringPiece& text);

  
  bool TestInputInContext(const StringPiece& text, const StringPiece& context);

 private:
  bool error_;
  vector<TestInstance*> v_;

  DISALLOW_EVIL_CONSTRUCTORS(Tester);
};

bool TestRegexpOnText(const StringPiece& regexp, const StringPiece& text);

}  

#endif  

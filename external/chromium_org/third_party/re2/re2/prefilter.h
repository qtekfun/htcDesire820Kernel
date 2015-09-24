// Copyright 2009 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_PREFILTER_H_
#define RE2_PREFILTER_H_

#include "util/util.h"

namespace re2 {

class RE2;

class Regexp;

class Prefilter {
  
 public:
  enum Op {
    ALL = 0,  
    NONE,  
    ATOM,  
    AND,   
    OR,   
  };

  explicit Prefilter(Op op);
  ~Prefilter();

  Op op() { return op_; }
  const string& atom() const { return atom_; }
  void set_unique_id(int id) { unique_id_ = id; }
  int unique_id() const { return unique_id_; }

  
  vector<Prefilter*>* subs() {
    CHECK(op_ == AND || op_ == OR);
    return subs_;
  }

  
  
  void set_subs(vector<Prefilter*>* subs) { subs_ = subs; }

  
  
  
  static Prefilter* FromRE2(const RE2* re2);

  
  string DebugString() const;

 private:
  class Info;

  
  
  static Prefilter* And(Prefilter* a, Prefilter* b);

  
  
  static Prefilter* Or(Prefilter* a, Prefilter* b);

  
  static Prefilter* AndOr(Op op, Prefilter* a, Prefilter* b);

  static Prefilter* FromRegexp(Regexp* a);

  static Prefilter* FromString(const string& str);

  static Prefilter* OrStrings(set<string>* ss);

  static Info* BuildInfo(Regexp* re);

  Prefilter* Simplify();

  
  Op op_;

  
  vector<Prefilter*>* subs_;

  
  string atom_;

  
  
  
  
  
  int unique_id_;

  
  int alloc_id_;

  DISALLOW_EVIL_CONSTRUCTORS(Prefilter);
};

}  

#endif  

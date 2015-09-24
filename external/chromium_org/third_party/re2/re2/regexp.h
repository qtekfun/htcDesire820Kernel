// Copyright 2006 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


// Any operation that traverses the Regexp structures should be written

#ifndef RE2_REGEXP_H__
#define RE2_REGEXP_H__

#include "util/util.h"
#include "re2/stringpiece.h"

namespace re2 {

enum RegexpOp {
  
  kRegexpNoMatch = 1,

  
  kRegexpEmptyMatch,

  
  kRegexpLiteral,

  
  kRegexpLiteralString,

  
  kRegexpConcat,
  
  kRegexpAlternate,

  
  kRegexpStar,
  
  kRegexpPlus,
  
  kRegexpQuest,

  
  
  kRegexpRepeat,

  
  
  kRegexpCapture,

  
  kRegexpAnyChar,

  
  kRegexpAnyByte,

  
  kRegexpBeginLine,
  
  kRegexpEndLine,

  
  kRegexpWordBoundary,
  
  kRegexpNoWordBoundary,

  
  kRegexpBeginText,
  
  kRegexpEndText,

  
  kRegexpCharClass,

  
  
  kRegexpHaveMatch,

  kMaxRegexpOp = kRegexpHaveMatch,
};

enum RegexpStatusCode {
  
  kRegexpSuccess = 0,

  
  kRegexpInternalError,

  
  kRegexpBadEscape,          
  kRegexpBadCharClass,       
  kRegexpBadCharRange,       
  kRegexpMissingBracket,     
  kRegexpMissingParen,       
  kRegexpTrailingBackslash,  
  kRegexpRepeatArgument,     
  kRegexpRepeatSize,         
  kRegexpRepeatOp,           
  kRegexpBadPerlOp,          
  kRegexpBadUTF8,            
  kRegexpBadNamedCapture,    
};

class RegexpStatus {
 public:
  RegexpStatus() : code_(kRegexpSuccess), tmp_(NULL) {}
  ~RegexpStatus() { delete tmp_; }

  void set_code(enum RegexpStatusCode code) { code_ = code; }
  void set_error_arg(const StringPiece& error_arg) { error_arg_ = error_arg; }
  void set_tmp(string* tmp) { delete tmp_; tmp_ = tmp; }
  enum RegexpStatusCode code() const { return code_; }
  const StringPiece& error_arg() const { return error_arg_; }
  bool ok() const { return code() == kRegexpSuccess; }

  
  void Copy(const RegexpStatus& status);

  
  
  static string CodeText(enum RegexpStatusCode code);

  
  
  string Text() const;

 private:
  enum RegexpStatusCode code_;  
  StringPiece error_arg_;       
  string* tmp_;                 

  DISALLOW_EVIL_CONSTRUCTORS(RegexpStatus);
};

class SimplifyWalker;

class Prog;

struct RuneRange {
  RuneRange() : lo(0), hi(0) { }
  RuneRange(int l, int h) : lo(l), hi(h) { }
  Rune lo;
  Rune hi;
};

struct RuneRangeLess {
  bool operator()(const RuneRange& a, const RuneRange& b) const {
    return a.hi < b.lo;
  }
};

class CharClassBuilder;

class CharClass {
 public:
  void Delete();

  typedef RuneRange* iterator;
  iterator begin() { return ranges_; }
  iterator end() { return ranges_ + nranges_; }

  int size() { return nrunes_; }
  bool empty() { return nrunes_ == 0; }
  bool full() { return nrunes_ == Runemax+1; }
  bool FoldsASCII() { return folds_ascii_; }

  bool Contains(Rune r);
  CharClass* Negate();

 private:
  CharClass();  
  ~CharClass();  
  static CharClass* New(int maxranges);

  friend class CharClassBuilder;

  bool folds_ascii_;
  int nrunes_;
  RuneRange *ranges_;
  int nranges_;
  DISALLOW_EVIL_CONSTRUCTORS(CharClass);
};

class Regexp {
 public:

  
  enum ParseFlags {
    NoParseFlags = 0,
    FoldCase     = 1<<0,   
    Literal      = 1<<1,   
    ClassNL      = 1<<2,   
                           
    DotNL        = 1<<3,   
    MatchNL      = ClassNL | DotNL,
    OneLine      = 1<<4,   
                           
                           
    Latin1       = 1<<5,   
    NonGreedy    = 1<<6,   
    PerlClasses  = 1<<7,   
    PerlB        = 1<<8,   
    PerlX        = 1<<9,   
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
                           
    UnicodeGroups = 1<<10, 
                           
    NeverNL      = 1<<11,  
                           
    NeverCapture = 1<<12,  

    
    LikePerl     = ClassNL | OneLine | PerlClasses | PerlB | PerlX |
                   UnicodeGroups,

    
    WasDollar    = 1<<15,  
  };

  
  RegexpOp op() { return static_cast<RegexpOp>(op_); }
  int nsub() { return nsub_; }
  bool simple() { return simple_; }
  enum ParseFlags parse_flags() { return static_cast<ParseFlags>(parse_flags_); }
  int Ref();  

  Regexp** sub() {
    if(nsub_ <= 1)
      return &subone_;
    else
      return submany_;
  }

  int min() { DCHECK_EQ(op_, kRegexpRepeat); return min_; }
  int max() { DCHECK_EQ(op_, kRegexpRepeat); return max_; }
  Rune rune() { DCHECK_EQ(op_, kRegexpLiteral); return rune_; }
  CharClass* cc() { DCHECK_EQ(op_, kRegexpCharClass); return cc_; }
  int cap() { DCHECK_EQ(op_, kRegexpCapture); return cap_; }
  const string* name() { DCHECK_EQ(op_, kRegexpCapture); return name_; }
  Rune* runes() { DCHECK_EQ(op_, kRegexpLiteralString); return runes_; }
  int nrunes() { DCHECK_EQ(op_, kRegexpLiteralString); return nrunes_; }
  int match_id() { DCHECK_EQ(op_, kRegexpHaveMatch); return match_id_; }

  
  Regexp* Incref();

  
  void Decref();

  
  
  
  static Regexp* Parse(const StringPiece& s, ParseFlags flags,
                       RegexpStatus* status);

  
  
  
  // Simplified means that counted repetition has been rewritten
  
  
  
  Regexp* Simplify();
  friend class SimplifyWalker;

  
  
  
  static bool SimplifyRegexp(const StringPiece& src, ParseFlags flags,
                             string* dst,
                             RegexpStatus* status);

  
  int NumCaptures();
  friend class NumCapturesWalker;

  
  
  
  map<string, int>* NamedCaptures();

  
  
  
  map<int, string>* CaptureNames();

  
  
  string ToString();

  
  
  
  static Regexp* Plus(Regexp* sub, ParseFlags flags);
  static Regexp* Star(Regexp* sub, ParseFlags flags);
  static Regexp* Quest(Regexp* sub, ParseFlags flags);
  static Regexp* Concat(Regexp** subs, int nsubs, ParseFlags flags);
  static Regexp* Alternate(Regexp** subs, int nsubs, ParseFlags flags);
  static Regexp* Capture(Regexp* sub, ParseFlags flags, int cap);
  static Regexp* Repeat(Regexp* sub, ParseFlags flags, int min, int max);
  static Regexp* NewLiteral(Rune rune, ParseFlags flags);
  static Regexp* NewCharClass(CharClass* cc, ParseFlags flags);
  static Regexp* LiteralString(Rune* runes, int nrunes, ParseFlags flags);
  static Regexp* HaveMatch(int match_id, ParseFlags flags);

  
  static Regexp* AlternateNoFactor(Regexp** subs, int nsubs, ParseFlags flags);

  
  
  string Dump();

  
  template<typename T> class Walker;

  
  
  
  
  
  Prog* CompileToProg(int64 max_mem);
  Prog* CompileToReverseProg(int64 max_mem);

  
  
  
  
  
  bool MimicsPCRE();

  
  void NullWalk();

  
  
  
  
  bool RequiredPrefix(string* prefix, bool *foldcase, Regexp** suffix);

 private:
  
  explicit Regexp(RegexpOp op, ParseFlags parse_flags);

  
  
  ~Regexp();
  void Destroy();
  bool QuickDestroy();

  
  class ParseState;
  friend class ParseState;
  friend bool ParseCharClass(StringPiece* s, Regexp** out_re,
                             RegexpStatus* status);

  
  friend bool RegexpEqualTestingOnly(Regexp*, Regexp*);

  
  bool ComputeSimple();

  
  
  
  static Regexp* ConcatOrAlternate(RegexpOp op, Regexp** subs, int nsubs,
                                   ParseFlags flags, bool can_factor);

  
  
  
  static Rune* LeadingString(Regexp* re, int* nrune, ParseFlags* flags);

  
  
  static void RemoveLeadingString(Regexp* re, int n);

  
  
  
  static Regexp* LeadingRegexp(Regexp* re);

  
  
  static Regexp* RemoveLeadingRegexp(Regexp* re);

  
  
  static int FactorAlternation(Regexp** sub, int nsub, ParseFlags flags);
  static int FactorAlternationRecursive(Regexp** sub, int nsub,
                                        ParseFlags flags, int maxdepth);

  
  
  
  static bool Equal(Regexp* a, Regexp* b);

  
  void AllocSub(int n) {
    if (n < 0 || static_cast<uint16>(n) != n)
      LOG(FATAL) << "Cannot AllocSub " << n;
    if (n > 1)
      submany_ = new Regexp*[n];
    nsub_ = n;
  }

  
  void AddRuneToString(Rune r);

  
  void Swap(Regexp *that);

  
  
  uint8 op_;

  
  
  
  uint8 simple_;

  
  
  
  uint16 parse_flags_;

  
  
  
  
  
  
  
  
  uint16 ref_;
  static const uint16 kMaxRef = 0xffff;

  
  
  
  
  
  
  uint16 nsub_;
  static const uint16 kMaxNsub = 0xffff;
  union {
    Regexp** submany_;  
    Regexp* subone_;  
  };

  
  Regexp* down_;

  
  union {
    struct {  
      int max_;
      int min_;
    };
    struct {  
      int cap_;
      string* name_;
    };
    struct {  
      int nrunes_;
      Rune* runes_;
    };
    struct {  
      
      
      
      CharClass* cc_;
      CharClassBuilder* ccb_;
    };
    Rune rune_;  
    int match_id_;  
    void *the_union_[2];  
  };

  DISALLOW_EVIL_CONSTRUCTORS(Regexp);
};

typedef set<RuneRange, RuneRangeLess> RuneRangeSet;

class CharClassBuilder {
 public:
  CharClassBuilder();

  typedef RuneRangeSet::iterator iterator;
  iterator begin() { return ranges_.begin(); }
  iterator end() { return ranges_.end(); }

  int size() { return nrunes_; }
  bool empty() { return nrunes_ == 0; }
  bool full() { return nrunes_ == Runemax+1; }

  bool Contains(Rune r);
  bool FoldsASCII();
  bool AddRange(Rune lo, Rune hi);  
  CharClassBuilder* Copy();
  void AddCharClass(CharClassBuilder* cc);
  void Negate();
  void RemoveAbove(Rune r);
  CharClass* GetCharClass();
  void AddRangeFlags(Rune lo, Rune hi, Regexp::ParseFlags parse_flags);

 private:
  static const uint32 AlphaMask = (1<<26) - 1;
  uint32 upper_;  
  uint32 lower_;  
  int nrunes_;
  RuneRangeSet ranges_;
  DISALLOW_EVIL_CONSTRUCTORS(CharClassBuilder);
};

inline Regexp::ParseFlags operator|(Regexp::ParseFlags a, Regexp::ParseFlags b)
{
  return static_cast<Regexp::ParseFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline Regexp::ParseFlags operator^(Regexp::ParseFlags a, Regexp::ParseFlags b)
{
  return static_cast<Regexp::ParseFlags>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline Regexp::ParseFlags operator&(Regexp::ParseFlags a, Regexp::ParseFlags b)
{
  return static_cast<Regexp::ParseFlags>(static_cast<int>(a) & static_cast<int>(b));
}

inline Regexp::ParseFlags operator~(Regexp::ParseFlags a)
{
  return static_cast<Regexp::ParseFlags>(~static_cast<int>(a));
}



}  

#endif  

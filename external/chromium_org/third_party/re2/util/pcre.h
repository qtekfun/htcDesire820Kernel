// Copyright 2003-2010 Google Inc.  All Rights Reserved.
// license that can be found in the LICENSE file.

// This is a variant of PCRE's pcrecpp.h, originally written at Google.


#include "util/util.h"
#include "re2/stringpiece.h"

#ifdef USEPCRE
#include <pcre.h>
namespace re2 {
const bool UsingPCRE = true;
}  
#else
namespace re2 {
const bool UsingPCRE = false;
struct pcre;
struct pcre_extra { int flags, match_limit, match_limit_recursion; };
#define pcre_free(x) {}
#define PCRE_EXTRA_MATCH_LIMIT 0
#define PCRE_EXTRA_MATCH_LIMIT_RECURSION 0
#define PCRE_ANCHORED 0
#define PCRE_NOTEMPTY 0
#define PCRE_ERROR_NOMATCH 1
#define PCRE_ERROR_MATCHLIMIT 2
#define PCRE_ERROR_RECURSIONLIMIT 3
#define PCRE_INFO_CAPTURECOUNT 0
#ifndef WIN32
#define pcre_compile(a,b,c,d,e) ({ (void)(a); (void)(b); *(c)=""; *(d)=0; (void)(e); ((pcre*)0); })
#define pcre_exec(a, b, c, d, e, f, g, h) ({ (void)(a); (void)(b); (void)(c); (void)(d); (void)(e); (void)(f); (void)(g); (void)(h); 0; })
#define pcre_fullinfo(a, b, c, d) ({ (void)(a); (void)(b); (void)(c); *(d) = 0; 0; })
#else
#define pcre_compile(a,b,c,d,e) NULL
#define pcre_exec(a, b, c, d, e, f, g, h) NULL
#define pcre_fullinfo(a, b, c, d) NULL
#endif
}  
#endif

namespace re2 {

class PCRE_Options;

class PCRE {
 public:
  
  class Arg;

  
  
  
  static Arg no_more_args;

  
  
  
  
  
  
  
  
  enum Option {
    None = 0x0000,
    UTF8 = 0x0800,  
    EnabledCompileOptions = UTF8,
    EnabledExecOptions = 0x0000,  
  };

  
  
  PCRE(const char* pattern);
  PCRE(const char* pattern, Option option);
  PCRE(const string& pattern);
  PCRE(const string& pattern, Option option);
  PCRE(const char *pattern, const PCRE_Options& re_option);
  PCRE(const string& pattern, const PCRE_Options& re_option);

  ~PCRE();

  
  
  
  const string& pattern() const { return pattern_; }

  
  
  const string& error() const { return *error_; }

  
  
  
  
  
  bool HitLimit();
  void ClearHitLimit();

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct FullMatchFunctor {
    bool operator ()(const StringPiece& text, const PCRE& re, 
                     const Arg& ptr1 = no_more_args,
                     const Arg& ptr2 = no_more_args,
                     const Arg& ptr3 = no_more_args,
                     const Arg& ptr4 = no_more_args,
                     const Arg& ptr5 = no_more_args,
                     const Arg& ptr6 = no_more_args,
                     const Arg& ptr7 = no_more_args,
                     const Arg& ptr8 = no_more_args,
                     const Arg& ptr9 = no_more_args,
                     const Arg& ptr10 = no_more_args,
                     const Arg& ptr11 = no_more_args,
                     const Arg& ptr12 = no_more_args,
                     const Arg& ptr13 = no_more_args,
                     const Arg& ptr14 = no_more_args,
                     const Arg& ptr15 = no_more_args,
                     const Arg& ptr16 = no_more_args) const;
  };

  static const FullMatchFunctor FullMatch;

  
  
  struct PartialMatchFunctor {
    bool operator ()(const StringPiece& text, const PCRE& re, 
                     const Arg& ptr1 = no_more_args,
                     const Arg& ptr2 = no_more_args,
                     const Arg& ptr3 = no_more_args,
                     const Arg& ptr4 = no_more_args,
                     const Arg& ptr5 = no_more_args,
                     const Arg& ptr6 = no_more_args,
                     const Arg& ptr7 = no_more_args,
                     const Arg& ptr8 = no_more_args,
                     const Arg& ptr9 = no_more_args,
                     const Arg& ptr10 = no_more_args,
                     const Arg& ptr11 = no_more_args,
                     const Arg& ptr12 = no_more_args,
                     const Arg& ptr13 = no_more_args,
                     const Arg& ptr14 = no_more_args,
                     const Arg& ptr15 = no_more_args,
                     const Arg& ptr16 = no_more_args) const;
  };

  static const PartialMatchFunctor PartialMatch;

  
  
  
  struct ConsumeFunctor {
    bool operator ()(StringPiece* input, const PCRE& pattern, 
                     const Arg& ptr1 = no_more_args,
                     const Arg& ptr2 = no_more_args,
                     const Arg& ptr3 = no_more_args,
                     const Arg& ptr4 = no_more_args,
                     const Arg& ptr5 = no_more_args,
                     const Arg& ptr6 = no_more_args,
                     const Arg& ptr7 = no_more_args,
                     const Arg& ptr8 = no_more_args,
                     const Arg& ptr9 = no_more_args,
                     const Arg& ptr10 = no_more_args,
                     const Arg& ptr11 = no_more_args,
                     const Arg& ptr12 = no_more_args,
                     const Arg& ptr13 = no_more_args,
                     const Arg& ptr14 = no_more_args,
                     const Arg& ptr15 = no_more_args,
                     const Arg& ptr16 = no_more_args) const;
  };

  static const ConsumeFunctor Consume;

  
  
  
  
  struct FindAndConsumeFunctor {
    bool operator ()(StringPiece* input, const PCRE& pattern,
                     const Arg& ptr1 = no_more_args,
                     const Arg& ptr2 = no_more_args,
                     const Arg& ptr3 = no_more_args,
                     const Arg& ptr4 = no_more_args,
                     const Arg& ptr5 = no_more_args,
                     const Arg& ptr6 = no_more_args,
                     const Arg& ptr7 = no_more_args,
                     const Arg& ptr8 = no_more_args,
                     const Arg& ptr9 = no_more_args,
                     const Arg& ptr10 = no_more_args,
                     const Arg& ptr11 = no_more_args,
                     const Arg& ptr12 = no_more_args,
                     const Arg& ptr13 = no_more_args,
                     const Arg& ptr14 = no_more_args,
                     const Arg& ptr15 = no_more_args,
                     const Arg& ptr16 = no_more_args) const;
  };

  static const FindAndConsumeFunctor FindAndConsume;

  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool Replace(string *str,
                      const PCRE& pattern,
                      const StringPiece& rewrite);

  
  
  
  
  
  
  
  
  
  
  static int GlobalReplace(string *str,
                           const PCRE& pattern,
                           const StringPiece& rewrite);

  
  
  
  
  
  
  static bool Extract(const StringPiece &text,
                      const PCRE& pattern,
                      const StringPiece &rewrite,
                      string *out);

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool CheckRewriteString(const StringPiece& rewrite, string* error) const;

  
  
  
  
  
  
  
  static string QuoteMeta(const StringPiece& unquoted);

  

  
  enum Anchor {
    UNANCHORED,         
    ANCHOR_START,       
    ANCHOR_BOTH,        
  };

  
  
  bool DoMatch(const StringPiece& text,
               Anchor anchor,
               int* consumed,
               const Arg* const* args, int n) const;

  
  
  int NumberOfCapturingGroups() const;

 private:
  void Init(const char* pattern, Option option, int match_limit,
            int stack_limit, bool report_errors);

  
  
  
  
  
  
  
  
  
  
  
  int TryMatch(const StringPiece& text,
               int startpos,
               Anchor anchor,
               bool empty_ok,
               int *vec,
               int vecsize) const;

  
  
  bool Rewrite(string *out,
               const StringPiece &rewrite,
               const StringPiece &text,
               int *vec,
               int veclen) const;

  
  bool DoMatchImpl(const StringPiece& text,
                   Anchor anchor,
                   int* consumed,
                   const Arg* const args[],
                   int n,
                   int* vec,
                   int vecsize) const;

  
  pcre* Compile(Anchor anchor);

  string            pattern_;
  Option            options_;
  pcre*             re_full_;        
  pcre*             re_partial_;     
  const string*     error_;          
  bool              report_errors_;  
  int               match_limit_;    
  int               stack_limit_;    
  mutable int32_t  hit_limit_;  
  DISALLOW_EVIL_CONSTRUCTORS(PCRE);
};

class PCRE_Options {
 public:
  
  PCRE_Options() : option_(PCRE::None), match_limit_(0), stack_limit_(0), report_errors_(true) {}
  
  PCRE::Option option() const { return option_; }
  void set_option(PCRE::Option option) {
    option_ = option;
  }
  int match_limit() const { return match_limit_; }
  void set_match_limit(int match_limit) {
    match_limit_ = match_limit;
  }
  int stack_limit() const { return stack_limit_; }
  void set_stack_limit(int stack_limit) {
    stack_limit_ = stack_limit;
  }

  
  
  bool report_errors() const { return report_errors_; }
  void set_report_errors(bool report_errors) {
    report_errors_ = report_errors;
  }
 private:
  PCRE::Option option_;
  int match_limit_;
  int stack_limit_;
  bool report_errors_;
};




template <class T>
class _PCRE_MatchObject {
 public:
  static inline bool Parse(const char* str, int n, void* dest) {
    if (dest == NULL) return true;
    T* object = reinterpret_cast<T*>(dest);
    return object->ParseFrom(str, n);
  }
};

class PCRE::Arg {
 public:
  
  Arg();

  
  Arg(void*);

  typedef bool (*Parser)(const char* str, int n, void* dest);

#define MAKE_PARSER(type,name) \
  Arg(type* p) : arg_(p), parser_(name) { } \
  Arg(type* p, Parser parser) : arg_(p), parser_(parser) { } \


  MAKE_PARSER(char,               parse_char);
  MAKE_PARSER(unsigned char,      parse_uchar);
  MAKE_PARSER(short,              parse_short);
  MAKE_PARSER(unsigned short,     parse_ushort);
  MAKE_PARSER(int,                parse_int);
  MAKE_PARSER(unsigned int,       parse_uint);
  MAKE_PARSER(long,               parse_long);
  MAKE_PARSER(unsigned long,      parse_ulong);
  MAKE_PARSER(long long,          parse_longlong);
  MAKE_PARSER(unsigned long long, parse_ulonglong);
  MAKE_PARSER(float,              parse_float);
  MAKE_PARSER(double,             parse_double);
  MAKE_PARSER(string,             parse_string);
  MAKE_PARSER(StringPiece,        parse_stringpiece);

#undef MAKE_PARSER

  
  template <class T> Arg(T*, Parser parser);
  
  template <class T> Arg(T* p)
    : arg_(p), parser_(_PCRE_MatchObject<T>::Parse) {
  }

  
  bool Parse(const char* str, int n) const;

 private:
  void*         arg_;
  Parser        parser_;

  static bool parse_null          (const char* str, int n, void* dest);
  static bool parse_char          (const char* str, int n, void* dest);
  static bool parse_uchar         (const char* str, int n, void* dest);
  static bool parse_float         (const char* str, int n, void* dest);
  static bool parse_double        (const char* str, int n, void* dest);
  static bool parse_string        (const char* str, int n, void* dest);
  static bool parse_stringpiece   (const char* str, int n, void* dest);

#define DECLARE_INTEGER_PARSER(name)                                        \
 private:                                                                   \
  static bool parse_ ## name(const char* str, int n, void* dest);           \
  static bool parse_ ## name ## _radix(                                     \
    const char* str, int n, void* dest, int radix);                         \
 public:                                                                    \
  static bool parse_ ## name ## _hex(const char* str, int n, void* dest);   \
  static bool parse_ ## name ## _octal(const char* str, int n, void* dest); \
  static bool parse_ ## name ## _cradix(const char* str, int n, void* dest)

  DECLARE_INTEGER_PARSER(short);
  DECLARE_INTEGER_PARSER(ushort);
  DECLARE_INTEGER_PARSER(int);
  DECLARE_INTEGER_PARSER(uint);
  DECLARE_INTEGER_PARSER(long);
  DECLARE_INTEGER_PARSER(ulong);
  DECLARE_INTEGER_PARSER(longlong);
  DECLARE_INTEGER_PARSER(ulonglong);

#undef DECLARE_INTEGER_PARSER
};

inline PCRE::Arg::Arg() : arg_(NULL), parser_(parse_null) { }
inline PCRE::Arg::Arg(void* p) : arg_(p), parser_(parse_null) { }

inline bool PCRE::Arg::Parse(const char* str, int n) const {
  return (*parser_)(str, n, arg_);
}

#define MAKE_INTEGER_PARSER(type, name) \
  inline PCRE::Arg Hex(type* ptr) { \
    return PCRE::Arg(ptr, PCRE::Arg::parse_ ## name ## _hex); } \
  inline PCRE::Arg Octal(type* ptr) { \
    return PCRE::Arg(ptr, PCRE::Arg::parse_ ## name ## _octal); } \
  inline PCRE::Arg CRadix(type* ptr) { \
    return PCRE::Arg(ptr, PCRE::Arg::parse_ ## name ## _cradix); }

MAKE_INTEGER_PARSER(short,              short);
MAKE_INTEGER_PARSER(unsigned short,     ushort);
MAKE_INTEGER_PARSER(int,                int);
MAKE_INTEGER_PARSER(unsigned int,       uint);
MAKE_INTEGER_PARSER(long,               long);
MAKE_INTEGER_PARSER(unsigned long,      ulong);
MAKE_INTEGER_PARSER(long long,          longlong);
MAKE_INTEGER_PARSER(unsigned long long, ulonglong);

#undef MAKE_INTEGER_PARSER

}  

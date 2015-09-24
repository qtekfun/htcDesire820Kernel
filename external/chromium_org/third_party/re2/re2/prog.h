// Copyright 2007 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_PROG_H__
#define RE2_PROG_H__

#include "util/util.h"
#include "re2/re2.h"

namespace re2 {

template<int Bits>
class Bitmap {
 public:
  Bitmap() { Reset(); }
  int Size() { return Bits; }

  void Reset() {
    for (int i = 0; i < Words; i++)
      w_[i] = 0;
  }
  bool Get(int k) const {
    return w_[k >> WordLog] & (1<<(k & 31));
  }
  void Set(int k) {
    w_[k >> WordLog] |= 1<<(k & 31);
  }
  void Clear(int k) {
    w_[k >> WordLog] &= ~(1<<(k & 31));
  }
  uint32 Word(int i) const {
    return w_[i];
  }

 private:
  static const int WordLog = 5;
  static const int Words = (Bits+31)/32;
  uint32 w_[Words];
  DISALLOW_EVIL_CONSTRUCTORS(Bitmap);
};


enum InstOp {
  kInstAlt = 0,      
  kInstAltMatch,     
  kInstByteRange,    
  kInstCapture,      
  kInstEmptyWidth,   
  kInstMatch,        
  kInstNop,          
  kInstFail,         
};

enum EmptyOp {
  kEmptyBeginLine        = 1<<0,      
  kEmptyEndLine          = 1<<1,      
  kEmptyBeginText        = 1<<2,      
  kEmptyEndText          = 1<<3,      
  kEmptyWordBoundary     = 1<<4,      
  kEmptyNonWordBoundary  = 1<<5,      
  kEmptyAllFlags         = (1<<6)-1,
};

class Regexp;

class DFA;
struct OneState;

class Prog {
 public:
  Prog();
  ~Prog();

  
  class Inst {
   public:
    Inst() : out_opcode_(0), out1_(0) { }

    
    void InitAlt(uint32 out, uint32 out1);
    void InitByteRange(int lo, int hi, int foldcase, uint32 out);
    void InitCapture(int cap, uint32 out);
    void InitEmptyWidth(EmptyOp empty, uint32 out);
    void InitMatch(int id);
    void InitNop(uint32 out);
    void InitFail();

    
    int id(Prog* p) { return this - p->inst_; }
    InstOp opcode() { return static_cast<InstOp>(out_opcode_&7); }
    int out()     { return out_opcode_>>3; }
    int out1()    { DCHECK(opcode() == kInstAlt || opcode() == kInstAltMatch); return out1_; }
    int cap()       { DCHECK_EQ(opcode(), kInstCapture); return cap_; }
    int lo()        { DCHECK_EQ(opcode(), kInstByteRange); return lo_; }
    int hi()        { DCHECK_EQ(opcode(), kInstByteRange); return hi_; }
    int foldcase()  { DCHECK_EQ(opcode(), kInstByteRange); return foldcase_; }
    int match_id()  { DCHECK_EQ(opcode(), kInstMatch); return match_id_; }
    EmptyOp empty() { DCHECK_EQ(opcode(), kInstEmptyWidth); return empty_; }
    bool greedy(Prog *p) {
      DCHECK_EQ(opcode(), kInstAltMatch);
      return p->inst(out())->opcode() == kInstByteRange;
    }

    
    inline bool Matches(int c) {
      DCHECK_EQ(opcode(), kInstByteRange);
      if (foldcase_ && 'A' <= c && c <= 'Z')
        c += 'a' - 'A';
      return lo_ <= c && c <= hi_;
    }

    
    string Dump();

    
    
    static const int kMaxInst = (1<<28) - 1;

   private:
    void set_opcode(InstOp opcode) {
      out_opcode_ = (out()<<3) | opcode;
    }

    void set_out(int out) {
      out_opcode_ = (out<<3) | opcode();
    }

    void set_out_opcode(int out, InstOp opcode) {
      out_opcode_ = (out<<3) | opcode;
    }

    uint32 out_opcode_;  
    union {              
      uint32 out1_;      
                         

      int32 cap_;        
                         
                         
                         
                         
                         

      int32 match_id_;   
                         

      struct {           
        uint8 lo_;       
        uint8 hi_;       
        uint8 foldcase_; 
      };

      EmptyOp empty_;    
                         
    };

    friend class Compiler;
    friend struct PatchList;
    friend class Prog;

    DISALLOW_EVIL_CONSTRUCTORS(Inst);
  };

  
  enum Anchor {
    kUnanchored,  
    kAnchored,    
  };

  
  
  
  
  
  
  
  
  
  
  
  
  enum MatchKind {
    kFirstMatch,     
    kLongestMatch,   
    kFullMatch,      
    kManyMatch       
  };

  Inst *inst(int id) { return &inst_[id]; }
  int start() { return start_; }
  int start_unanchored() { return start_unanchored_; }
  void set_start(int start) { start_ = start; }
  void set_start_unanchored(int start) { start_unanchored_ = start; }
  int64 size() { return size_; }
  bool reversed() { return reversed_; }
  void set_reversed(bool reversed) { reversed_ = reversed; }
  int64 byte_inst_count() { return byte_inst_count_; }
  const Bitmap<256>& byterange() { return byterange_; }
  void set_dfa_mem(int64 dfa_mem) { dfa_mem_ = dfa_mem; }
  int64 dfa_mem() { return dfa_mem_; }
  int flags() { return flags_; }
  void set_flags(int flags) { flags_ = flags; }
  bool anchor_start() { return anchor_start_; }
  void set_anchor_start(bool b) { anchor_start_ = b; }
  bool anchor_end() { return anchor_end_; }
  void set_anchor_end(bool b) { anchor_end_ = b; }
  int bytemap_range() { return bytemap_range_; }
  const uint8* bytemap() { return bytemap_; }

  
  string Dump();
  string DumpUnanchored();

  
  
  
  
  void MarkByteRange(int lo, int hi);

  
  
  static uint32 EmptyFlags(const StringPiece& context, const char* p);

  
  
  
  
  
  
  
  static bool IsWordChar(uint8 c) {
    return ('A' <= c && c <= 'Z') ||
           ('a' <= c && c <= 'z') ||
           ('0' <= c && c <= '9') ||
           c == '_';
  }

  
  
  
  
  
  
  
  
  
  
  
  
  

  
  bool SearchNFA(const StringPiece& text, const StringPiece& context,
                 Anchor anchor, MatchKind kind,
                 StringPiece* match, int nmatch);

  
  
  
  
  
  
  bool SearchDFA(const StringPiece& text, const StringPiece& context,
                 Anchor anchor, MatchKind kind,
                 StringPiece* match0, bool* failed,
                 vector<int>* matches);

  
  
  
  
  int BuildEntireDFA(MatchKind kind);

  
  void ComputeByteMap();

  
  void Optimize();

  
  
  
  bool IsOnePass();
  bool SearchOnePass(const StringPiece& text, const StringPiece& context,
                     Anchor anchor, MatchKind kind,
                     StringPiece* match, int nmatch);

  
  
  bool SearchBitState(const StringPiece& text, const StringPiece& context,
                      Anchor anchor, MatchKind kind,
                      StringPiece* match, int nmatch);

  static const int kMaxOnePassCapture = 5;  

  
  
  
  
  
  
  bool UnsafeSearchBacktrack(const StringPiece& text,
                             const StringPiece& context,
                             Anchor anchor, MatchKind kind,
                             StringPiece* match, int nmatch);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool PossibleMatchRange(string* min, string* max, int maxlen);

  
  
  static Prog* CompileSet(const RE2::Options& options, RE2::Anchor anchor,
                          Regexp* re);

 private:
  friend class Compiler;

  DFA* GetDFA(MatchKind kind);

  bool anchor_start_;       
  bool anchor_end_;         
  bool reversed_;           
  bool did_onepass_;        

  int start_;               
  int start_unanchored_;    
  int size_;                
  int byte_inst_count_;     
  int bytemap_range_;       
  int flags_;               
  int onepass_statesize_;   

  Inst* inst_;              

  Mutex dfa_mutex_;    
  DFA* volatile dfa_first_;     
  DFA* volatile dfa_longest_;   
  int64 dfa_mem_;      
  void (*delete_dfa_)(DFA* dfa);

  Bitmap<256> byterange_;    
                             
  uint8 bytemap_[256];       
  uint8 *unbytemap_;         

  uint8* onepass_nodes_;     
  OneState* onepass_start_;  

  DISALLOW_EVIL_CONSTRUCTORS(Prog);
};

}  

#endif  

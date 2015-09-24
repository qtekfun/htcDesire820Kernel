// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Copyright (c) 1994-2006 Sun Microsystems Inc.
// - Redistributions of source code must retain the above copyright notice,
// - Redistribution in binary form must reproduce the above copyright
// specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR

// The original source code covered by the above license above has been
// Copyright 2006-2008 the V8 project authors. All rights reserved.


#ifndef TRACELINE_ASSEMBLER_H_
#define TRACELINE_ASSEMBLER_H_

#include <windows.h>
#include <stdio.h>
#include <string>

#include "logging.h"

#define ASSERT(x) CHECK(x)

enum Register {
  EAX = 0,
  ECX = 1,
  EDX = 2,
  EBX = 3,
  ESP = 4,
  EBP = 5,
  ESI = 6,
  EDI = 7
};

enum Condition {
  overflow      =  0,
  no_overflow   =  1,
  below         =  2,
  above_equal   =  3,
  equal         =  4,
  not_equal     =  5,
  below_equal   =  6,
  above         =  7,
  sign          =  8,
  not_sign      =  9,
  parity_even   = 10,
  parity_odd    = 11,
  less          = 12,
  greater_equal = 13,
  less_equal    = 14,
  greater       = 15,

  
  zero          = equal,
  not_zero      = not_equal,
  negative      = sign,
  positive      = not_sign
};

class Label {
 public:
  Label() { Unuse(); }
  ~Label() { ASSERT(!is_linked()); }

  void Unuse() {
    num_ = 0;
  }

  bool is_unused() const { return num_ == 0; }
  bool is_bound() const { return num_ == -1; }
  bool is_linked() const { return num_ > 0; }

  int binding_pos() const {
    ASSERT(is_bound());
    return table_[0];
  }

  int num_links() const {
    ASSERT(!is_bound());
    return num_;  
  }

  int link_pos(int i) const {
    ASSERT(is_linked());
    ASSERT(i < num_);
    return table_[i];
  }

 private:
  void bind_to(int pos)  {
    ASSERT(!is_bound());
    table_[0] = pos;
    num_ = -1;
  }
  void link_to(int pos)  {
    ASSERT(!is_bound());
    ASSERT(num_ < kTableSize);

    table_[num_] = pos;
    ++num_;
  }

  static const int kTableSize = 3;

  
  
  int table_[kTableSize];
  
  
  int num_;

  friend class CodeBuffer;  
};


enum ScaleFactor {
  SCALE_TIMES_1 = 0,
  SCALE_TIMES_2 = 1,
  SCALE_TIMES_4 = 2,
  SCALE_TIMES_8 = 3
};


class Operand {
 public:
  explicit Operand(const Operand& x) : len_(x.len_) {
    memcpy(buf_, x.buf_, sizeof(buf_));
  }

  
  explicit Operand(Register reg) {
    Init(reg);
  }

  
  explicit Operand(int disp) {
    Init(disp);
  }

  
  Operand(Register base, int disp) {
    Init(base, disp);
  }

  
  Operand(Register base, Register index, ScaleFactor scale, int disp) {
    Init(base, index, scale, disp);
  }

  
  Operand(Register index, ScaleFactor scale, int disp) {
    Init(index, scale, disp);
  }

  void set_reg(Register reg) {
    ASSERT(len_ > 0);
    buf_[0] = (buf_[0] & ~0x38) | static_cast<char>(reg << 3);
  }

  char* data() { return buf_; }
  int length() { return len_; }

 private:
  
  void Init(Register reg) {
    set_modrm(3, reg);
  }

  
  void Init(int disp) {
    set_modrm(0, EBP);
    set_dispr(disp);
  }

  
  void Init(Register base, int disp) {
    if (disp == 0) {
      
      set_modrm(0, base);
      if (base == ESP) set_sib(SCALE_TIMES_1, ESP, base);
    } else if (is_int8(disp)) {
      
      set_modrm(1, base);
      if (base == ESP) set_sib(SCALE_TIMES_1, ESP, base);
      set_disp8(disp);
    } else {
      
      set_modrm(2, base);
      if (base == ESP) set_sib(SCALE_TIMES_1, ESP, base);
      set_dispr(disp);
    }
  }

  
  void Init(Register base,
            Register index,
            ScaleFactor scale,
            int disp) {
    ASSERT(index != ESP);  
    if (disp == 0 && base != EBP) {
      
      set_modrm(0, ESP);
      set_sib(scale, index, base);
    } else if (is_int8(disp)) {
      
      set_modrm(1, ESP);
      set_sib(scale, index, base);
      set_disp8(disp);
    } else {
      
      set_modrm(2, ESP);
      set_sib(scale, index, base);
      set_dispr(disp);
    }
  }

  
  void Init(Register index,
            ScaleFactor scale,
            int disp) {
    ASSERT(index != ESP);  
    
    
    
    
    if (scale == SCALE_TIMES_2) {
      Init(index, index, SCALE_TIMES_1, disp);
    } else {
      set_modrm(0, ESP);
      set_sib(scale, index, EBP);
      set_dispr(disp);
    }
  }

  
  bool is_reg(Register reg) const {
    return ((buf_[0] & 0xF8) == 0xC0)  
        && ((buf_[0] & 0x07) == reg);  
  }

  void set_modrm(int mod, Register rm) {  
    ASSERT((mod & -4) == 0);
    buf_[0] = mod << 6 | rm;
    len_ = 1;
  }

  void set_sib(ScaleFactor scale, Register index, Register base) {
    ASSERT(len_ == 1);
    ASSERT((scale & -4) == 0);
    buf_[1] = scale << 6 | index << 3 | base;
    len_ = 2;
  }

  void set_disp8(char disp) {
    ASSERT(len_ == 1 || len_ == 2);
    *reinterpret_cast<char*>(&buf_[len_++]) = disp;
  }

  void set_dispr(int disp) {
    ASSERT(len_ == 1 || len_ == 2);
    *reinterpret_cast<int*>(&buf_[len_]) = disp;
    len_ += sizeof(int);
  }

  bool is_int8(int x) { return x >= -128 && x <= 127; }

  
  char buf_[6];
  
  unsigned int len_;
};

class CodeBuffer {
 public:
  
  explicit CodeBuffer(char* buf) : pos_(0), buf_(buf) { }

  void* data() { return buf_; }
  int size() { return pos_; }

  void emit(unsigned char b) {
    buf_[pos_++] = b;
  }
  void emit_word(unsigned short w) {
    *reinterpret_cast<unsigned short*>(&buf_[pos_]) = w;
    pos_ += 2;
  }
  void emit_dword(unsigned int d) {
    *reinterpret_cast<unsigned int*>(&buf_[pos_]) = d;
    pos_ += 4;
  }

  void emit_bytes(const char* bytes, size_t size) {
    for (size_t i = 0; i < size; ++i)
      emit(bytes[i]);
  }

  void emit_bytes(const std::string& bytes) {
    emit_bytes(bytes.data(), bytes.size());
  }

  void put_dword_at(int pos, unsigned int d) {
    *reinterpret_cast<unsigned int*>(&buf_[pos]) = d;
  }

  
  void emit_operand(Register reg, Operand operand) {
    operand.set_reg(reg);
    memcpy(&buf_[pos_], operand.data(), operand.length());
    pos_ += operand.length();
  }

  void bind(Label* l) {
    ASSERT(!l->is_bound());
    for (int i = 0; i < l->num_links(); ++i) {
      put_dword_at(l->link_pos(i), pos_ - (l->link_pos(i) + 4));
    }
    l->bind_to(pos_);
  }

  

  void add(Register dst, Register src) {
    emit(0x01); emit(0xc0 | (src << 3) | dst);
  }
  void add_imm(Register dst, int d) {
    if (d >= -128 && d <= 127) {
      emit(0x83); emit(0xc0 | dst); emit(d & 0xff);
    } else {
      emit(0x81); emit(0xc0 | dst); emit_dword(d);
    }
  }

  void and_(Register r, unsigned int mask) {
    emit(0x81); emit(0xe0 | r); emit_dword(mask);
  }

  void call(Register r) {
    call(Operand(r));
  }
  void call(const Operand& dst) {
    emit(0xff); emit_operand(EDX, dst);
  }

  void cmp(Register r1, Register r2) {
    emit(0x39); emit(0xc0 | (r2 << 3) | r1);
  }

  void cmp_imm(Register r, int d) {
    if (d >= -128 && d <= 127) {
      emit(0x83); emit(0xf8 | r); emit(d & 0xff);
    } else {
      emit(0x81); emit(0xf8 | r); emit_dword(d);
    }
  }

  void fs() {
    emit(0x64);
  }

  
  
  void inc_atomic(Register mem, Register inc) {
    
    emit(0xF0); emit(0x0F); emit(0xC1); emit((inc << 3) | mem);
  }

  void int3() {
    emit(0xcc);
  }

  void jcc(Condition cc, Label* l) {
    emit(0x0f); emit(0x80 | cc);
    if (l->is_bound()) {
      emit_dword(l->binding_pos() - (pos_ + 4));
    } else {
      
      l->link_to(pos_);
      emit_dword(0);
    }
  }

  void jmp(Register r) {
    emit(0xff); emit(0xe0 | r);
  }

  void jmp(Label* l) {
    if (l->is_bound()) {
      jmp_rel(l->binding_pos() - (pos_ + 5));
    } else {
      
      l->link_to(pos_ + 1);
      jmp_rel(0);
    }
  }

  void jmp_rel(int i) {
    emit(0xe9); emit_dword(i);
  }

  void jmp_rel_short(char c) {
    emit(0xeb); emit(c);
  }

  void lea(Register dst, const Operand& src) {
    emit(0x8d); emit_operand(dst, src);
  }

  void lodsb() {
    emit(0xac);
  }
  void lodsd() {
    emit(0xad);
  }

  void loop(Label* l) {
    ASSERT(l->is_bound());
    int pos = l->binding_pos() - (pos_ + 2);
    ASSERT(pos >= -128 && pos < 0);

    emit(0xe2); emit(pos & 0xff);
  }

  void mov(Register dst, Register src) {
    emit(0x89); emit(0xc0 | (src << 3) | dst);
  }
  void mov(Register dst, const Operand& src) {
    emit(0x8b); emit_operand(dst, src);
  }
  void mov_imm(Register r, unsigned int d) {
    emit(0xb8 | r); emit_dword(d);
  }

  void movsb() {
    emit(0xa4);
  }
  void movsd() {
    emit(0xa5);
  }

  void or_(Register r, unsigned int mask) {
    emit(0x81); emit(0xc8 | r); emit_dword(mask);
  }

  void pop(Register r) {
    emit(0x58 | r);
  }
  void pop(const Operand& dst) {
    emit(0x8f); emit_operand(EAX, dst);
  }

  void push(Register r) {
    emit(0x50 | r);
  }
  void push(const Operand& src) {
    emit(0xff); emit_operand(ESI, src);
  }
  void push_imm(int i) {
    if (i >= -128 && i <= 127) {
      emit(0x6a); emit(i & 0xff);
    } else {
      emit(0x68); emit_dword(i);
    }
  }

  
  void rdtsc() {
    emit(0x0F); emit(0x31);
  }

  void rep() {
    emit(0xf3);
  }

  void ret() {
    ret(0);
  }
  void ret(short c) {
    if (c == 0) {
      emit(0xc3);
    } else {
      emit(0xc2); emit_word(c);
    }
  }

  void spin() {
    jmp_rel_short(-2);
  }

  void stosb() {
    emit(0xaa);
  }
  void stosd() {
    emit(0xab);
  }

  void sysenter() {
    emit(0x0f); emit(0x34);
  }

  
  void which_cpu() {
    
    push(EAX);
    push(EAX);
    
    emit(0x0f); emit(0x01); emit_operand(ECX, Operand(ESP, 2));
    pop(EAX);
    pop(EAX);  
  }

  
  void which_thread() {
    
    emit(0x64); emit(0xa1); emit_dword(0x24);
    
    
  }

  void xchg(Register r1, Register r2) {
    if (r1 == EAX) {
      emit(0x90 | r2);
    } else if (r2 == EAX) {
      emit(0x90 | r1);
    } else {
      xchg(r1, Operand(r2));
    }
  }
  void xchg(Register r1, const Operand& oper) {
    emit(0x87); emit_operand(r1, oper);
  }

 private:
  int pos_;
  char* buf_;
};

#endif  

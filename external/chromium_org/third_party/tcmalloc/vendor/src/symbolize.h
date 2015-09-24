// Copyright (c) 2009, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_SYMBOLIZE_H_
#define TCMALLOC_SYMBOLIZE_H_

#include "config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>  
#endif
#include <stddef.h>  
#include <map>

using std::map;

class SymbolTable {
 public:
  SymbolTable()
    : symbol_buffer_(NULL) {}
  ~SymbolTable() {
    delete[] symbol_buffer_;
  }

  
  
  void Add(const void* addr);

  
  
  
  const char* GetSymbol(const void* addr);

  
  
  int Symbolize();

 private:
  typedef map<const void*, const char*> SymbolMap;

  
  static const int kSymbolSize = 1024;

  
  SymbolMap symbolization_table_;

  
  char *symbol_buffer_;
};

#endif  

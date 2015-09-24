// Copyright (c) 2009, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_STACK_TRACE_TABLE_H_
#define TCMALLOC_STACK_TRACE_TABLE_H_

#include <config.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>                     
#endif
#include "common.h"

namespace tcmalloc {

class PERFTOOLS_DLL_DECL StackTraceTable {
 public:
  
  StackTraceTable();
  ~StackTraceTable();

  
  
  
  void AddTrace(const StackTrace& t);

  
  
  
  
  void** ReadStackTracesAndClear();

  
  struct Bucket {
    
    uintptr_t hash;
    StackTrace trace;

    
    int count;
    Bucket* next;

    bool KeyEqual(uintptr_t h, const StackTrace& t) const;
  };

  
  int depth_total() const { return depth_total_; }
  int bucket_total() const { return bucket_total_; }

 private:
  static const int kHashTableSize = 1 << 14; 

  bool error_;
  int depth_total_;
  int bucket_total_;
  Bucket** table_;
};

}  

#endif  

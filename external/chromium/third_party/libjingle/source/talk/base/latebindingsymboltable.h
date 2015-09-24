/*
 * libjingle
 * Copyright 2004--2010, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_BASE_LATEBINDINGSYMBOLTABLE_H_
#define TALK_BASE_LATEBINDINGSYMBOLTABLE_H_

#include <stddef.h>  
#include <string.h>

#include "talk/base/common.h"
#include "talk/base/logging.h"


namespace talk_base {

#ifdef LINUX
typedef void *DllHandle;

const DllHandle kInvalidDllHandle = NULL;
#else
#error Not implemented
#endif

DllHandle InternalLoadDll(const char dll_name[]);

void InternalUnloadDll(DllHandle handle);

bool InternalLoadSymbols(DllHandle handle,
                         int num_symbols,
                         const char *const symbol_names[],
                         void *symbols[]);

template <int SYMBOL_TABLE_SIZE,
          const char kDllName[],
          const char *const kSymbolNames[]>
class LateBindingSymbolTable {
 public:
  LateBindingSymbolTable()
      : handle_(kInvalidDllHandle),
        undefined_symbols_(false) {
    memset(symbols_, 0, sizeof(symbols_));
  }

  ~LateBindingSymbolTable() {
    Unload();
  }

  static int NumSymbols() {
    return SYMBOL_TABLE_SIZE;
  }

  
  static const char *GetSymbolName(int index) {
    ASSERT(index < NumSymbols());
    return kSymbolNames[index];
  }

  bool IsLoaded() const {
    return handle_ != kInvalidDllHandle;
  }

  
  
  bool Load() {
    if (IsLoaded()) {
      return true;
    }
    if (undefined_symbols_) {
      
      
      LOG(LS_ERROR) << "We know there are undefined symbols";
      return false;
    }
    handle_ = InternalLoadDll(kDllName);
    if (!IsLoaded()) {
      return false;
    }
    if (!InternalLoadSymbols(handle_, NumSymbols(), kSymbolNames, symbols_)) {
      undefined_symbols_ = true;
      Unload();
      return false;
    }
    return true;
  }

  void Unload() {
    if (!IsLoaded()) {
      return;
    }
    InternalUnloadDll(handle_);
    handle_ = kInvalidDllHandle;
    memset(symbols_, 0, sizeof(symbols_));
  }

  
  
  void *GetSymbol(int index) const {
    ASSERT(IsLoaded());
    ASSERT(index < NumSymbols());
    return symbols_[index];
  }

 private:
  DllHandle handle_;
  bool undefined_symbols_;
  void *symbols_[SYMBOL_TABLE_SIZE];

  DISALLOW_COPY_AND_ASSIGN(LateBindingSymbolTable);
};

#define LATE_BINDING_SYMBOL_TABLE_DECLARE_BEGIN(ClassName) \
enum {

#define LATE_BINDING_SYMBOL_TABLE_DECLARE_ENTRY(ClassName, sym) \
  ClassName##_SYMBOL_TABLE_INDEX_##sym,

#define LATE_BINDING_SYMBOL_TABLE_DECLARE_END(ClassName) \
  ClassName##_SYMBOL_TABLE_SIZE \
}; \
\
extern const char ClassName##_kDllName[]; \
extern const char *const \
    ClassName##_kSymbolNames[ClassName##_SYMBOL_TABLE_SIZE]; \
\
typedef ::talk_base::LateBindingSymbolTable<ClassName##_SYMBOL_TABLE_SIZE, \
                                            ClassName##_kDllName, \
                                            ClassName##_kSymbolNames> \
    ClassName;

#define LATE_BINDING_SYMBOL_TABLE_DEFINE_BEGIN(ClassName, dllName) \
const char ClassName##_kDllName[] = dllName; \
const char *const ClassName##_kSymbolNames[ClassName##_SYMBOL_TABLE_SIZE] = {

#define LATE_BINDING_SYMBOL_TABLE_DEFINE_ENTRY(ClassName, sym) \
  #sym,

#define LATE_BINDING_SYMBOL_TABLE_DEFINE_END(ClassName) \
};

#define LATESYM_INDEXOF(ClassName, sym) \
  (ClassName##_SYMBOL_TABLE_INDEX_##sym)

#define LATESYM_GET(ClassName, inst, sym) \
  (*reinterpret_cast<typeof(&sym)>( \
      (inst)->GetSymbol(LATESYM_INDEXOF(ClassName, sym))))

}  

#endif  

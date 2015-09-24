/* Copyright (c) 2007, Google Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Joi Sigurdsson
 * Author: Scott Francis
 *
 * Definition of PreamblePatcher
 */

#ifndef GOOGLE_PERFTOOLS_PREAMBLE_PATCHER_H_
#define GOOGLE_PERFTOOLS_PREAMBLE_PATCHER_H_

#include "config.h"
#include <windows.h>

#include "base/logging.h"
#define SIDESTEP_ASSERT(cond)  RAW_DCHECK(cond, #cond)
#define SIDESTEP_LOG(msg)      RAW_VLOG(1, msg)

#ifdef _M_X64
#define MAX_PREAMBLE_STUB_SIZE    (64)
#else
#define MAX_PREAMBLE_STUB_SIZE    (32)
#endif

#ifdef _M_X64
static const bool kIs64BitBinary = true;
#else
static const bool kIs64BitBinary = false;
#endif

namespace sidestep {

enum SideStepError {
  SIDESTEP_SUCCESS = 0,
  SIDESTEP_INVALID_PARAMETER,
  SIDESTEP_INSUFFICIENT_BUFFER,
  SIDESTEP_JUMP_INSTRUCTION,
  SIDESTEP_FUNCTION_TOO_SMALL,
  SIDESTEP_UNSUPPORTED_INSTRUCTION,
  SIDESTEP_NO_SUCH_MODULE,
  SIDESTEP_NO_SUCH_FUNCTION,
  SIDESTEP_ACCESS_DENIED,
  SIDESTEP_UNEXPECTED,
};

#define SIDESTEP_TO_HRESULT(error)                      \
  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_NULL, error)

class DeleteUnsignedCharArray;

class PERFTOOLS_DLL_DECL PreamblePatcher {
 public:

  
  
  
  
  
  
  
  
  
  
  
  
  template <class T>
  static SideStepError Patch(T target_function,
                             T replacement_function,
                             T* original_function_stub) {
    
    
    
    return RawPatch((void*)(target_function),
                    (void*)(replacement_function),
                    (void**)(original_function_stub));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  template <class T>
  static SideStepError Patch(LPCTSTR module_name,
                             LPCSTR function_name,
                             T replacement_function,
                             T* original_function_stub) {
    SIDESTEP_ASSERT(module_name && function_name);
    if (!module_name || !function_name) {
      SIDESTEP_ASSERT(false &&
                      "You must specify a module name and function name.");
      return SIDESTEP_INVALID_PARAMETER;
    }
    HMODULE module = ::GetModuleHandle(module_name);
    SIDESTEP_ASSERT(module != NULL);
    if (!module) {
      SIDESTEP_ASSERT(false && "Invalid module name.");
      return SIDESTEP_NO_SUCH_MODULE;
    }
    FARPROC existing_function = ::GetProcAddress(module, function_name);
    if (!existing_function) {
      SIDESTEP_ASSERT(
          false && "Did not find any function with that name in the module.");
      return SIDESTEP_NO_SUCH_FUNCTION;
    }
    
    
    
    return RawPatch((void*)existing_function, (void*)replacement_function,
                    (void**)(original_function_stub));
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError RawPatch(void* target_function,
                                void* replacement_function,
                                void** original_function_stub);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError Unpatch(void* target_function,
                               void* replacement_function,
                               void* original_function_stub);

  
  
  
  
  
  
  
  
  
  
  
  template <class T>
  static T ResolveTarget(T target_function) {
    return (T)ResolveTargetImpl((unsigned char*)target_function, NULL);
  }

  
  
  
  
  
  
  
  
  
  
  static unsigned char* AllocPreambleBlockNear(void* target);

  
  
  
  static void FreePreambleBlock(unsigned char* block);

 private:
  friend class DeleteUnsignedCharArray;

   
  struct PreamblePage {
    unsigned int magic_;
    PreamblePage* next_;
    
    
    void* free_;
  };

  
  
  
  
  
  
  static PreamblePage* preamble_pages_;
  
  
  static long granularity_;

  
  static long pagesize_;

  
  static bool initialized_;

  
  static void Initialize();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError RawPatchWithStubAndProtections(
      void* target_function,
      void* replacement_function,
      unsigned char* preamble_stub,
      unsigned long stub_size,
      unsigned long* bytes_needed);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError RawPatchWithStub(void* target_function,
                                        void* replacement_function,
                                        unsigned char* preamble_stub,
                                        unsigned long stub_size,
                                        unsigned long* bytes_needed);


  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static void* ResolveTargetImpl(unsigned char* target_function,
                                 unsigned char* stop_before,
                                 bool stop_before_trampoline = false);

  
  
  
  
  
  
  static void* AllocPageNear(void* target);

  
  
  
  
  
  
  
  
  static bool IsShortConditionalJump(unsigned char* target,
                                     unsigned int instruction_size);

  
  
  
  
  
  
  
  
  static bool IsNearConditionalJump(unsigned char* target,
                                    unsigned int instruction_size);

  
  
  
  
  
  
  
  
  static bool IsNearRelativeJump(unsigned char* target,
                                 unsigned int instruction_size);

  
  
  
  
  
  
  
  
  static bool IsNearAbsoluteCall(unsigned char* target,
                                 unsigned int instruction_size);

  
  
  
  
  
  
  
  
  static bool IsNearRelativeCall(unsigned char* target,
                                 unsigned int instruction_size);

  
  
  
  
  
  
  
  
  static bool IsMovWithDisplacement(unsigned char* target,
                                    unsigned int instruction_size);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError PatchShortConditionalJump(unsigned char* source,
                                                 unsigned int instruction_size,
                                                 unsigned char* target,
                                                 unsigned int* target_bytes,
                                                 unsigned int target_size);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError PatchNearJumpOrCall(unsigned char* source,
                                           unsigned int instruction_size,
                                           unsigned char* target,
                                           unsigned int* target_bytes,
                                           unsigned int target_size);
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SideStepError PatchMovWithDisplacement(unsigned char* source,
                                                unsigned int instruction_size,
                                                unsigned char* target,
                                                unsigned int* target_bytes,
                                                unsigned int target_size);
};

};  

#endif  

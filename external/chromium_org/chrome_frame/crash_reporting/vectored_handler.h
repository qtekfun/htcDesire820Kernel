// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CRASH_REPORTING_VECTORED_HANDLER_H_
#define CHROME_FRAME_CRASH_REPORTING_VECTORED_HANDLER_H_


#if !defined(_M_IX86)
#error only x86 is supported for now.
#endif

template <typename E>
class VectoredHandlerT {
 public:
  VectoredHandlerT(E* api);
  ~VectoredHandlerT();

  
  FORCEINLINE LONG Handler(EXCEPTION_POINTERS* exceptionInfo);
  long get_exceptions_seen() const {
    return exceptions_seen_;
  }

 private:
  bool ModuleHasInstalledSEHFilter();
  E* api_;
  long exceptions_seen_;
};

struct ModuleOfInterest {
  
  inline bool IsOurModule(const void* address) {
    return (start_ <= address && address < end_);
  }

  
  inline void SetModule(const void* module_start, const void* module_end) {
    start_ = module_start;
    end_ = module_end;
  }

  inline void SetCurrentModule() {
    
    const void* start = &__ImageBase;
    const char* s = reinterpret_cast<const char*>(start);
    const IMAGE_NT_HEADERS32* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>
        (s + __ImageBase.e_lfanew);
    const void* end = s + nt->OptionalHeader.SizeOfImage;
    SetModule(start, end);
  }

  const void* start_;
  const void* end_;
};

struct ModuleOfInterestWithExcludedRegion : public ModuleOfInterest {
  inline bool IsOurModule(const void* address) {
    return (start_ <= address && address < end_) &&
           (address < special_region_start_ || special_region_end_ <= address);
  }

  inline void SetExcludedRegion(const void* start, const void* end) {
    special_region_start_ = start;
    special_region_end_ = end;
  }

  const void* special_region_start_;
  const void* special_region_end_;
};


#endif  

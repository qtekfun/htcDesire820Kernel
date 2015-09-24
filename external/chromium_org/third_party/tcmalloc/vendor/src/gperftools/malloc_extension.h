// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_MALLOC_EXTENSION_H_
#define BASE_MALLOC_EXTENSION_H_

#include <stddef.h>
#ifndef _MSC_VER
#include <stdint.h>
#endif
#include <string>
#include <vector>

#ifndef PERFTOOLS_DLL_DECL
# ifdef _WIN32
#   define PERFTOOLS_DLL_DECL  __declspec(dllimport)
# else
#   define PERFTOOLS_DLL_DECL
# endif
#endif

static const int kMallocHistogramSize = 64;

typedef std::string MallocExtensionWriter;

namespace base {
struct MallocRange;
}

class SysAllocator {
 public:
  SysAllocator() {
  }
  virtual ~SysAllocator();

  
  
  
  virtual void* Alloc(size_t size, size_t *actual_size, size_t alignment) = 0;
};

class PERFTOOLS_DLL_DECL MallocExtension {
 public:
  virtual ~MallocExtension();

  
  
  
  
  
  
  static void Initialize();

  
  virtual bool VerifyAllMemory();
  virtual bool VerifyNewMemory(const void* p);
  virtual bool VerifyArrayNewMemory(const void* p);
  virtual bool VerifyMallocMemory(const void* p);
  virtual bool MallocMemoryStats(int* blocks, size_t* total,
                                 int histogram[kMallocHistogramSize]);

  
  
  
  
  virtual void GetStats(char* buffer, int buffer_length);

  
  
  
  
  
  
  
  
  
  
  virtual void GetHeapSample(MallocExtensionWriter* writer);

  
  
  
  
  
  
  
  
  virtual void GetHeapGrowthStacks(MallocExtensionWriter* writer);

  
  
  
  
  
  typedef void (RangeFunction)(void*, const base::MallocRange*);
  virtual void Ranges(void* arg, RangeFunction func);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
  
  
  
  virtual bool GetNumericProperty(const char* property, size_t* value);

  
  
  
  
  
  virtual bool SetNumericProperty(const char* property, size_t value);

  
  
  
  
  
  
  
  
  
  
  virtual void MarkThreadIdle();

  
  
  
  
  
  virtual void MarkThreadBusy();

  
  
  
  virtual SysAllocator* GetSystemAllocator();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void SetSystemAllocator(SysAllocator *a);

  
  
  
  
  virtual void ReleaseToSystem(size_t num_bytes);

  
  virtual void ReleaseFreeMemory();

  
  
  
  
  
  virtual void SetMemoryReleaseRate(double rate);

  
  virtual double GetMemoryReleaseRate();

  
  
  
  
  
  
  virtual size_t GetEstimatedAllocatedSize(size_t size);

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual size_t GetAllocatedSize(const void* p);

  
  
  
  
  
  
  
  
  
  enum Ownership {
    
    
    kUnknownOwnership = 0,
    kOwned,
    kNotOwned
  };
  virtual Ownership GetOwnership(const void* p);

  
  static MallocExtension* instance();

  
  
  static void Register(MallocExtension* implementation);

  
  
  struct FreeListInfo {
    size_t min_object_size;
    size_t max_object_size;
    size_t total_bytes_free;
    const char* type;
  };
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void GetFreeListSizes(std::vector<FreeListInfo>* v);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void** ReadStackTraces(int* sample_period);

  
  
  virtual void** ReadHeapGrowthStackTraces();
};

namespace base {

struct MallocRange {
  enum Type {
    INUSE,                
    FREE,                 
    UNMAPPED,             
    UNKNOWN,
    
  };

  uintptr_t address;    
  size_t length;        
  Type type;            
  double fraction;      

  
  
  
  
};

} 

#endif  

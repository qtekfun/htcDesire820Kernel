// Copyright 2000 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef RE2_UTIL_ARENA_H_
#define RE2_UTIL_ARENA_H_

namespace re2 {

class UnsafeArena {
 public:
  UnsafeArena(const size_t block_size);
  virtual ~UnsafeArena();

  void Reset();

  
  
  
  
  
  
#ifdef __i386__
  static const int kDefaultAlignment = 4;
#else
  static const int kDefaultAlignment = 8;
#endif

 private:
  void* GetMemoryFallback(const size_t size, const int align);

 public:
  void* GetMemory(const size_t size, const int align) {
    if ( size > 0 && size < remaining_ && align == 1 ) {       
      last_alloc_ = freestart_;
      freestart_ += size;
      remaining_ -= size;
      return reinterpret_cast<void*>(last_alloc_);
    }
    return GetMemoryFallback(size, align);
  }

 private:
  struct AllocatedBlock {
    char *mem;
    size_t size;
  };

  
  
  AllocatedBlock *AllocNewBlock(const size_t block_size);

  const AllocatedBlock *IndexToBlock(int index) const;

  const size_t block_size_;
  char* freestart_;         
  char* freestart_when_empty_;  
  char* last_alloc_;         
  size_t remaining_;
  
  int blocks_alloced_;       
  AllocatedBlock first_blocks_[16];   
  
  vector<AllocatedBlock>* overflow_blocks_;

  void FreeBlocks();         

  DISALLOW_EVIL_CONSTRUCTORS(UnsafeArena);
};

enum AllocateInArenaType { AllocateInArena };

}  

inline void* operator new(size_t size,
                          re2::AllocateInArenaType ,
                          re2::UnsafeArena *arena) {
  return reinterpret_cast<char*>(arena->GetMemory(size, 1));
}

#endif  


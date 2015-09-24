// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_MEMORY_ALLOCATOR_H_
#define COURGETTE_MEMORY_ALLOCATOR_H_

#include <memory>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/platform_file.h"

#ifndef NDEBUG

template<class T>
class CheckReturnValue {
 public:
  
  CheckReturnValue(T value) : value_(value), checked_(false) {  
  }
  CheckReturnValue(const CheckReturnValue& other)
      : value_(other.value_), checked_(other.checked_) {
    other.checked_ = true;
  }

  CheckReturnValue& operator=(const CheckReturnValue& other) {
    if (this != &other) {
      DCHECK(checked_);
      value_ = other.value_;
      checked_ = other.checked_;
      other.checked_ = true;
    }
  }

  ~CheckReturnValue() {
    DCHECK(checked_);
  }

  operator const T&() const {
    checked_ = true;
    return value_;
  }

 private:
  T value_;
  mutable bool checked_;
};
typedef CheckReturnValue<bool> CheckBool;
#else
typedef bool CheckBool;
#endif

namespace courgette {

#if defined(OS_WIN)

class TempFile {
 public:
  TempFile();
  ~TempFile();

  bool Create();
  void Close();
  bool SetSize(size_t size);

  
  bool valid() const;

  
  
  base::PlatformFile handle() const;

 protected:
  base::PlatformFile file_;
};

class FileMapping {
 public:
  FileMapping();
  ~FileMapping();

  
  bool Create(HANDLE file, size_t size);
  void Close();

  
  bool valid() const;

  
  
  void* view() const;

 protected:
  bool InitializeView(size_t size);

  HANDLE mapping_;
  void* view_;
};

class TempMapping {
 public:
  TempMapping();
  ~TempMapping();

  
  
  bool Initialize(size_t size);

  
  void* memory() const;

  
  bool valid() const;

  
  
  
  static TempMapping* GetMappingFromPtr(void* mem);

 protected:
  TempFile file_;
  FileMapping mapping_;
};

template<class T>
class MemoryAllocator {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef const value_type* const_pointer;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  
  
  enum AllocationType {
    HEAP_ALLOCATION,
    FILE_ALLOCATION,
  };

  
  
  
  
  
  static const size_t kMaxHeapAllocationSize = 1024 * 1024 * 5;

  template<class OtherT>
  struct rebind {
    
    typedef MemoryAllocator<OtherT> other;
  };

  MemoryAllocator() _THROW0() {
  }

  
  
  MemoryAllocator(const MemoryAllocator<T>& other) _THROW0() {  
  }

  template<class OtherT>
  MemoryAllocator(const MemoryAllocator<OtherT>& other) _THROW0() {  
  }

  ~MemoryAllocator() {
  }

  void deallocate(pointer ptr, size_type size) {
    uint8* mem = reinterpret_cast<uint8*>(ptr);
    mem -= sizeof(T);
    if (mem[0] == HEAP_ALLOCATION) {
      delete [] mem;
    } else {
      DCHECK_EQ(static_cast<uint8>(FILE_ALLOCATION), mem[0]);
      TempMapping* mapping = TempMapping::GetMappingFromPtr(mem);
      delete mapping;
    }
  }

  pointer allocate(size_type count) {
    
    
    
    
    count++;

    if (count > max_size())
      return NULL;

    size_type bytes = count * sizeof(T);
    uint8* mem = NULL;

    
    if (count < kMaxHeapAllocationSize)
      mem = new(std::nothrow) uint8[bytes];
    if (mem != NULL) {
      mem[0] = static_cast<uint8>(HEAP_ALLOCATION);
    } else {
      
      
      TempMapping* mapping = new(std::nothrow) TempMapping();
      if (mapping && mapping->Initialize(bytes)) {
        mem = reinterpret_cast<uint8*>(mapping->memory());
        mem[0] = static_cast<uint8>(FILE_ALLOCATION);
      }
    }
    return mem ? reinterpret_cast<pointer>(mem + sizeof(T)) : NULL;
  }

  pointer allocate(size_type count, const void* hint) {
    return allocate(count);
  }

  void construct(pointer ptr, const T& value) {
    ::new(ptr) T(value);
  }

  void destroy(pointer ptr) {
    ptr->~T();
  }

  size_type max_size() const _THROW0() {
    size_type count = static_cast<size_type>(-1) / sizeof(T);
    return (0 < count ? count : 1);
  }
};

#else  

template<class T>
class MemoryAllocator {
 public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef value_type& reference;
  typedef const value_type* const_pointer;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  template<class OtherT>
  struct rebind {
    
    typedef MemoryAllocator<OtherT> other;
  };

  MemoryAllocator() {
  }

  explicit MemoryAllocator(const MemoryAllocator<T>& other) {
  }

  template<class OtherT>
  explicit MemoryAllocator(const MemoryAllocator<OtherT>& other) {
  }

  ~MemoryAllocator() {
  }

  void deallocate(pointer ptr, size_type size) {
    delete [] ptr;
  }

  pointer allocate(size_type count) {
    if (count > max_size())
      return NULL;
    return reinterpret_cast<pointer>(
        new(std::nothrow) uint8[count * sizeof(T)]);
  }

  pointer allocate(size_type count, const void* hint) {
    return allocate(count);
  }

  void construct(pointer ptr, const T& value) {
    ::new(ptr) T(value);
  }

  void destroy(pointer ptr) {
    ptr->~T();
  }

  size_type max_size() const {
    size_type count = static_cast<size_type>(-1) / sizeof(T);
    return (0 < count ? count : 1);
  }
};

#endif  

template<typename T, class Allocator = MemoryAllocator<T> >
class NoThrowBuffer {
 public:
  typedef T value_type;
  static const size_t kAllocationFailure = 0xffffffff;
  static const size_t kStartSize = sizeof(T) > 0x100 ? 1 : 0x100 / sizeof(T);

  NoThrowBuffer() : buffer_(NULL), size_(0), alloc_size_(0) {
  }

  ~NoThrowBuffer() {
    clear();
  }

  void clear() {
    if (buffer_) {
      alloc_.deallocate(buffer_, alloc_size_);
      buffer_ = NULL;
      size_ = 0;
      alloc_size_ = 0;
    }
  }

  bool empty() const {
    return size_ == 0;
  }

  CheckBool reserve(size_t size) WARN_UNUSED_RESULT {
    if (failed())
      return false;

    if (size <= alloc_size_)
      return true;

    if (size < kStartSize)
      size = kStartSize;

    T* new_buffer = alloc_.allocate(size);
    if (!new_buffer) {
      clear();
      alloc_size_ = kAllocationFailure;
    } else {
      if (buffer_) {
        memcpy(new_buffer, buffer_, size_ * sizeof(T));
        alloc_.deallocate(buffer_, alloc_size_);
      }
      buffer_ = new_buffer;
      alloc_size_ = size;
    }

    return !failed();
  }

  CheckBool append(const T* data, size_t size) WARN_UNUSED_RESULT {
    if (failed())
      return false;

    if (size > alloc_.max_size() - size_)
      return false;

    if (!size)
      return true;

    if ((alloc_size_ - size_) < size) {
      const size_t max_size = alloc_.max_size();
      size_t new_size = alloc_size_ ? alloc_size_ : kStartSize;
      while (new_size < size_ + size) {
        if (new_size < max_size - new_size) {
          new_size *= 2;
        } else {
          new_size = max_size;
        }
      }
      if (!reserve(new_size))
        return false;
    }

    memcpy(buffer_ + size_, data, size * sizeof(T));
    size_ += size;

    return true;
  }

  CheckBool resize(size_t size, const T& init_value) WARN_UNUSED_RESULT {
    if (size > size_) {
      if (!reserve(size))
        return false;
      for (size_t i = size_; i < size; ++i)
        buffer_[i] = init_value;
    } else if (size < size_) {
      
      
    }

    size_ = size;

    return true;
  }

  CheckBool push_back(const T& item) WARN_UNUSED_RESULT {
    return append(&item, 1);
  }

  const T& back() const {
    return buffer_[size_ - 1];
  }

  T& back() {
    return buffer_[size_ - 1];
  }

  const T* begin() const {
    if (!size_)
      return NULL;
    return &buffer_[0];
  }

  T* begin() {
    if (!size_)
      return NULL;
    return &buffer_[0];
  }

  const T* end() const {
    if (!size_)
      return NULL;
    return &buffer_[size_ - 1];
  }

  T* end() {
    if (!size_)
      return NULL;
    return &buffer_[size_ - 1];
  }

  const T& operator[](size_t index) const {
    DCHECK(index < size_);
    return buffer_[index];
  }

  T& operator[](size_t index) {
    DCHECK(index < size_);
    return buffer_[index];
  }

  size_t size() const {
    return size_;
  }

  T* data() const {
    return buffer_;
  }

  
  bool failed() const {
    return alloc_size_ == kAllocationFailure;
  }

 protected:
  T* buffer_;
  size_t size_;  
  size_t alloc_size_;  
  Allocator alloc_;
};

}  

#endif  

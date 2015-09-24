// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_REF_COUNTED_MEMORY_H_
#define BASE_MEMORY_REF_COUNTED_MEMORY_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"

namespace base {

class BASE_EXPORT RefCountedMemory
    : public base::RefCountedThreadSafe<RefCountedMemory> {
 public:
  
  
  virtual const unsigned char* front() const = 0;

  
  virtual size_t size() const = 0;

  
  bool Equals(const scoped_refptr<RefCountedMemory>& other) const;

 protected:
  friend class base::RefCountedThreadSafe<RefCountedMemory>;
  RefCountedMemory();
  virtual ~RefCountedMemory();
};

class BASE_EXPORT RefCountedStaticMemory : public RefCountedMemory {
 public:
  RefCountedStaticMemory()
      : data_(NULL), length_(0) {}
  RefCountedStaticMemory(const unsigned char* data, size_t length)
      : data_(length ? data : NULL), length_(length) {}

  
  virtual const unsigned char* front() const OVERRIDE;
  virtual size_t size() const OVERRIDE;

 private:
  virtual ~RefCountedStaticMemory();

  const unsigned char* data_;
  size_t length_;

  DISALLOW_COPY_AND_ASSIGN(RefCountedStaticMemory);
};

class BASE_EXPORT RefCountedBytes : public RefCountedMemory {
 public:
  RefCountedBytes();

  
  explicit RefCountedBytes(const std::vector<unsigned char>& initializer);

  
  
  
  static RefCountedBytes* TakeVector(std::vector<unsigned char>* to_destroy);

  
  virtual const unsigned char* front() const OVERRIDE;
  virtual size_t size() const OVERRIDE;

  const std::vector<unsigned char>& data() const { return data_; }
  std::vector<unsigned char>& data() { return data_; }

 private:
  virtual ~RefCountedBytes();

  std::vector<unsigned char> data_;

  DISALLOW_COPY_AND_ASSIGN(RefCountedBytes);
};

class BASE_EXPORT RefCountedString : public RefCountedMemory {
 public:
  RefCountedString();

  
  
  
  static RefCountedString* TakeString(std::string* to_destroy);

  
  virtual const unsigned char* front() const OVERRIDE;
  virtual size_t size() const OVERRIDE;

  const std::string& data() const { return data_; }
  std::string& data() { return data_; }

 private:
  virtual ~RefCountedString();

  std::string data_;

  DISALLOW_COPY_AND_ASSIGN(RefCountedString);
};

class BASE_EXPORT RefCountedMallocedMemory : public base::RefCountedMemory {
 public:
  RefCountedMallocedMemory(void* data, size_t length);

  
  virtual const unsigned char* front() const OVERRIDE;
  virtual size_t size() const OVERRIDE;

 private:
  virtual ~RefCountedMallocedMemory();

  unsigned char* data_;
  size_t length_;

  DISALLOW_COPY_AND_ASSIGN(RefCountedMallocedMemory);
};

}  

#endif  

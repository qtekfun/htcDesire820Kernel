// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_REF_COUNTED_MEMORY_H_
#define BASE_MEMORY_REF_COUNTED_MEMORY_H_
#pragma once

#include <vector>

#include "base/base_api.h"
#include "base/memory/ref_counted.h"


class BASE_API RefCountedMemory
    : public base::RefCountedThreadSafe<RefCountedMemory> {
 public:
  
  
  virtual const unsigned char* front() const = 0;

  
  virtual size_t size() const = 0;

 protected:
  friend class base::RefCountedThreadSafe<RefCountedMemory>;
  RefCountedMemory();
  virtual ~RefCountedMemory();
};

class BASE_API RefCountedStaticMemory : public RefCountedMemory {
 public:
  RefCountedStaticMemory()
      : data_(NULL), length_(0) {}
  RefCountedStaticMemory(const unsigned char* data, size_t length)
      : data_(data), length_(length) {}

  
  virtual const unsigned char* front() const;
  virtual size_t size() const;

 private:
  const unsigned char* data_;
  size_t length_;

  DISALLOW_COPY_AND_ASSIGN(RefCountedStaticMemory);
};

class BASE_API RefCountedBytes : public RefCountedMemory {
 public:
  RefCountedBytes();

  
  RefCountedBytes(const std::vector<unsigned char>& initializer);

  
  
  
  static RefCountedBytes* TakeVector(std::vector<unsigned char>* to_destroy);

  
  virtual const unsigned char* front() const;
  virtual size_t size() const;

  std::vector<unsigned char> data;

 protected:
  friend class base::RefCountedThreadSafe<RefCountedBytes>;
  virtual ~RefCountedBytes();

 private:
  DISALLOW_COPY_AND_ASSIGN(RefCountedBytes);
};

#endif  

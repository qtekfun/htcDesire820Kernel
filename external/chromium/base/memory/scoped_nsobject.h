// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_SCOPED_NSOBJECT_H_
#define BASE_MEMORY_SCOPED_NSOBJECT_H_
#pragma once

#import <Foundation/Foundation.h>
#include "base/basictypes.h"
#include "base/compiler_specific.h"

template<typename NST>
class scoped_nsobject {
 public:
  typedef NST* element_type;

  explicit scoped_nsobject(NST* object = nil)
      : object_(object) {
  }

  ~scoped_nsobject() {
    [object_ release];
  }

  void reset(NST* object = nil) {
    
    
    
    
    
    [object_ release];
    object_ = object;
  }

  bool operator==(NST* that) const { return object_ == that; }
  bool operator!=(NST* that) const { return object_ != that; }

  operator NST*() const {
    return object_;
  }

  NST* get() const {
    return object_;
  }

  void swap(scoped_nsobject& that) {
    NST* temp = that.object_;
    that.object_ = object_;
    object_ = temp;
  }

  
  
  
  NST* release() WARN_UNUSED_RESULT {
    NST* temp = object_;
    object_ = nil;
    return temp;
  }

 private:
  NST* object_;

  DISALLOW_COPY_AND_ASSIGN(scoped_nsobject);
};

template <class C>
void swap(scoped_nsobject<C>& p1, scoped_nsobject<C>& p2) {
  p1.swap(p2);
}

template <class C>
bool operator==(C* p1, const scoped_nsobject<C>& p2) {
  return p1 == p2.get();
}

template <class C>
bool operator!=(C* p1, const scoped_nsobject<C>& p2) {
  return p1 != p2.get();
}


template<>
class scoped_nsobject<id> {
 public:
  typedef id element_type;

  explicit scoped_nsobject(id object = nil)
      : object_(object) {
  }

  ~scoped_nsobject() {
    [object_ release];
  }

  void reset(id object = nil) {
    
    
    
    
    
    [object_ release];
    object_ = object;
  }

  bool operator==(id that) const { return object_ == that; }
  bool operator!=(id that) const { return object_ != that; }

  operator id() const {
    return object_;
  }

  id get() const {
    return object_;
  }

  void swap(scoped_nsobject& that) {
    id temp = that.object_;
    that.object_ = object_;
    object_ = temp;
  }

  
  
  
  id release() WARN_UNUSED_RESULT {
    id temp = object_;
    object_ = nil;
    return temp;
  }

 private:
  id object_;

  DISALLOW_COPY_AND_ASSIGN(scoped_nsobject);
};

template<>
class scoped_nsobject<NSAutoreleasePool> {
 private:
  explicit scoped_nsobject(NSAutoreleasePool* object = nil);
  DISALLOW_COPY_AND_ASSIGN(scoped_nsobject);
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MAC_SCOPED_NSOBJECT_H_
#define BASE_MAC_SCOPED_NSOBJECT_H_

#import <Foundation/Foundation.h>
#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace base {


template<typename NST>
class scoped_nsprotocol {
 public:
  explicit scoped_nsprotocol(NST object = nil) : object_(object) {}

  scoped_nsprotocol(const scoped_nsprotocol<NST>& that)
      : object_([that.object_ retain]) {
  }

  ~scoped_nsprotocol() {
    [object_ release];
  }

  scoped_nsprotocol& operator=(const scoped_nsprotocol<NST>& that) {
    reset([that.get() retain]);
    return *this;
  }

  void reset(NST object = nil) {
    
    
    
    
    [object_ release];
    object_ = object;
  }

  bool operator==(NST that) const { return object_ == that; }
  bool operator!=(NST that) const { return object_ != that; }

  operator NST() const {
    return object_;
  }

  NST get() const {
    return object_;
  }

  void swap(scoped_nsprotocol& that) {
    NST temp = that.object_;
    that.object_ = object_;
    object_ = temp;
  }

  
  
  
  NST release() WARN_UNUSED_RESULT {
    NST temp = object_;
    object_ = nil;
    return temp;
  }

  
  NST autorelease() {
    return [release() autorelease];
  }

 private:
  NST object_;
};

template <class C>
void swap(scoped_nsprotocol<C>& p1, scoped_nsprotocol<C>& p2) {
  p1.swap(p2);
}

template <class C>
bool operator==(C p1, const scoped_nsprotocol<C>& p2) {
  return p1 == p2.get();
}

template <class C>
bool operator!=(C p1, const scoped_nsprotocol<C>& p2) {
  return p1 != p2.get();
}

template<typename NST>
class scoped_nsobject : public scoped_nsprotocol<NST*> {
 public:
  explicit scoped_nsobject(NST* object = nil)
      : scoped_nsprotocol<NST*>(object) {}

  scoped_nsobject(const scoped_nsobject<NST>& that)
      : scoped_nsprotocol<NST*>(that) {
  }

  scoped_nsobject& operator=(const scoped_nsobject<NST>& that) {
    scoped_nsprotocol<NST*>::operator=(that);
    return *this;
  }
};

template<>
class scoped_nsobject<id> : public scoped_nsprotocol<id> {
 public:
  explicit scoped_nsobject(id object = nil) : scoped_nsprotocol<id>(object) {}

  scoped_nsobject(const scoped_nsobject<id>& that)
      : scoped_nsprotocol<id>(that) {
  }

  scoped_nsobject& operator=(const scoped_nsobject<id>& that) {
    scoped_nsprotocol<id>::operator=(that);
    return *this;
  }
};

template<>
class scoped_nsobject<NSAutoreleasePool> {
 private:
  explicit scoped_nsobject(NSAutoreleasePool* object = nil);
  DISALLOW_COPY_AND_ASSIGN(scoped_nsobject);
};

}  

#endif  

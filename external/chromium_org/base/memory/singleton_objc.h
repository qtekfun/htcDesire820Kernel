// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_MEMORY_SINGLETON_OBJC_H_
#define BASE_MEMORY_SINGLETON_OBJC_H_

#import <Foundation/Foundation.h>
#include "base/memory/singleton.h"

template<typename Type>
struct DefaultSingletonObjCTraits : public DefaultSingletonTraits<Type> {
  static Type* New() {
    return [[Type alloc] init];
  }

  static void Delete(Type* object) {
    [object release];
  }
};

template<typename Type,
         typename Traits = DefaultSingletonObjCTraits<Type>,
         typename DifferentiatingType = Type>
class SingletonObjC : public Singleton<Type, Traits, DifferentiatingType> {
};

#endif  

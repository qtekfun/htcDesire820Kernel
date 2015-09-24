// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_MAC_OBJC_METHOD_SWIZZLE_H_
#define CHROME_COMMON_MAC_OBJC_METHOD_SWIZZLE_H_

#import <objc/runtime.h>

namespace ObjcEvilDoers {

Method GetImplementedInstanceMethod(Class aClass, SEL aSelector);

IMP SwizzleImplementedInstanceMethods(
    Class aClass, const SEL originalSelector, const SEL alternateSelector);

}  

#endif  

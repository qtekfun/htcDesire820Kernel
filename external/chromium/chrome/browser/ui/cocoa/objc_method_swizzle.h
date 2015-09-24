// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_OBJC_METHOD_SWIZZLE_H_
#define CHROME_BROWSER_UI_COCOA_OBJC_METHOD_SWIZZLE_H_
#pragma once

#import <objc/objc-class.h>

namespace ObjcEvilDoers {

Method GetImplementedInstanceMethod(Class aClass, SEL aSelector);

IMP SwizzleImplementedInstanceMethods(
    Class aClass, const SEL originalSelector, const SEL alternateSelector);

}  

#endif  

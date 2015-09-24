// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_INJECTION_TEST_MAC_H_
#define CONTENT_PUBLIC_COMMON_INJECTION_TEST_MAC_H_

#import <Foundation/Foundation.h>

#include <string>

typedef void (*LogRendererSandboxTestMessage)(std::string message,
                                              bool is_error);

@interface RendererSandboxTestsRunner : NSObject

+ (void)setLogFunction:(LogRendererSandboxTestMessage)logFunction;

+ (BOOL)runTests;

@end

#endif  

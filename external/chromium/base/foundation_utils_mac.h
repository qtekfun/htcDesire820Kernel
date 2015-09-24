// Copyright (c) 2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FOUNDATION_UTILS_MAC_H_
#define BASE_FOUNDATION_UTILS_MAC_H_
#pragma once

#include <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>

static inline id CFTypeRefToNSObjectAutorelease(CFTypeRef cf_object) {
  
  
  
  
  
  
  
  return [NSMakeCollectable(cf_object) autorelease];
}

#endif  

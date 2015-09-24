// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_MAC_OBJC_ZOMBIE_H_
#define CHROME_COMMON_MAC_OBJC_ZOMBIE_H_

#include "base/basictypes.h"

namespace ObjcEvilDoers {

bool ZombieEnable(bool zombieAllObjects, size_t zombieCount);

void ZombieDisable();

}  

#if defined(OS_MACOSX)
#if defined(__OBJC__)

#import <Foundation/Foundation.h>

@interface NSObject (CrZombie)
- (BOOL)shouldBecomeCrZombie;
@end

#endif  
#endif  

#endif  

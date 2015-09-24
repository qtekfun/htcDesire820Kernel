// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_NSOBJECT_ZOMBIE_H_
#define CHROME_BROWSER_UI_COCOA_NSOBJECT_ZOMBIE_H_
#pragma once

#import <Foundation/Foundation.h>

namespace ObjcEvilDoers {

BOOL ZombieEnable(BOOL zombieAllObjects, size_t zombieCount);

void ZombieDisable();

}  

@interface NSObject (CrZombie)
- (BOOL)shouldBecomeCrZombie;
@end

#endif  

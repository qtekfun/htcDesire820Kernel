// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_MOCK_CHROME_APPLICATION_MAC_H_
#define BASE_TEST_MOCK_CHROME_APPLICATION_MAC_H_
#pragma once

#if defined(__OBJC__)

#import <AppKit/AppKit.h>

#include "base/message_pump_mac.h"

@interface MockCrApp : NSApplication<CrAppProtocol>
@end

#endif

namespace mock_cr_app {
void RegisterMockCrApp();
}  

#endif  

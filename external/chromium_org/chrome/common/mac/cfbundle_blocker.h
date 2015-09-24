// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_MAC_CFBUNDLE_BLOCKER_H_
#define CHROME_COMMON_MAC_CFBUNDLE_BLOCKER_H_

#if defined(__OBJC__)
@class NSString;
#else
class NSString;
#endif

namespace chrome {
namespace common {
namespace mac {

void EnableCFBundleBlocker();

bool IsBundleAllowed(NSString* bundle_id, NSString* version);

}  
}  
}  

#endif  

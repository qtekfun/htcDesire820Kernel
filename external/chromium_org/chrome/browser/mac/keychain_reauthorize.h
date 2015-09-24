// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MAC_KEYCHAIN_REAUTHORIZE_H_
#define CHROME_BROWSER_MAC_KEYCHAIN_REAUTHORIZE_H_

#ifdef __OBJC__
@class NSString;
#else
class NSString;
#endif

namespace chrome {

void KeychainReauthorize();

void KeychainReauthorizeIfNeeded(NSString* pref_key, int max_tries);

}  

#endif  

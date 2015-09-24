// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_SYNC_HELPER_H_
#define CHROME_COMMON_EXTENSIONS_SYNC_HELPER_H_

namespace extensions {

class Extension;

namespace sync_helper {

bool IsSyncable(const Extension* extension);

bool IsSyncableExtension(const Extension* extension);
bool IsSyncableApp(const Extension* extension);

}  
}  

#endif  

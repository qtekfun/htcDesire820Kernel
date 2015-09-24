// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SYNC_BUNDLE_H_
#define CHROME_BROWSER_EXTENSIONS_SYNC_BUNDLE_H_

namespace extensions {

class Extension;

class SyncBundle {
 public:
  virtual ~SyncBundle() {}

  
  virtual bool IsSyncing() const = 0;

  
  virtual void SyncChangeIfNeeded(const Extension& extension) = 0;
};

}  

#endif  

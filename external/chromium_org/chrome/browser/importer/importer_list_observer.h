// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_LIST_OBSERVER_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_LIST_OBSERVER_H_

namespace importer {

class ImporterListObserver {
 public:
  virtual void OnSourceProfilesLoaded() = 0;

 protected:
  virtual ~ImporterListObserver() {}
};

}  

#endif  

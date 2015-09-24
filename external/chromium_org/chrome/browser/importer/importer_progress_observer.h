// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_IMPORTER_PROGRESS_OBSERVER_H_
#define CHROME_BROWSER_IMPORTER_IMPORTER_PROGRESS_OBSERVER_H_

#include "chrome/common/importer/importer_data_types.h"

namespace importer {

class ImporterProgressObserver {
 public:
  
  virtual void ImportStarted() = 0;

  
  virtual void ImportItemStarted(ImportItem item) = 0;

  
  
  virtual void ImportItemEnded(ImportItem item) = 0;

  
  virtual void ImportEnded() = 0;

 protected:
  virtual ~ImporterProgressObserver() {}
};

}  

#endif  

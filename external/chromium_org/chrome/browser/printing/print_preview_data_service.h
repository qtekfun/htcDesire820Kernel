// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_PREVIEW_DATA_SERVICE_H_
#define CHROME_BROWSER_PRINTING_PRINT_PREVIEW_DATA_SERVICE_H_

#include <map>
#include <string>

#include "base/memory/ref_counted.h"

template<typename T> struct DefaultSingletonTraits;

class PrintPreviewDataStore;

namespace base {
class RefCountedBytes;
}

class PrintPreviewDataService {
 public:
  static PrintPreviewDataService* GetInstance();

  
  
  
  
  void GetDataEntry(int32 preview_ui_id, int index,
                    scoped_refptr<base::RefCountedBytes>* data);

  
  
  
  
  
  void SetDataEntry(int32 preview_ui_id, int index,
                    const base::RefCountedBytes* data);

  
  void RemoveEntry(int32 preview_ui_id);

  
  int GetAvailableDraftPageCount(int32 preview_ui_id);

 private:
  friend struct DefaultSingletonTraits<PrintPreviewDataService>;

  
  
  
  typedef std::map<int32, scoped_refptr<PrintPreviewDataStore> >
      PreviewDataStoreMap;

  PrintPreviewDataService();
  virtual ~PrintPreviewDataService();

  PreviewDataStoreMap data_store_map_;

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewDataService);
};

#endif  

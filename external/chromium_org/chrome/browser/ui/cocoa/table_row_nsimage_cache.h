// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABLE_ROW_NSIMAGE_CACHE_H_
#define CHROME_BROWSER_UI_COCOA_TABLE_ROW_NSIMAGE_CACHE_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

namespace gfx {
class ImageSkia;
}

class TableRowNSImageCache {
 public:
  
  class Table {
   public:
    
    virtual int RowCount() const = 0;

    
    virtual gfx::ImageSkia GetIcon(int row) const = 0;

   protected:
    virtual ~Table() {}
  };

  
  explicit TableRowNSImageCache(Table* model);
  ~TableRowNSImageCache();

  
  NSImage* GetImageForRow(int row);

  
  void OnModelChanged();
  void OnItemsChanged(int start, int length);
  void OnItemsAdded(int start, int length);
  void OnItemsRemoved(int start, int length);

 private:
  
  Table* model_;  

  
  
  base::scoped_nsobject<NSPointerArray> icon_images_;
};

#endif  

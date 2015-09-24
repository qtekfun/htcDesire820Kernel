// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_SHELF_ITEM_DELEGATE_MANAGER_TEST_API_H_
#define ASH_TEST_SHELF_ITEM_DELEGATE_MANAGER_TEST_API_H_

#include "base/basictypes.h"

namespace ash {

class ShelfItemDelegateManager;

namespace test {

class ShelfItemDelegateManagerTestAPI {
 public:
  explicit ShelfItemDelegateManagerTestAPI(ShelfItemDelegateManager* manager);

  
  void RemoveAllShelfItemDelegateForTest();

 private:
  ShelfItemDelegateManager* manager_;  

  DISALLOW_COPY_AND_ASSIGN(ShelfItemDelegateManagerTestAPI);
};

}  
}  

#endif  
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_TEST_APPS_GRID_CONTROLLER_TEST_HELPER_H_
#define UI_APP_LIST_COCOA_TEST_APPS_GRID_CONTROLLER_TEST_HELPER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#import "ui/base/test/ui_cocoa_test_helper.h"

@class AppsGridController;

namespace app_list {

namespace test {

class AppsGridControllerTestHelper : public ui::CocoaTest {
 public:
  static const size_t kItemsPerPage;

  AppsGridControllerTestHelper();
  virtual ~AppsGridControllerTestHelper();

  void SetUpWithGridController(AppsGridController* grid_controller);

 protected:
  
  void SimulateClick(NSView* view);

  
  void SimulateKeyAction(SEL c);

  void SimulateMouseEnterItemAt(size_t index);
  void SimulateMouseExitItemAt(size_t index);

  
  
  std::string GetViewContent() const;

  
  
  
  size_t GetPageIndexForItem(int item_id) const;

  void DelayForCollectionView();
  void SinkEvents();

  NSButton* GetItemViewAt(size_t index);
  NSCollectionView* GetPageAt(size_t index);
  NSView* GetSelectedView();

  AppsGridController* apps_grid_controller_;

 private:
  base::MessageLoopForUI message_loop_;

  DISALLOW_COPY_AND_ASSIGN(AppsGridControllerTestHelper);
};

}  
}  

#endif  

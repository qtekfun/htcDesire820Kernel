// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TASK_MANAGER_MAC_H_
#define CHROME_BROWSER_UI_COCOA_TASK_MANAGER_MAC_H_

#import <Cocoa/Cocoa.h>
#include <vector>

#include "base/mac/scoped_nsobject.h"
#include "chrome/browser/task_manager/task_manager.h"
#include "chrome/browser/ui/cocoa/table_row_nsimage_cache.h"

@class WindowSizeAutosaver;
class TaskManagerMac;

namespace gfx {
class ImageSkia;
}

@interface TaskManagerWindowController :
  NSWindowController<NSTableViewDataSource,
                     NSTableViewDelegate> {
 @private
  IBOutlet NSTableView* tableView_;
  IBOutlet NSButton* endProcessButton_;
  TaskManagerMac* taskManagerObserver_;  
  TaskManager* taskManager_;  
  TaskManagerModel* model_;  

  base::scoped_nsobject<WindowSizeAutosaver> size_saver_;

  
  
  std::vector<int> viewToModelMap_;
  std::vector<int> modelToViewMap_;

  
  base::scoped_nsobject<NSSortDescriptor> currentSortDescriptor_;
}

- (id)initWithTaskManagerObserver:(TaskManagerMac*)taskManagerObserver;

- (void)reloadData;

- (IBAction)statsLinkClicked:(id)sender;

- (IBAction)killSelectedProcesses:(id)sender;

- (void)selectDoubleClickedTab:(id)sender;
@end

@interface TaskManagerWindowController (TestingAPI)
- (NSTableView*)tableView;
@end

class TaskManagerMac : public TaskManagerModelObserver,
                       public TableRowNSImageCache::Table {
 public:
  explicit TaskManagerMac(TaskManager* task_manager);
  virtual ~TaskManagerMac();

  
  virtual void OnModelChanged() OVERRIDE;
  virtual void OnItemsChanged(int start, int length) OVERRIDE;
  virtual void OnItemsAdded(int start, int length) OVERRIDE;
  virtual void OnItemsRemoved(int start, int length) OVERRIDE;

  
  
  void WindowWasClosed();

  
  virtual int RowCount() const OVERRIDE;
  virtual gfx::ImageSkia GetIcon(int r) const OVERRIDE;

  
  
  static void Show();

  
  TaskManager* task_manager() { return task_manager_; }

  
  NSImage* GetImageForRow(int row);

  
  TaskManagerWindowController* cocoa_controller() { return window_controller_; }

 private:
  
  TaskManager* const task_manager_;  

  
  TaskManagerModel* const model_;  

  
  
  TaskManagerWindowController* window_controller_;  

  
  TableRowNSImageCache icon_cache_;

  
  
  static TaskManagerMac* instance_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerMac);
};

#endif  

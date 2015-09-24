// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TASK_MANAGER_MAC_H_
#define CHROME_BROWSER_UI_COCOA_TASK_MANAGER_MAC_H_
#pragma once

#import <Cocoa/Cocoa.h>
#include <vector>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/task_manager/task_manager.h"
#include "chrome/browser/ui/cocoa/table_row_nsimage_cache.h"

@class WindowSizeAutosaver;
class SkBitmap;
class TaskManagerMac;

@interface TaskManagerWindowController :
  NSWindowController<NSTableViewDataSource,
                     NSTableViewDelegate> {
 @private
  IBOutlet NSTableView* tableView_;
  IBOutlet NSButton* endProcessButton_;
  TaskManagerMac* taskManagerObserver_;  
  TaskManager* taskManager_;  
  TaskManagerModel* model_;  
  bool highlightBackgroundResources_;

  scoped_nsobject<WindowSizeAutosaver> size_saver_;

  
  
  std::vector<int> viewToModelMap_;
  std::vector<int> modelToViewMap_;

  
  scoped_nsobject<NSSortDescriptor> currentSortDescriptor_;

  
  scoped_nsobject<NSColor> backgroundResourceColor_;
}

- (id)initWithTaskManagerObserver:(TaskManagerMac*)taskManagerObserver
     highlightBackgroundResources:(bool)highlightBackgroundResources;

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
  TaskManagerMac(TaskManager* task_manager, bool highlight_background);
  virtual ~TaskManagerMac();

  
  virtual void OnModelChanged();
  virtual void OnItemsChanged(int start, int length);
  virtual void OnItemsAdded(int start, int length);
  virtual void OnItemsRemoved(int start, int length);

  
  
  void WindowWasClosed();

  
  virtual int RowCount() const;
  virtual SkBitmap GetIcon(int r) const;

  
  
  
  static void Show(bool highlight_background_resources);

  
  TaskManager* task_manager() { return task_manager_; }

  
  NSImage* GetImageForRow(int row);

  
  TaskManagerWindowController* cocoa_controller() { return window_controller_; }

  
  bool IsBackgroundRow(int row) const;
 private:
  
  TaskManager* const task_manager_;  

  
  TaskManagerModel* const model_;  

  
  
  TaskManagerWindowController* window_controller_;  

  
  TableRowNSImageCache icon_cache_;

  
  bool highlight_background_resources_;

  
  
  static TaskManagerMac* instance_;

  DISALLOW_COPY_AND_ASSIGN(TaskManagerMac);
};

#endif  

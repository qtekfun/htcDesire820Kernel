// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SHELF_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SHELF_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "ui/base/models/simple_menu_model.h"

class BaseDownloadItemModel;
class Browser;
class DownloadItem;

class DownloadShelf {
 public:
  virtual ~DownloadShelf() {}

  
  
  virtual void AddDownload(BaseDownloadItemModel* download_model) = 0;

  
  
  
  
  virtual bool IsShowing() const = 0;

  
  virtual bool IsClosing() const = 0;

  
  virtual void Show() = 0;

  
  virtual void Close() = 0;

  virtual Browser* browser() const = 0;
};

class DownloadShelfContextMenu : public ui::SimpleMenuModel::Delegate {
 public:
  virtual ~DownloadShelfContextMenu();

  virtual DownloadItem* download() const;

  enum ContextMenuCommands {
    SHOW_IN_FOLDER = 1,  
    OPEN_WHEN_COMPLETE,  
    ALWAYS_OPEN_TYPE,    
    CANCEL,              
    TOGGLE_PAUSE,        
    MENU_LAST
  };

 protected:
  explicit DownloadShelfContextMenu(BaseDownloadItemModel* download_model);

  ui::SimpleMenuModel* GetInProgressMenuModel();
  ui::SimpleMenuModel* GetFinishedMenuModel();
  
  DownloadItem* download_;

  
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual void ExecuteCommand(int command_id);
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual bool IsItemForCommandIdDynamic(int command_id) const;
  virtual string16 GetLabelForCommandId(int command_id) const;

  
  BaseDownloadItemModel* model_;

 private:
  
  
  scoped_ptr<ui::SimpleMenuModel> in_progress_download_menu_model_;
  scoped_ptr<ui::SimpleMenuModel> finished_download_menu_model_;

  DISALLOW_COPY_AND_ASSIGN(DownloadShelfContextMenu);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SHELF_CONTEXT_MENU_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_SHELF_CONTEXT_MENU_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "content/public/browser/download_item.h"
#include "ui/base/models/simple_menu_model.h"

namespace content {
class PageNavigator;
}

class DownloadShelfContextMenu : public ui::SimpleMenuModel::Delegate,
                                 public content::DownloadItem::Observer {
 public:
  enum ContextMenuCommands {
    SHOW_IN_FOLDER = 1,    
    OPEN_WHEN_COMPLETE,    
    ALWAYS_OPEN_TYPE,      
    PLATFORM_OPEN,         
    CANCEL,                
    TOGGLE_PAUSE,          
    DISCARD,               
    KEEP,                  
    LEARN_MORE_SCANNING,   
    LEARN_MORE_INTERRUPTED,
    REPORT,                
  };

  virtual ~DownloadShelfContextMenu();

  content::DownloadItem* download_item() const { return download_item_; }

 protected:
  DownloadShelfContextMenu(content::DownloadItem* download_item,
                           content::PageNavigator* navigator);

  
  
  ui::SimpleMenuModel* GetMenuModel();

  
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual base::string16 GetLabelForCommandId(int command_id) const OVERRIDE;

 private:
  
  
  void DetachFromDownloadItem();

  
  virtual void OnDownloadDestroyed(content::DownloadItem* download) OVERRIDE;

  ui::SimpleMenuModel* GetInProgressMenuModel();
  ui::SimpleMenuModel* GetFinishedMenuModel();
  ui::SimpleMenuModel* GetInterruptedMenuModel();
  ui::SimpleMenuModel* GetMaybeMaliciousMenuModel();
  ui::SimpleMenuModel* GetMaliciousMenuModel();

  
  
  scoped_ptr<ui::SimpleMenuModel> in_progress_download_menu_model_;
  scoped_ptr<ui::SimpleMenuModel> finished_download_menu_model_;
  scoped_ptr<ui::SimpleMenuModel> interrupted_download_menu_model_;
  scoped_ptr<ui::SimpleMenuModel> maybe_malicious_download_menu_model_;
  scoped_ptr<ui::SimpleMenuModel> malicious_download_menu_model_;

  
  content::DownloadItem* download_item_;

  
  content::PageNavigator* navigator_;

  DISALLOW_COPY_AND_ASSIGN(DownloadShelfContextMenu);
};

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_OPTIONS_MENU_MODEL_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_OPTIONS_MENU_MODEL_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/models/simple_menu_model.h"

class Balloon;

class CornerSelectionMenuModel : public ui::SimpleMenuModel,
                                 public ui::SimpleMenuModel::Delegate {
 public:
  explicit CornerSelectionMenuModel(Balloon* balloon);
  virtual ~CornerSelectionMenuModel();

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 private:
  
  Balloon* balloon_;

  DISALLOW_COPY_AND_ASSIGN(CornerSelectionMenuModel);
};

class NotificationOptionsMenuModel : public ui::SimpleMenuModel,
                                     public ui::SimpleMenuModel::Delegate {
 public:
  explicit NotificationOptionsMenuModel(Balloon* balloon);
  virtual ~NotificationOptionsMenuModel();

  
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual base::string16 GetLabelForCommandId(int command_id) const OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

 private:
  Balloon* balloon_; 

  scoped_ptr<CornerSelectionMenuModel> corner_menu_model_;

  DISALLOW_COPY_AND_ASSIGN(NotificationOptionsMenuModel);
};

#endif  

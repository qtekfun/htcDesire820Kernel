// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_OPTIONS_MENU_MODEL_H_
#define CHROME_BROWSER_NOTIFICATIONS_NOTIFICATION_OPTIONS_MENU_MODEL_H_
#pragma once

#include "chrome/browser/notifications/balloon.h"
#include "ui/base/models/simple_menu_model.h"

class CornerSelectionMenuModel : public ui::SimpleMenuModel,
                                 public ui::SimpleMenuModel::Delegate {
 public:
  explicit CornerSelectionMenuModel(Balloon* balloon);
  virtual ~CornerSelectionMenuModel();

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

 private:
  
  Balloon* balloon_;

  DISALLOW_COPY_AND_ASSIGN(CornerSelectionMenuModel);
};

class NotificationOptionsMenuModel : public ui::SimpleMenuModel,
                                     public ui::SimpleMenuModel::Delegate {
 public:
  explicit NotificationOptionsMenuModel(Balloon* balloon);
  virtual ~NotificationOptionsMenuModel();

  
  virtual bool IsItemForCommandIdDynamic(int command_id) const;
  virtual string16 GetLabelForCommandId(int command_id) const;

  
  virtual bool IsCommandIdChecked(int command_id) const;
  virtual bool IsCommandIdEnabled(int command_id) const;
  virtual bool GetAcceleratorForCommandId(int command_id,
                                          ui::Accelerator* accelerator);
  virtual void ExecuteCommand(int command_id);

 private:
  Balloon* balloon_; 

  scoped_ptr<CornerSelectionMenuModel> corner_menu_model_;

  DISALLOW_COPY_AND_ASSIGN(NotificationOptionsMenuModel);
};

#endif  

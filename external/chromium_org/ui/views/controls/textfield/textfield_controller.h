// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_CONTROLLER_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_CONTROLLER_H_

#include <set>

#include "base/strings/string16.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/views/views_export.h"

namespace ui {
class KeyEvent;
class MouseEvent;
class SimpleMenuModel;
}  

namespace views {

class Textfield;

class VIEWS_EXPORT TextfieldController {
 public:
  
  
  
  virtual void ContentsChanged(Textfield* sender,
                               const string16& new_contents) {}

  
  
  
  virtual bool HandleKeyEvent(Textfield* sender,
                              const ui::KeyEvent& key_event);

  
  
  
  virtual bool HandleMouseEvent(Textfield* sender,
                                const ui::MouseEvent& mouse_event);

  
  
  virtual void OnBeforeUserAction(Textfield* sender) {}

  
  
  virtual void OnAfterUserAction(Textfield* sender) {}

  
  virtual void OnAfterCutOrCopy() {}

  
  virtual void OnAfterPaste() {}

  // Called after the textfield has written drag data to give the controller a
  
  virtual void OnWriteDragData(ui::OSExchangeData* data) {}

  
  
  virtual void OnGetDragOperationsForTextfield(int* drag_operations) {}

  
  virtual void AppendDropFormats(
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) {}

  
  
  
  
  virtual int OnDrop(const ui::OSExchangeData& data);

  
  virtual void UpdateContextMenu(ui::SimpleMenuModel* menu_contents) {}

  
  virtual bool IsCommandIdEnabled(int command_id) const;

  
  
  virtual bool IsItemForCommandIdDynamic(int command_id) const;

  
  virtual string16 GetLabelForCommandId(int command_id) const;

  
  
  
  
  
  virtual bool HandlesCommand(int command_id) const;

  
  virtual void ExecuteCommand(int command_id, int event_flag) {}

 protected:
  virtual ~TextfieldController() {}
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_MODELS_H_
#define CHROME_BROWSER_UI_AUTOFILL_AUTOFILL_DIALOG_MODELS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/base/models/combobox_model.h"
#include "ui/base/models/simple_menu_model.h"

namespace autofill {

class SuggestionsMenuModel;

class SuggestionsMenuModelDelegate {
 public:
  virtual ~SuggestionsMenuModelDelegate();

  
  virtual void SuggestionsMenuWillShow() = 0;

  
  virtual void SuggestionItemSelected(SuggestionsMenuModel* model,
                                      size_t index) = 0;
};

class SuggestionsMenuModel : public ui::SimpleMenuModel,
                             public ui::SimpleMenuModel::Delegate {
 public:
  explicit SuggestionsMenuModel(SuggestionsMenuModelDelegate* delegate);
  virtual ~SuggestionsMenuModel();

  
  void AddKeyedItem(const std::string& key,
                    const base::string16& display_label);

  
  
  void AddKeyedItemWithIcon(const std::string& key,
                            const base::string16& display_label,
                            const gfx::Image& icon);

  
  
  void AddKeyedItemWithMinorText(const std::string& key,
                                const base::string16& display_label,
                                const base::string16& display_minor_text);

  
  
  void AddKeyedItemWithMinorTextAndIcon(
      const std::string& key,
      const base::string16& display_label,
      const base::string16& display_minor_text,
      const gfx::Image& icon);

  
  void Reset();

  
  std::string GetItemKeyAt(int index) const;

  
  
  std::string GetItemKeyForCheckedItem() const;

  
  void SetCheckedItem(const std::string& item_key);
  void SetCheckedIndex(size_t index);

  int checked_item() const { return checked_item_; }

  
  void SetEnabled(const std::string& item_key, bool enabled);

  
  virtual void MenuWillShow() OVERRIDE;

  
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;
  virtual void MenuWillShow(ui::SimpleMenuModel* source) OVERRIDE;

 private:
  
  struct Item {
    std::string key;  
    bool enabled;  
  };
  
  
  std::vector<Item> items_;

  
  size_t GetItemIndex(const std::string& item_key);

  SuggestionsMenuModelDelegate* delegate_;

  
  
  int checked_item_;

  DISALLOW_COPY_AND_ASSIGN(SuggestionsMenuModel);
};

class MonthComboboxModel : public ui::ComboboxModel {
 public:
  MonthComboboxModel();
  virtual ~MonthComboboxModel();

  static base::string16 FormatMonth(int index);

  
  virtual int GetItemCount() const OVERRIDE;
  virtual base::string16 GetItemAt(int index) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(MonthComboboxModel);
};

class YearComboboxModel : public ui::ComboboxModel {
 public:
  YearComboboxModel();
  virtual ~YearComboboxModel();

  
  virtual int GetItemCount() const OVERRIDE;
  virtual base::string16 GetItemAt(int index) OVERRIDE;

 private:
  
  int this_year_;

  DISALLOW_COPY_AND_ASSIGN(YearComboboxModel);
};

}  

#endif  

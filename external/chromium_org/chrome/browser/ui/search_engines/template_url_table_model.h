// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_ENGINES_TEMPLATE_URL_TABLE_MODEL_H_
#define CHROME_BROWSER_UI_SEARCH_ENGINES_TEMPLATE_URL_TABLE_MODEL_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/search_engines/template_url_service_observer.h"
#include "ui/base/models/table_model.h"

class ModelEntry;
class TemplateURL;
class TemplateURLService;

namespace gfx {
class ImageSkia;
}


class TemplateURLTableModel : public ui::TableModel,
                                     TemplateURLServiceObserver {
 public:
  explicit TemplateURLTableModel(TemplateURLService* template_url_service);

  virtual ~TemplateURLTableModel();

  
  
  
  void Reload();

  
  virtual int RowCount() OVERRIDE;
  virtual base::string16 GetText(int row, int column) OVERRIDE;
  virtual gfx::ImageSkia GetIcon(int row) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;
  virtual bool HasGroups() OVERRIDE;
  virtual Groups GetGroups() OVERRIDE;
  virtual int GetGroupID(int row) OVERRIDE;

  
  void Remove(int index);

  
  void Add(int index,
           const base::string16& short_name,
           const base::string16& keyword,
           const std::string& url);

  
  void ModifyTemplateURL(int index,
                         const base::string16& title,
                         const base::string16& keyword,
                         const std::string& url);

  
  void ReloadIcon(int index);

  
  TemplateURL* GetTemplateURL(int index);

  
  
  int IndexOfTemplateURL(const TemplateURL* template_url);

  
  
  
  int MoveToMainGroup(int index);

  
  
  int MakeDefaultTemplateURL(int index);

  
  void NotifyChanged(int index);

  TemplateURLService* template_url_service() const {
    return template_url_service_;
  }

  
  int last_search_engine_index() const { return last_search_engine_index_; }

  
  
  int last_other_engine_index() const { return last_other_engine_index_; }

 private:
  friend class ModelEntry;

  
  void FaviconAvailable(ModelEntry* entry);

  
  virtual void OnTemplateURLServiceChanged() OVERRIDE;

  
  scoped_ptr<ModelEntry> RemoveEntry(int index);

  
  void AddEntry(int index, scoped_ptr<ModelEntry> entry);

  ui::TableModelObserver* observer_;

  
  std::vector<ModelEntry*> entries_;

  
  TemplateURLService* template_url_service_;

  
  
  int last_search_engine_index_;

  
  
  int last_other_engine_index_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLTableModel);
};


#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_ENGINES_TEMPLATE_URL_TABLE_MODEL_H_
#define CHROME_BROWSER_UI_SEARCH_ENGINES_TEMPLATE_URL_TABLE_MODEL_H_
#pragma once

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/string16.h"
#include "chrome/browser/search_engines/template_url_model_observer.h"
#include "ui/base/models/table_model.h"

class ModelEntry;
class SkBitmap;
class TemplateURL;
class TemplateURLModel;


class TemplateURLTableModel : public ui::TableModel,
                                     TemplateURLModelObserver {
 public:
  explicit TemplateURLTableModel(TemplateURLModel* template_url_model);

  virtual ~TemplateURLTableModel();

  
  
  void Reload();

  
  virtual int RowCount() OVERRIDE;
  virtual string16 GetText(int row, int column) OVERRIDE;
  virtual SkBitmap GetIcon(int row) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;
  virtual bool HasGroups() OVERRIDE;
  virtual Groups GetGroups() OVERRIDE;
  virtual int GetGroupID(int row) OVERRIDE;

  
  void Remove(int index);

  
  void Add(int index, TemplateURL* template_url);

  
  void ModifyTemplateURL(int index,
                         const string16& title,
                         const string16& keyword,
                         const std::string& url);

  
  void ReloadIcon(int index);

  
  const TemplateURL& GetTemplateURL(int index);

  
  
  int IndexOfTemplateURL(const TemplateURL* template_url);

  
  
  
  int MoveToMainGroup(int index);

  
  
  int MakeDefaultTemplateURL(int index);

  
  void NotifyChanged(int index);

  TemplateURLModel* template_url_model() const { return template_url_model_; }

  
  int last_search_engine_index() const { return last_search_engine_index_; }

 private:
  friend class ModelEntry;

  
  void FaviconAvailable(ModelEntry* entry);

  
  virtual void OnTemplateURLModelChanged();

  ui::TableModelObserver* observer_;

  
  std::vector<ModelEntry*> entries_;

  
  TemplateURLModel* template_url_model_;

  
  
  int last_search_engine_index_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLTableModel);
};


#endif  

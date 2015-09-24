// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_COOKIES_VIEW_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_COOKIES_VIEW_HANDLER_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/cookies_tree_model.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class CookiesViewHandler : public OptionsPageUIHandler,
                           public CookiesTreeModel::Observer {
 public:
  CookiesViewHandler();
  virtual ~CookiesViewHandler();

  
  virtual void GetLocalizedValues(DictionaryValue* localized_strings);
  virtual void RegisterMessages();

  
  virtual void TreeNodesAdded(ui::TreeModel* model,
                              ui::TreeModelNode* parent,
                              int start,
                              int count);
  virtual void TreeNodesRemoved(ui::TreeModel* model,
                                ui::TreeModelNode* parent,
                                int start,
                                int count);
  virtual void TreeNodeChanged(ui::TreeModel* model, ui::TreeModelNode* node) {}
  virtual void TreeModelBeginBatch(CookiesTreeModel* model);
  virtual void TreeModelEndBatch(CookiesTreeModel* model);

 private:
  
  void EnsureCookiesTreeModelCreated();

  
  void UpdateSearchResults(const ListValue* args);

  
  void RemoveAll(const ListValue* args);

  
  void Remove(const ListValue* args);

  
  
  void LoadChildren(const ListValue* args);

  
  
  void SendChildren(CookieTreeNode* parent);

  
  scoped_ptr<CookiesTreeModel> cookies_tree_model_;

  
  bool batch_update_;

  DISALLOW_COPY_AND_ASSIGN(CookiesViewHandler);
};

#endif  

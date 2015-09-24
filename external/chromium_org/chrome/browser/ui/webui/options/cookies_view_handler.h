// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_COOKIES_VIEW_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_COOKIES_VIEW_HANDLER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/browsing_data/cookies_tree_model.h"
#include "chrome/browser/ui/webui/options/options_ui.h"

class CookiesTreeModelUtil;

namespace options {

class CookiesViewHandler : public OptionsPageUIHandler,
                           public CookiesTreeModel::Observer {
 public:
  CookiesViewHandler();
  virtual ~CookiesViewHandler();

  
  virtual void GetLocalizedValues(
      base::DictionaryValue* localized_strings) OVERRIDE;
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void TreeNodesAdded(ui::TreeModel* model,
                              ui::TreeModelNode* parent,
                              int start,
                              int count) OVERRIDE;
  virtual void TreeNodesRemoved(ui::TreeModel* model,
                                ui::TreeModelNode* parent,
                                int start,
                                int count) OVERRIDE;
  virtual void TreeNodeChanged(ui::TreeModel* model,
                               ui::TreeModelNode* node) OVERRIDE {}
  virtual void TreeModelBeginBatch(CookiesTreeModel* model) OVERRIDE;
  virtual void TreeModelEndBatch(CookiesTreeModel* model) OVERRIDE;

 private:
  
  void EnsureCookiesTreeModelCreated();

  
  void UpdateSearchResults(const base::ListValue* args);

  
  void RemoveAll(const base::ListValue* args);

  
  void Remove(const base::ListValue* args);

  
  
  void LoadChildren(const base::ListValue* args);

  
  
  void SendChildren(const CookieTreeNode* parent);

  
  
  void ReloadCookies(const base::ListValue* args);

  
  scoped_ptr<CookiesTreeModel> cookies_tree_model_;

  
  bool batch_update_;

  scoped_ptr<CookiesTreeModelUtil> model_util_;

  DISALLOW_COPY_AND_ASSIGN(CookiesViewHandler);
};

}  

#endif  

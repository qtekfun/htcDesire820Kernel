// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_ENGINES_KEYWORD_EDITOR_CONTROLLER_H_
#define CHROME_BROWSER_UI_SEARCH_ENGINES_KEYWORD_EDITOR_CONTROLLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"

class PrefRegistrySimple;
class Profile;
class TemplateURL;
class TemplateURLService;
class TemplateURLTableModel;

class KeywordEditorController {
 public:
  explicit KeywordEditorController(Profile* profile);
  ~KeywordEditorController();

  static void RegisterPrefs(PrefRegistrySimple* registry);

  
  
  
  int AddTemplateURL(const base::string16& title,
                     const base::string16& keyword,
                     const std::string& url);

  
  
  void ModifyTemplateURL(TemplateURL* template_url,
                         const base::string16& title,
                         const base::string16& keyword,
                         const std::string& url);

  
  bool CanEdit(const TemplateURL* url) const;

  
  bool CanMakeDefault(const TemplateURL* url) const;

  
  bool CanRemove(const TemplateURL* url) const;

  
  void RemoveTemplateURL(int index);

  
  
  int MakeDefaultTemplateURL(int index);

  
  bool loaded() const;

  
  TemplateURL* GetTemplateURL(int index);

  TemplateURLTableModel* table_model() {
    return table_model_.get();
  }

  TemplateURLService* url_model() const;

 private:
  
  Profile* profile_;

  
  scoped_ptr<TemplateURLTableModel> table_model_;

  DISALLOW_COPY_AND_ASSIGN(KeywordEditorController);
};

#endif  

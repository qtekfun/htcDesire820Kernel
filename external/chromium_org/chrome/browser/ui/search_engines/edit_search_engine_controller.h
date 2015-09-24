// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_ENGINES_EDIT_SEARCH_ENGINE_CONTROLLER_H_
#define CHROME_BROWSER_UI_SEARCH_ENGINES_EDIT_SEARCH_ENGINE_CONTROLLER_H_

#include <string>

#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"

class Profile;
class TemplateURL;

class EditSearchEngineControllerDelegate {
 public:
  
  
  
  
  
  virtual void OnEditedKeyword(TemplateURL* template_url,
                               const base::string16& title,
                               const base::string16& keyword,
                               const std::string& url) = 0;

 protected:
  virtual ~EditSearchEngineControllerDelegate() {}
};

class EditSearchEngineController {
 public:
  
  EditSearchEngineController(
      TemplateURL* template_url,
      EditSearchEngineControllerDelegate* edit_keyword_delegate,
      Profile* profile);
  ~EditSearchEngineController() {}

  
  bool IsTitleValid(const base::string16& title_input) const;

  
  
  
  
  bool IsURLValid(const std::string& url_input) const;

  
  
  
  bool IsKeywordValid(const base::string16& keyword_input) const;

  
  void AcceptAddOrEdit(const base::string16& title_input,
                       const base::string16& keyword_input,
                       const std::string& url_input);

  
  
  void CleanUpCancelledAdd();

  
  const TemplateURL* template_url() const { return template_url_; }
  const Profile* profile() const { return profile_; }

 private:
  
  
  std::string GetFixedUpURL(const std::string& url_input) const;

  
  
  
  TemplateURL* template_url_;

  
  
  EditSearchEngineControllerDelegate* edit_keyword_delegate_;

  
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(EditSearchEngineController);
};

#endif  

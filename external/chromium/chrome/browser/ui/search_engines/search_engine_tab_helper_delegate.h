// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_ENGINES_SEARCH_ENGINE_TAB_HELPER_DELEGATE_H_
#define CHROME_BROWSER_UI_SEARCH_ENGINES_SEARCH_ENGINE_TAB_HELPER_DELEGATE_H_
#pragma once

class Profile;
class TabContents;
class TemplateURL;
class TemplateURLModel;

class SearchEngineTabHelperDelegate {
 public:
  
  
  virtual void ConfirmSetDefaultSearchProvider(
      TabContents* tab_contents,
      TemplateURL* template_url,
      TemplateURLModel* template_url_model) = 0;

  
  
  virtual void ConfirmAddSearchProvider(const TemplateURL* template_url,
                                        Profile* profile) = 0;

 protected:
  virtual ~SearchEngineTabHelperDelegate();
};

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_SEARCH_ENGINES_SEARCH_ENGINE_TAB_HELPER_DELEGATE_H_
#define CHROME_BROWSER_UI_SEARCH_ENGINES_SEARCH_ENGINE_TAB_HELPER_DELEGATE_H_

class Profile;
class TemplateURL;

namespace content {
class WebContents;
}

class SearchEngineTabHelperDelegate {
 public:
  
  
  virtual void ConfirmAddSearchProvider(TemplateURL* template_url,
                                        Profile* profile) = 0;

 protected:
  virtual ~SearchEngineTabHelperDelegate();
};

#endif  

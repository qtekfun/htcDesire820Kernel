// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_SERVICE_OBSERVER_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_SERVICE_OBSERVER_H_

class TemplateURLServiceObserver {
 public:
  
  virtual void OnTemplateURLServiceChanged() = 0;

 protected:
  virtual ~TemplateURLServiceObserver() {}
};

#endif  

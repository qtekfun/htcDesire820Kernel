// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_MODEL_TEST_UTIL_H_
#define CHROME_BROWSER_SEARCH_ENGINES_TEMPLATE_URL_MODEL_TEST_UTIL_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/string16.h"
#include "chrome/browser/search_engines/template_url_model_observer.h"
#include "content/browser/browser_thread.h"

class TemplateURLModel;
class TemplateURLModelTestingProfile;
class TestingTemplateURLModel;
class TestingProfile;
class WebDataService;

class TemplateURLModelTestUtil : public TemplateURLModelObserver {
 public:
  TemplateURLModelTestUtil();

  virtual ~TemplateURLModelTestUtil();

  
  
  void SetUp();

  
  
  void TearDown();

  
  virtual void OnTemplateURLModelChanged();

  
  int GetObserverCount();

  
  void ResetObserverCount();

  
  
  static void BlockTillServiceProcessesRequests();

  
  
  static void BlockTillIOThreadProcessesRequests();

  
  void VerifyLoad();

  
  
  
  void ChangeModelToLoadState();

  
  void ClearModel();

  
  void ResetModel(bool verify_load);

  
  
  string16 GetAndClearSearchTerm();

  
  void SetGoogleBaseURL(const std::string& base_url) const;

  
  WebDataService* GetWebDataService();

  
  TemplateURLModel* model() const;

  
  TestingProfile* profile() const;

  
  void StartIOThread();

 private:
  MessageLoopForUI message_loop_;
  
  
  BrowserThread ui_thread_;
  scoped_ptr<TemplateURLModelTestingProfile> profile_;
  scoped_ptr<TestingTemplateURLModel> model_;
  int changed_count_;

  DISALLOW_COPY_AND_ASSIGN(TemplateURLModelTestUtil);
};

#endif  

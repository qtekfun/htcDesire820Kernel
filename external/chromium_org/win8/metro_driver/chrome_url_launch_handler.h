// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_METRO_DRIVER_CHROME_URL_LAUNCH_HANDLER_H_
#define CHROME_BROWSER_UI_METRO_DRIVER_CHROME_URL_LAUNCH_HANDLER_H_

#include <string>
#include <windows.applicationmodel.core.h>
#include <Windows.applicationModel.search.h>
#include <windows.ui.core.h>

#include "winrt_utils.h"

class ChromeUrlLaunchHandler {
 public:
  ChromeUrlLaunchHandler();
  ~ChromeUrlLaunchHandler();

  HRESULT Initialize();

  
  
  
  
  void PerformPendingNavigation();

  void Activate(winapp::Activation::IActivatedEventArgs* args);

 private:
  
  template<class T> void HandleSearchRequest(T* args);

  HRESULT OnQuerySubmitted(
      winapp::Search::ISearchPane* search_pane,
      winapp::Search::ISearchPaneQuerySubmittedEventArgs* args);

  string16 GetUrlFromLaunchArgs(const string16& launch_args);

  
  
  void HandleProtocolLaunch(
      winapp::Activation::IProtocolActivatedEventArgs* args);

  
  void HandleLaunch(winapp::Activation::ILaunchActivatedEventArgs* args);

  
  void InitiateNavigationOrSearchRequest(const wchar_t* url,
                                         const wchar_t* search_string);

  Microsoft::WRL::ComPtr<winapp::Search::ISearchPane> search_pane_;
  EventRegistrationToken query_submitted_token_;
};

#endif  

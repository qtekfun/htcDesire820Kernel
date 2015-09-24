// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTERNAL_PROTOCOL_HANDLER_H_
#define CHROME_BROWSER_EXTERNAL_PROTOCOL_HANDLER_H_
#pragma once

#include <string>

class DictionaryValue;
class GURL;
class PrefService;

class ExternalProtocolHandler {
 public:
  enum BlockState {
    DONT_BLOCK,
    BLOCK,
    UNKNOWN,
  };

  
  static BlockState GetBlockState(const std::string& scheme);

  
  static void SetBlockState(const std::string& scheme, BlockState state);

  
  
  
  
  
  
  
  static void LaunchUrl(const GURL& url, int render_process_host_id,
                        int tab_contents_id);

  
  
  
  
  
  
  
  
  
  
  static void RunExternalProtocolDialog(const GURL& url,
                                        int render_process_host_id,
                                        int routing_id);

  
  static void RegisterPrefs(PrefService* prefs);

  
  
  
  
  
  
  
  
  
  static void LaunchUrlWithoutSecurityCheck(const GURL& url);

  
  
  static void PrepopulateDictionary(DictionaryValue* win_pref);

  
  
  
  
  static void PermitLaunchUrl();
};

#endif  

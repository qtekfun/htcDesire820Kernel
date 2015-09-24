// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_PLUGIN_SELECTION_POLICY_H_
#define CHROME_BROWSER_CHROMEOS_PLUGIN_SELECTION_POLICY_H_

#include <map>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "webkit/plugins/npapi/webplugininfo.h"

class GURL;
class FilePath;

namespace chromeos {

#if !defined(OS_CHROMEOS)
#error This file is meant to be compiled on ChromeOS only.
#endif



class PluginSelectionPolicy
    : public base::RefCountedThreadSafe<PluginSelectionPolicy> {
 public:
  PluginSelectionPolicy();

  
  
  void StartInit();

  
  
  
  
  
  int FindFirstAllowed(const GURL& url,
                       const std::vector<webkit::npapi::WebPluginInfo>& info);

  
  
  
  
  bool IsAllowed(const GURL& url, const FilePath& path);

 private:
  
  FRIEND_TEST_ALL_PREFIXES(PluginSelectionPolicyTest, Basic);
  FRIEND_TEST_ALL_PREFIXES(PluginSelectionPolicyTest, FindFirstAllowed);
  FRIEND_TEST_ALL_PREFIXES(PluginSelectionPolicyTest, InitFromFile);
  FRIEND_TEST_ALL_PREFIXES(PluginSelectionPolicyTest, IsAllowed);
  FRIEND_TEST_ALL_PREFIXES(PluginSelectionPolicyTest, MissingFile);

  
  bool Init();

  
  bool InitFromFile(const FilePath& policy_file);

  typedef std::vector<std::pair<bool, std::string> > Policy;
  typedef std::map<std::string, Policy> PolicyMap;

  PolicyMap policies_;

  
  
  
  
  
  
  bool init_from_file_finished_;

  DISALLOW_COPY_AND_ASSIGN(PluginSelectionPolicy);
};

}  
#endif  

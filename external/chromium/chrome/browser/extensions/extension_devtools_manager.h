// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_DEVTOOLS_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_DEVTOOLS_MANAGER_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"

class ExtensionDevToolsBridge;
class MessageLoop;
class Profile;


class ExtensionDevToolsManager
    : public base::RefCountedThreadSafe<ExtensionDevToolsManager> {
 public:
  
  explicit ExtensionDevToolsManager(Profile* profile);

  void AddEventListener(const std::string& event_name,
                        int render_process_id);

  void RemoveEventListener(const std::string& event_name,
                           int render_process_id);

  void BridgeClosingForTab(int tab_id);

 private:
  friend class base::RefCountedThreadSafe<ExtensionDevToolsManager>;

  ~ExtensionDevToolsManager();

  
  std::map<int, linked_ptr<ExtensionDevToolsBridge> > tab_id_to_bridge_;

  
  
  std::map<int, std::set<int> > tab_id_to_render_process_ids_;

  Profile* profile_;
  MessageLoop* ui_loop_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionDevToolsManager);
};

#endif  


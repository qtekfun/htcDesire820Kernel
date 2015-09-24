// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_USER_SCRIPT_SLAVE_H_
#define CHROME_RENDERER_EXTENSIONS_USER_SCRIPT_SLAVE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/stl_util.h"
#include "base/strings/string_piece.h"
#include "extensions/common/user_script.h"
#include "third_party/WebKit/public/web/WebScriptSource.h"

class ExtensionSet;
class GURL;

namespace blink {
class WebFrame;
}

using blink::WebScriptSource;

namespace extensions {
class Extension;

class UserScriptSlave {
 public:
  
  
  static GURL GetDataSourceURLForFrame(const blink::WebFrame* frame);

  explicit UserScriptSlave(const ExtensionSet* extensions);
  ~UserScriptSlave();

  
  void GetActiveExtensions(std::set<std::string>* extension_ids);

  
  bool UpdateScripts(base::SharedMemoryHandle shared_memory);

  
  
  
  void InjectScripts(blink::WebFrame* frame, UserScript::RunLocation location);

  
  
  
  int GetIsolatedWorldIdForExtension(const Extension* extension,
                                     blink::WebFrame* frame);

  
  
  
  std::string GetExtensionIdForIsolatedWorld(int isolated_world_id);

  void RemoveIsolatedWorld(const std::string& extension_id);

 private:
  static void InitializeIsolatedWorld(int isolated_world_id,
                                      const Extension* extension);

  
  scoped_ptr<base::SharedMemory> shared_memory_;

  
  std::vector<UserScript*> scripts_;
  STLElementDeleter<std::vector<UserScript*> > script_deleter_;

  
  base::StringPiece api_js_;

  
  const ExtensionSet* extensions_;

  typedef std::map<std::string, int> IsolatedWorldMap;
  IsolatedWorldMap isolated_world_ids_;

  DISALLOW_COPY_AND_ASSIGN(UserScriptSlave);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_CHROME_V8_CONTEXT_SET_H_
#define CHROME_RENDERER_EXTENSIONS_CHROME_V8_CONTEXT_SET_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/bind.h"
#include "v8/include/v8.h"

class GURL;

namespace base {
class ListValue;
}

namespace content {
class RenderView;
}

namespace v8 {
class Context;
}

namespace extensions {
class ChromeV8Context;

class ChromeV8ContextSet {
 public:
  ChromeV8ContextSet();
  ~ChromeV8ContextSet();

  int size() const;

  
  void Add(ChromeV8Context* context);

  
  
  
  void Remove(ChromeV8Context* context);

  
  typedef std::set<ChromeV8Context*> ContextSet;
  ContextSet GetAll() const;

  
  
  ChromeV8Context* GetCurrent() const;

  
  
  ChromeV8Context* GetCalling() const;

  
  
  ChromeV8Context* GetByV8Context(v8::Handle<v8::Context> context) const;

  
  
  
  
  
  void ForEach(const std::string& extension_id,
               content::RenderView* render_view,
               const base::Callback<void(ChromeV8Context*)>& callback) const;

  
  
  
  
  
  ContextSet OnExtensionUnloaded(const std::string& extension_id);

 private:
  ContextSet contexts_;

  DISALLOW_COPY_AND_ASSIGN(ChromeV8ContextSet);
};

}  

#endif  

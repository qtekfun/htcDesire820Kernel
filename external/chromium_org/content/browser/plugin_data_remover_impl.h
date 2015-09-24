// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_PLUGIN_DATA_REMOVER_IMPL_H_
#define CONTENT_BROWSER_PLUGIN_DATA_REMOVER_IMPL_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "content/public/browser/plugin_data_remover.h"

namespace content {

class CONTENT_EXPORT PluginDataRemoverImpl : public PluginDataRemover {
 public:
  explicit PluginDataRemoverImpl(BrowserContext* browser_context);
  virtual ~PluginDataRemoverImpl();

  
  virtual base::WaitableEvent* StartRemoving(base::Time begin_time) OVERRIDE;

  
  
  
  void set_mime_type(const std::string& mime_type) { mime_type_ = mime_type; }

 private:
  class Context;

  std::string mime_type_;

  
  BrowserContext* browser_context_;

  
  
  scoped_refptr<Context> context_;

  DISALLOW_COPY_AND_ASSIGN(PluginDataRemoverImpl);
};

}  

#endif  

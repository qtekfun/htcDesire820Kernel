// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_TARGET_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_TARGET_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {

class DevToolsAgentHost;

class DevToolsTarget {
 public:
  virtual ~DevToolsTarget() {}

  
  virtual std::string GetId() const = 0;

  
  virtual std::string GetType() const = 0;

  
  virtual std::string GetTitle() const = 0;

  
  virtual std::string GetDescription() const = 0;

  
  virtual GURL GetUrl() const = 0;

  
  virtual GURL GetFaviconUrl() const = 0;

  
  virtual base::TimeTicks GetLastActivityTime() const = 0;

  
  virtual bool IsAttached() const = 0;

  
  virtual scoped_refptr<DevToolsAgentHost> GetAgentHost() const = 0;

  
  virtual bool Activate() const = 0;

  
  virtual bool Close() const = 0;
};

}  

#endif  

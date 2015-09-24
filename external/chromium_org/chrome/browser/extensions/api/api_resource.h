// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_API_RESOURCE_H_
#define CHROME_BROWSER_EXTENSIONS_API_API_RESOURCE_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/browser_thread.h"
#include "extensions/common/extension.h"

namespace extensions {

class ApiResource {
 public:
  virtual ~ApiResource();

  const std::string& owner_extension_id() const {
    return owner_extension_id_;
  }

  
  
  virtual bool IsPersistent() const;

  static const content::BrowserThread::ID kThreadId =
      content::BrowserThread::IO;

 protected:
  explicit ApiResource(const std::string& owner_extension_id);

 private:
  
  const std::string owner_extension_id_;

  DISALLOW_COPY_AND_ASSIGN(ApiResource);
};

}  

#endif  

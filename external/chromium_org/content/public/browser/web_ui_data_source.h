// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WEB_UI_DATA_SOURCE_H_
#define CONTENT_PUBLIC_BROWSER_WEB_UI_DATA_SOURCE_H_

#include "base/callback.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"

namespace base {
class DictionaryValue;
class RefCountedMemory;
}

namespace content {
class BrowserContext;

class WebUIDataSource {
 public:
  virtual ~WebUIDataSource() {}

  CONTENT_EXPORT static WebUIDataSource* Create(const std::string& source_name);

  
  CONTENT_EXPORT static void Add(BrowserContext* browser_context,
                                 WebUIDataSource* source);

  
  virtual void AddString(const std::string& name,
                         const base::string16& value) = 0;

  
  virtual void AddString(const std::string& name, const std::string& value) = 0;

  
  
  virtual void AddLocalizedString(const std::string& name, int ids) = 0;

  
  virtual void AddLocalizedStrings(
      const base::DictionaryValue& localized_strings) = 0;

  
  virtual void AddBoolean(const std::string& name, bool value) = 0;

  
  virtual void SetJsonPath(const std::string& path) = 0;

  
  
  virtual void SetUseJsonJSFormatV2() = 0;

  
  virtual void AddResourcePath(const std::string &path, int resource_id) = 0;

  
  virtual void SetDefaultResource(int resource_id) = 0;

  
  
  
  typedef base::Callback<void(base::RefCountedMemory*)> GotDataCallback;

  
  
  
  
  typedef base::Callback<bool(const std::string&, const GotDataCallback&)>
      HandleRequestCallback;

  
  virtual void SetRequestFilter(const HandleRequestCallback& callback) = 0;

  
  
  
  

  
  virtual void DisableReplaceExistingSource() = 0;
  virtual void DisableContentSecurityPolicy() = 0;
  virtual void OverrideContentSecurityPolicyObjectSrc(
      const std::string& data) = 0;
  virtual void OverrideContentSecurityPolicyFrameSrc(
      const std::string& data) = 0;
  virtual void DisableDenyXFrameOptions() = 0;
};

}  

#endif  

// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MESSAGING_NATIVE_MESSAGING_HOST_MANIFEST_H_
#define CHROME_BROWSER_EXTENSIONS_API_MESSAGING_NATIVE_MESSAGING_HOST_MANIFEST_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "extensions/common/url_pattern_set.h"

namespace base {
class DictionaryValue;
}

namespace extensions {

class NativeMessagingHostManifest {
 public:
  enum HostInterface {
    HOST_INTERFACE_STDIO,
  };

  ~NativeMessagingHostManifest();

  
  
  static bool IsValidName(const std::string& name);

  
  static scoped_ptr<NativeMessagingHostManifest> Load(
      const base::FilePath& file_path,
      std::string* error_message);

  const std::string& name() const { return name_; }
  const std::string& description() const { return description_; }
  HostInterface interface() const { return interface_; }
  const base::FilePath& path() const { return path_; }
  const URLPatternSet& allowed_origins() const { return allowed_origins_; }

 private:
  NativeMessagingHostManifest();

  
  
  bool Parse(base::DictionaryValue* dictionary, std::string* error_message);

  std::string name_;
  std::string description_;
  HostInterface interface_;
  base::FilePath path_;
  URLPatternSet allowed_origins_;

  DISALLOW_COPY_AND_ASSIGN(NativeMessagingHostManifest);
};

}  

#endif  

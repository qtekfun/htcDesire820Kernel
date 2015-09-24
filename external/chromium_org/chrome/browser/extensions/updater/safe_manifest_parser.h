// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_UPDATER_SAFE_MANIFEST_PARSER_H_
#define CHROME_BROWSER_EXTENSIONS_UPDATER_SAFE_MANIFEST_PARSER_H_
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/updater/manifest_fetch_data.h"
#include "chrome/common/extensions/update_manifest.h"
#include "content/public/browser/utility_process_host_client.h"

namespace extensions {

class SafeManifestParser : public content::UtilityProcessHostClient {
 public:
  
  typedef base::Callback<void(const ManifestFetchData&,
                              const UpdateManifest::Results*)> UpdateCallback;

  
  SafeManifestParser(const std::string& xml,
                     ManifestFetchData* fetch_data,
                     const UpdateCallback& update_callback);

  
  
  void Start();

 private:
  virtual ~SafeManifestParser();

  
  void ParseInSandbox();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void OnParseUpdateManifestSucceeded(const UpdateManifest::Results& results);
  void OnParseUpdateManifestFailed(const std::string& error_message);

  const std::string xml_;

  
  scoped_ptr<ManifestFetchData> fetch_data_;
  UpdateCallback update_callback_;

  DISALLOW_COPY_AND_ASSIGN(SafeManifestParser);
};

}  

#endif  

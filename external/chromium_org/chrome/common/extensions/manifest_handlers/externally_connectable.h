// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_EXTERNALLY_CONNECTABLE_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_EXTERNALLY_CONNECTABLE_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "extensions/common/extension.h"
#include "extensions/common/install_warning.h"
#include "extensions/common/manifest_handler.h"
#include "extensions/common/url_pattern_set.h"

class GURL;

namespace base {
class Value;
}

namespace extensions {

namespace externally_connectable_errors {
extern const char kErrorInvalid[];
extern const char kErrorInvalidMatchPattern[];
extern const char kErrorInvalidId[];
extern const char kErrorNothingSpecified[];
extern const char kErrorTopLevelDomainsNotAllowed[];
extern const char kErrorWildcardHostsNotAllowed[];
}  

class ExternallyConnectableHandler : public ManifestHandler {
 public:
  ExternallyConnectableHandler();
  virtual ~ExternallyConnectableHandler();

  virtual bool Parse(Extension* extension, base::string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ExternallyConnectableHandler);
};

struct ExternallyConnectableInfo : public Extension::ManifestData {
 public:
  
  
  static ExternallyConnectableInfo* Get(const Extension* extension);

  
  
  static scoped_ptr<ExternallyConnectableInfo> FromValue(
      const base::Value& value,
      std::vector<InstallWarning>* install_warnings,
      base::string16* error);

  virtual ~ExternallyConnectableInfo();

  
  const URLPatternSet matches;

  
  const std::vector<std::string> ids;

  
  
  const bool all_ids;

  
  
  const bool accepts_tls_channel_id;

  
  
  
  
  bool IdCanConnect(const std::string& id);

  
  ExternallyConnectableInfo(const URLPatternSet& matches,
                            const std::vector<std::string>& ids,
                            bool all_ids,
                            bool accepts_tls_channel_id);

 private:
  DISALLOW_COPY_AND_ASSIGN(ExternallyConnectableInfo);
};

}  

#endif  

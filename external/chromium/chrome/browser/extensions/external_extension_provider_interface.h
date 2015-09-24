// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTERNAL_EXTENSION_PROVIDER_INTERFACE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTERNAL_EXTENSION_PROVIDER_INTERFACE_H_
#pragma once

#include <vector>

#include "base/memory/linked_ptr.h"
#include "chrome/common/extensions/extension.h"

class FilePath;
class Version;

class ExternalExtensionProviderInterface {
 public:
  
  
  
  
  
  class VisitorInterface {
   public:
    virtual void OnExternalExtensionFileFound(
        const std::string& id,
        const Version* version,
        const FilePath& path,
        Extension::Location location) = 0;

    virtual void OnExternalExtensionUpdateUrlFound(
        const std::string& id,
        const GURL& update_url,
        Extension::Location location) = 0;

     
     
    virtual void OnExternalProviderReady() = 0;

   protected:
    virtual ~VisitorInterface() {}
  };

  virtual ~ExternalExtensionProviderInterface() {}

  
  virtual void ServiceShutdown() = 0;

  
  
  
  virtual void VisitRegisteredExtension() const = 0;

  
  virtual bool HasExtension(const std::string& id) const = 0;

  
  
  
  
  virtual bool GetExtensionDetails(const std::string& id,
                                   Extension::Location* location,
                                   scoped_ptr<Version>* version) const = 0;

  
  
  virtual bool IsReady() = 0;
};

typedef std::vector<linked_ptr<ExternalExtensionProviderInterface> >
    ProviderCollection;

#endif  

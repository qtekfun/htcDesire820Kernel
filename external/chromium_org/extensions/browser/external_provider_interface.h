// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_EXTERNAL_PROVIDER_INTERFACE_H_
#define EXTENSIONS_BROWSER_EXTERNAL_PROVIDER_INTERFACE_H_

#include <vector>

#include "base/memory/linked_ptr.h"
#include "extensions/common/manifest.h"

class GURL;

namespace base {
class FilePath;
class Version;
}

namespace extensions {

class ExternalProviderInterface {
 public:
  
  
  
  
  
  class VisitorInterface {
   public:
    
    
    
    virtual bool OnExternalExtensionFileFound(
        const std::string& id,
        const base::Version* version,
        const base::FilePath& path,
        Manifest::Location location,
        int creation_flags,
        bool mark_acknowledged) = 0;

    
    
    
    virtual bool OnExternalExtensionUpdateUrlFound(
        const std::string& id,
        const GURL& update_url,
        Manifest::Location location,
        int creation_flags,
        bool mark_acknowledged) = 0;

    
    
    
    
    
    virtual void OnExternalProviderReady(
        const ExternalProviderInterface* provider) = 0;

   protected:
    virtual ~VisitorInterface() {}
  };

  virtual ~ExternalProviderInterface() {}

  
  virtual void ServiceShutdown() = 0;

  
  
  
  virtual void VisitRegisteredExtension() = 0;

  
  virtual bool HasExtension(const std::string& id) const = 0;

  
  
  
  
  virtual bool GetExtensionDetails(
      const std::string& id,
      Manifest::Location* location,
      scoped_ptr<base::Version>* version) const = 0;

  
  
  virtual bool IsReady() const = 0;
};

typedef std::vector<linked_ptr<ExternalProviderInterface> >
    ProviderCollection;

}  

#endif  

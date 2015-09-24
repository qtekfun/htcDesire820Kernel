// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_COMPONENT_UPDATER_SERVICE_H_

#include <string>
#include <vector>

#include "base/version.h"
#include "url/gurl.h"

namespace base {
class DictionaryValue;
class FilePath;
}

namespace net {
class URLRequestContextGetter;
class URLRequest;
}

namespace component_updater {
class OnDemandTester;
}

namespace content {
class ResourceThrottle;
}

class ComponentPatcher;

class ComponentInstaller {
 public :
  
  
  
  virtual void OnUpdateError(int error) = 0;

  
  
  
  
  virtual bool Install(const base::DictionaryValue& manifest,
                       const base::FilePath& unpack_path) = 0;

  
  
  
  
  virtual bool GetInstalledFile(const std::string& file,
                                base::FilePath* installed_file) = 0;

  virtual ~ComponentInstaller() {}
};

class ComponentObserver {
 public:
  enum Events {
    
    COMPONENT_UPDATER_STARTED,

    
    COMPONENT_UPDATER_SLEEPING,

    
    
    COMPONENT_UPDATE_FOUND,

    
    
    COMPONENT_UPDATE_READY,

    
    COMPONENT_UPDATED,

    
    
    COMPONENT_NOT_UPDATED,
  };

  virtual ~ComponentObserver() {}

  
  
  virtual void OnEvent(Events event, int extra) = 0;
};

struct CrxComponent {
  std::vector<uint8> pk_hash;
  ComponentInstaller* installer;
  ComponentObserver* observer;
  Version version;
  std::string fingerprint;
  std::string name;
  bool allow_background_download;
  CrxComponent();
  ~CrxComponent();
};

std::string GetCrxComponentID(const CrxComponent& component);

struct CrxComponentInfo {
  
  
  std::string id;
  std::string version;
  std::string name;
  CrxComponentInfo();
  ~CrxComponentInfo();
};

class ComponentUpdateService {
 public:
  enum Status {
    kOk,
    kReplaced,
    kInProgress,
    kError
  };
  
  class Configurator {
   public:
    virtual ~Configurator() {}
    
    virtual int InitialDelay() = 0;
    
    virtual int NextCheckDelay() = 0;
    
    virtual int StepDelay() = 0;
    
    
    virtual int StepDelayMedium() = 0;
    
    virtual int MinimumReCheckWait() = 0;
    
    
    virtual int OnDemandDelay() = 0;
    
    virtual GURL UpdateUrl() = 0;
    
    
    virtual GURL PingUrl() = 0;
    
    virtual std::string ExtraRequestParams() = 0;
    
    virtual size_t UrlSizeLimit() = 0;
    
    virtual net::URLRequestContextGetter* RequestContext() = 0;
    
    virtual bool InProcess() = 0;
    
    
    virtual ComponentPatcher* CreateComponentPatcher() = 0;
    
    virtual bool DeltasEnabled() const = 0;
    
    
    virtual bool UseBackgroundDownloader() const = 0;
  };

  
  
  virtual Status Start() = 0;

  
  
  virtual Status Stop() = 0;

  
  
  virtual Status RegisterComponent(const CrxComponent& component) = 0;

  
  virtual void GetComponents(std::vector<CrxComponentInfo>* components) = 0;

  
  
  
  virtual content::ResourceThrottle* GetOnDemandResourceThrottle(
      net::URLRequest* request, const std::string& crx_id) = 0;

  virtual ~ComponentUpdateService() {}

  friend class ComponentsUI;
  friend class component_updater::OnDemandTester;

 private:
  
  
  
  
  
  
  virtual Status OnDemandUpdate(const std::string& component_id) = 0;
};

ComponentUpdateService* ComponentUpdateServiceFactory(
    ComponentUpdateService::Configurator* config);

#endif  

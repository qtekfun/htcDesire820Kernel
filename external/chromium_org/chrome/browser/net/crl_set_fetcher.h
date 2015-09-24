// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CRL_SET_FETCHER_H_
#define CHROME_BROWSER_NET_CRL_SET_FETCHER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/component_updater/component_updater_service.h"

namespace base {
class DictionaryValue;
class FilePath;
}

namespace net {
class CRLSet;
}

class ComponentUpdateService;

class CRLSetFetcher : public ComponentInstaller,
                      public base::RefCountedThreadSafe<CRLSetFetcher> {
 public:
  CRLSetFetcher();

  void StartInitialLoad(ComponentUpdateService* cus);

  
  virtual void OnUpdateError(int error) OVERRIDE;
  virtual bool Install(const base::DictionaryValue& manifest,
                       const base::FilePath& unpack_path) OVERRIDE;
  virtual bool GetInstalledFile(const std::string& file,
                                base::FilePath* installed_file) OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<CRLSetFetcher>;

  virtual ~CRLSetFetcher();

  
  
  bool GetCRLSetFilePath(base::FilePath* path) const;

  
  
  
  void DoInitialLoadFromDisk();

  
  
  void LoadFromDisk(base::FilePath load_from,
                    scoped_refptr<net::CRLSet>* out_crl_set);

  
  
  void SetCRLSetIfNewer(scoped_refptr<net::CRLSet> crl_set);

  
  void RegisterComponent(uint32 sequence_of_loaded_crl);

  ComponentUpdateService* cus_;

  
  
  scoped_refptr<net::CRLSet> crl_set_;

  DISALLOW_COPY_AND_ASSIGN(CRLSetFetcher);
};

#endif  

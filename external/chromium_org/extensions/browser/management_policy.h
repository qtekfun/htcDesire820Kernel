// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_MANAGEMENT_POLICY_H_
#define EXTENSIONS_BROWSER_MANAGEMENT_POLICY_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "extensions/common/extension.h"

namespace extensions {

class ManagementPolicy {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class Provider {
   public:
    Provider() {}
    virtual ~Provider() {}

    
    
    
    virtual std::string GetDebugPolicyProviderName() const = 0;

    
    
    virtual bool UserMayLoad(const Extension* extension,
                             string16* error) const;

    
    
    
    virtual bool UserMayModifySettings(const Extension* extension,
                                       string16* error) const;

    
    
    
    
    
    virtual bool MustRemainEnabled(const Extension* extension,
                                   string16* error) const;

    
    
    virtual bool MustRemainDisabled(const Extension* extension,
                                    Extension::DisableReason* reason,
                                    string16* error) const;

   private:
    DISALLOW_COPY_AND_ASSIGN(Provider);
  };

  ManagementPolicy();
  ~ManagementPolicy();

  
  
  
  void RegisterProvider(Provider* provider);
  void UnregisterProvider(Provider* provider);

  
  
  bool UserMayLoad(const Extension* extension, string16* error) const;

  
  
  
  bool UserMayModifySettings(const Extension* extension,
                             string16* error) const;

  
  
  
  bool MustRemainEnabled(const Extension* extension,
                         string16* error) const;

  
  
  bool MustRemainDisabled(const Extension* extension,
                          Extension::DisableReason* reason,
                          string16* error) const;

  
  void UnregisterAllProviders();
  int GetNumProviders() const;

 private:
  
  
  typedef bool (Provider::*ProviderFunction)(const Extension*, string16*) const;

  typedef std::set<Provider*> ProviderList;

  
  
  
  
  
  bool ApplyToProviderList(ProviderFunction function,
                           const char* debug_operation_name,
                           bool normal_result,
                           const Extension* extension,
                           string16* error) const;

  ProviderList providers_;

  DISALLOW_COPY_AND_ASSIGN(ManagementPolicy);
};

}  

#endif  

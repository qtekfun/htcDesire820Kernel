// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_SPECIAL_STORAGE_POLICY_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_SPECIAL_STORAGE_POLICY_H_
#pragma once

#include <map>
#include <string>

#include "base/synchronization/lock.h"
#include "googleurl/src/gurl.h"
#include "webkit/quota/special_storage_policy.h"

class Extension;

class ExtensionSpecialStoragePolicy : public quota::SpecialStoragePolicy {
 public:
  ExtensionSpecialStoragePolicy();

  
  
  virtual bool IsStorageProtected(const GURL& origin);
  virtual bool IsStorageUnlimited(const GURL& origin);
  virtual bool IsFileHandler(const std::string& extension_id);

  
  void GrantRightsForExtension(const Extension* extension);
  void RevokeRightsForExtension(const Extension* extension);
  void RevokeRightsForAllExtensions();

 private:
  class SpecialCollection {
   public:
    SpecialCollection();
    ~SpecialCollection();

    bool Contains(const GURL& origin);
    bool ContainsExtension(const std::string& extension_id);
    void Add(const Extension* extension);
    void Remove(const Extension* extension);
    void Clear();

   private:
    typedef std::map<GURL, bool> CachedResults;
    typedef std::map<std::string, scoped_refptr<const Extension> > Extensions;
    Extensions extensions_;
    CachedResults cached_results_;
  };

  virtual ~ExtensionSpecialStoragePolicy();

  base::Lock lock_;  
  SpecialCollection protected_apps_;
  SpecialCollection unlimited_extensions_;
  SpecialCollection file_handler_extensions_;
};

#endif  

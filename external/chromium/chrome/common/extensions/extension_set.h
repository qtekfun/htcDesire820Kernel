// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_SET_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_SET_H_
#pragma once

#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "chrome/common/extensions/extension.h"
#include "googleurl/src/gurl.h"

class ExtensionSet {
 public:
  ExtensionSet();
  ~ExtensionSet();

  
  size_t size() const;

  
  bool Contains(const std::string& id);

  
  
  void Insert(const scoped_refptr<const Extension>& extension);

  
  void Remove(const std::string& id);

  
  
  std::string GetIdByURL(const GURL& url) const;

  
  
  
  
  
  const Extension* GetByURL(const GURL& url) const;

  
  
  bool InSameExtent(const GURL& old_url, const GURL& new_url) const;

  
  const Extension* GetByID(const std::string& id) const;

  
  
  
  bool ExtensionBindingsAllowed(const GURL& url) const;

 private:
  FRIEND_TEST_ALL_PREFIXES(ExtensionSetTest, ExtensionSet);

  
  typedef std::map<std::string, scoped_refptr<const Extension> > ExtensionMap;
  ExtensionMap extensions_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionSet);
};

#endif  

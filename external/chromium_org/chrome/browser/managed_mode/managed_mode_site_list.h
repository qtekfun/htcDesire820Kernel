// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MANAGED_MODE_MANAGED_MODE_SITE_LIST_H_
#define CHROME_BROWSER_MANAGED_MODE_MANAGED_MODE_SITE_LIST_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"

class Profile;

namespace base {
class DictionaryValue;
class ListValue;
}

class ManagedModeSiteList {
 public:
  struct Site {
    Site(const base::string16& name, int category_id);
    ~Site();

    
    base::string16 name;

    
    
    
    int category_id;

    
    std::vector<std::string> patterns;

    
    
    std::vector<std::string> hostname_hashes;
  };

  ManagedModeSiteList(const std::string& extension_id,
                      const base::FilePath& path);
  ~ManagedModeSiteList();

  
  
  ManagedModeSiteList* Clone();

  
  
  
  static void GetCategoryNames(std::vector<base::string16>* categories);

  
  void GetSites(std::vector<Site>* sites);

 private:
  bool LazyLoad();
  void CopyThumbnailUrl(const base::DictionaryValue* source,
                        base::DictionaryValue* dest);

  std::string extension_id_;
  base::FilePath path_;
  scoped_ptr<base::DictionaryValue> categories_;
  scoped_ptr<base::ListValue> sites_;

  DISALLOW_COPY_AND_ASSIGN(ManagedModeSiteList);
};

#endif  

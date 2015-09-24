// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_EXTERNAL_PREF_CACHE_LOADER_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_EXTERNAL_PREF_CACHE_LOADER_H_

#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/external_pref_loader.h"

class Profile;

namespace chromeos {

class ExternalCacheDispatcher;

class ExternalPrefCacheLoader : public extensions::ExternalPrefLoader {
 public:
  
  
  
  
  ExternalPrefCacheLoader(int base_path_id, Profile* profile);

  void OnExtensionListsUpdated(const base::DictionaryValue* prefs);

 private:
  friend class base::RefCountedThreadSafe<ExternalLoader>;

  virtual ~ExternalPrefCacheLoader();

  virtual void StartLoading() OVERRIDE;
  virtual void LoadFinished() OVERRIDE;

  Profile* profile_;
  scoped_refptr<ExternalCacheDispatcher> cache_dispatcher_;

  DISALLOW_COPY_AND_ASSIGN(ExternalPrefCacheLoader);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTERNAL_PREF_LOADER_H_
#define CHROME_BROWSER_EXTENSIONS_EXTERNAL_PREF_LOADER_H_

#include "chrome/browser/extensions/external_loader.h"

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"

namespace extensions {

class ExternalPrefLoader : public ExternalLoader {
 public:
  enum Options {
    NONE = 0,

    
    
    
    ENSURE_PATH_CONTROLLED_BY_ADMIN = 1 << 0
  };

  
  
  
  ExternalPrefLoader(int base_path_id, Options options);

  virtual const base::FilePath GetBaseCrxFilePath() OVERRIDE;

 protected:
  virtual ~ExternalPrefLoader() {}

  virtual void StartLoading() OVERRIDE;
  bool IsOptionSet(Options option) {
    return (options_ & option) != 0;
  }

  
  
  const int base_path_id_;

  const Options options_;

 private:
  friend class base::RefCountedThreadSafe<ExternalLoader>;

  
  
  
  void LoadOnFileThread();

  
  
  
  
  void ReadExternalExtensionPrefFile(DictionaryValue * prefs);

  
  
  
  
  
  void ReadStandaloneExtensionPrefFiles(DictionaryValue * prefs);

  
  
  base::FilePath base_path_;

  DISALLOW_COPY_AND_ASSIGN(ExternalPrefLoader);
};

class ExternalTestingLoader : public ExternalLoader {
 public:
  ExternalTestingLoader(const std::string& json_data,
                        const base::FilePath& fake_base_path);

  virtual const base::FilePath GetBaseCrxFilePath() OVERRIDE;

 protected:
  virtual void StartLoading() OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<ExternalLoader>;

  virtual ~ExternalTestingLoader();

  base::FilePath fake_base_path_;
  scoped_ptr<DictionaryValue> testing_prefs_;

  DISALLOW_COPY_AND_ASSIGN(ExternalTestingLoader);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_COMPONENT_EXTENSION_IME_MANAGER_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_COMPONENT_EXTENSION_IME_MANAGER_IMPL_H_

#include <set>
#include <vector>

#include "base/bind.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner.h"
#include "base/threading/thread_checker.h"
#include "base/values.h"
#include "chromeos/ime/component_extension_ime_manager.h"

namespace chromeos {

class ComponentExtensionIMEManagerImpl
    : public ComponentExtensionIMEManagerDelegate {
 public:
  ComponentExtensionIMEManagerImpl();
  virtual ~ComponentExtensionIMEManagerImpl();

  
  virtual std::vector<ComponentExtensionIME> ListIME() OVERRIDE;
  virtual bool Load(const std::string& extension_id,
                    const std::string& manifest,
                    const base::FilePath& file_path) OVERRIDE;
  virtual bool Unload(const std::string& extension_id,
                      const base::FilePath& file_path) OVERRIDE;

  
  
  void InitializeAsync(const base::Closure& callback);

  
  
  bool IsInitialized();

 private:
  
  
  
  static void ReadComponentExtensionsInfo(
      std::vector<ComponentExtensionIME>* out_imes);

  
  
  void OnReadComponentExtensionsInfo(std::vector<ComponentExtensionIME>* result,
                                     const base::Closure& callback);

  
  
  static scoped_ptr<DictionaryValue> GetManifest(
      const base::FilePath& file_path);

  
  
  
  static bool ReadExtensionInfo(const DictionaryValue& manifest,
                                const std::string& extension_id,
                                ComponentExtensionIME* out);

  
  
  
  static bool ReadEngineComponent(const DictionaryValue& dict,
                                  ComponentExtensionEngine* out);

  
  bool is_initialized_;

  
  std::vector<ComponentExtensionIME> component_extension_list_;

  
  std::set<std::string> loaded_extension_id_;

  
  base::ThreadChecker thread_checker_;
  base::WeakPtrFactory<ComponentExtensionIMEManagerImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ComponentExtensionIMEManagerImpl);
};

}  

#endif  


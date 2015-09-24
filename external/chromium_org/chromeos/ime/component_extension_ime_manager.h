// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_IME_COMPONENT_EXTENSION_IME_MANAGER_H_
#define CHROMEOS_IME_COMPONENT_EXTENSION_IME_MANAGER_H_

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/ime/input_method_descriptor.h"

namespace chromeos {

struct CHROMEOS_EXPORT ComponentExtensionEngine {
  ComponentExtensionEngine();
  ~ComponentExtensionEngine();
  std::string engine_id;  
  std::string display_name;  
  std::vector<std::string> language_codes;  
  std::string description;  
  std::vector<std::string> layouts;  
};

struct CHROMEOS_EXPORT ComponentExtensionIME {
  ComponentExtensionIME();
  ~ComponentExtensionIME();
  std::string id;  
  std::string manifest;  
  std::string description;  
  GURL options_page_url; 
  GURL input_view_url; 
  base::FilePath path;
  std::vector<ComponentExtensionEngine> engines;
};

class CHROMEOS_EXPORT ComponentExtensionIMEManagerDelegate {
 public:
  ComponentExtensionIMEManagerDelegate();
  virtual ~ComponentExtensionIMEManagerDelegate();

  
  virtual std::vector<ComponentExtensionIME> ListIME() = 0;

  
  
  virtual bool Load(const std::string& extension_id,
                    const std::string& manifest,
                    const base::FilePath& path) = 0;

  
  
  virtual bool Unload(const std::string& extension_id,
                      const base::FilePath& path) = 0;
};

class CHROMEOS_EXPORT ComponentExtensionIMEManager {
 public:
  class Observer {
   public:
    
    virtual void OnInitialized() = 0;
  };

  ComponentExtensionIMEManager();
  virtual ~ComponentExtensionIMEManager();

  
  
  
  void Initialize(scoped_ptr<ComponentExtensionIMEManagerDelegate> delegate);

  
  bool IsInitialized();

  
  
  
  bool LoadComponentExtensionIME(const std::string& input_method_id);

  
  
  
  bool UnloadComponentExtensionIME(const std::string& input_method_id);

  
  
  bool IsWhitelisted(const std::string& input_method_id);

  
  bool IsWhitelistedExtension(const std::string& extension_id);

  
  
  std::string GetId(const std::string& extension_id,
                    const std::string& engine_id);

  
  std::string GetName(const std::string& input_method_id);

  
  std::string GetDescription(const std::string& input_method_id);

  
  std::vector<std::string> ListIMEByLanguage(const std::string& language);

  
  input_method::InputMethodDescriptors GetAllIMEAsInputMethodDescriptor();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 private:
  
  
  
  bool FindEngineEntry(const std::string& input_method_id,
                       ComponentExtensionIME* out_extension,
                       ComponentExtensionEngine* out_engine);
  scoped_ptr<ComponentExtensionIMEManagerDelegate> delegate_;

  std::vector<ComponentExtensionIME> component_extension_imes_;

  ObserverList<Observer> observers_;

  bool is_initialized_;

  DISALLOW_COPY_AND_ASSIGN(ComponentExtensionIMEManager);
};

}  

#endif  

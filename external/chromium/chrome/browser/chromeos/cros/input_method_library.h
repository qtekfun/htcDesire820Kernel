// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_INPUT_METHOD_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_INPUT_METHOD_LIBRARY_H_
#pragma once

#include <string>
#include <utility>

#include "base/observer_list.h"
#include "base/time.h"
#include "base/timer.h"
#include "third_party/cros/chromeos_input_method.h"

namespace chromeos {

class InputMethodLibrary {
 public:
  class Observer {
   public:
    virtual ~Observer() = 0;
    
    virtual void InputMethodChanged(
        InputMethodLibrary* obj,
        const InputMethodDescriptor& current_input_method,
        size_t num_active_input_methods) = 0;

    
    virtual void ActiveInputMethodsChanged(
        InputMethodLibrary* obj,
        const InputMethodDescriptor& current_input_method,
        size_t num_active_input_methods) = 0;

    
    virtual void PreferenceUpdateNeeded(
        InputMethodLibrary* obj,
        const InputMethodDescriptor& previous_input_method,
        const InputMethodDescriptor& current_input_method) = 0;

    
    virtual void PropertyListChanged(
        InputMethodLibrary* obj,
        const ImePropertyList& current_ime_properties) = 0;

    
    virtual void FirstObserverIsAdded(InputMethodLibrary* obj) = 0;
  };
  virtual ~InputMethodLibrary() {}

  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  
  virtual InputMethodDescriptors* GetActiveInputMethods() = 0;

  
  virtual size_t GetNumActiveInputMethods() = 0;

  
  
  
  virtual InputMethodDescriptors* GetSupportedInputMethods() = 0;

  
  virtual void ChangeInputMethod(const std::string& input_method_id) = 0;

  
  
  
  
  
  
  
  virtual void SetImePropertyActivated(const std::string& key,
                                       bool activated) = 0;

  
  virtual bool InputMethodIsActivated(const std::string& input_method_id) = 0;

  
  
  
  
  
  
  
  
  
  virtual bool SetImeConfig(const std::string& section,
                            const std::string& config_name,
                            const ImeConfigValue& value) = 0;

  
  
  virtual std::string GetKeyboardOverlayId(
      const std::string& input_method_id) = 0;

  
  
  
  virtual bool StartInputMethodDaemon() = 0;

  
  virtual void StopInputMethodDaemon() = 0;

  
  
  virtual void SetDeferImeStartup(bool defer) = 0;

  
  
  virtual void SetEnableAutoImeShutdown(bool enable) = 0;


  virtual InputMethodDescriptor previous_input_method() const = 0;
  virtual InputMethodDescriptor current_input_method() const = 0;

  virtual const ImePropertyList& current_ime_properties() const = 0;

  
  
  static InputMethodLibrary* GetImpl(bool stub);
};

}  

#endif  

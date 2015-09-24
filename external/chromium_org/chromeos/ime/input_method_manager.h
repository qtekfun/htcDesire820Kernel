// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_IME_INPUT_METHOD_MANAGER_H_
#define CHROMEOS_IME_INPUT_METHOD_MANAGER_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/ime/input_method_descriptor.h"
#include "chromeos/ime/input_method_property.h"

namespace ui {
class Accelerator;
}  

namespace chromeos {
class ComponentExtensionIMEManager;
class InputMethodEngineInterface;
namespace input_method {

class InputMethodUtil;
class XKeyboard;

class CHROMEOS_EXPORT InputMethodManager {
 public:
  enum State {
    STATE_LOGIN_SCREEN = 0,
    STATE_BROWSER_SCREEN,
    STATE_LOCK_SCREEN,
    STATE_TERMINATING,
  };

  class Observer {
   public:
    virtual ~Observer() {}
    
    
    virtual void InputMethodChanged(InputMethodManager* manager,
                                    bool show_message) = 0;
    
    virtual void InputMethodPropertyChanged(InputMethodManager* manager) = 0;
  };

  
  
  
  
  
  class CandidateWindowObserver {
   public:
    virtual ~CandidateWindowObserver() {}
    
    virtual void CandidateWindowOpened(InputMethodManager* manager) = 0;
    
    virtual void CandidateWindowClosed(InputMethodManager* manager) = 0;
  };

  virtual ~InputMethodManager() {}

  
  
  static CHROMEOS_EXPORT InputMethodManager* Get();

  
  
  
  
  static CHROMEOS_EXPORT void Initialize(InputMethodManager* instance);

  
  static CHROMEOS_EXPORT void Shutdown();

  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void AddCandidateWindowObserver(
      CandidateWindowObserver* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  virtual void RemoveCandidateWindowObserver(
      CandidateWindowObserver* observer) = 0;

  
  
  
  virtual scoped_ptr<InputMethodDescriptors>
      GetSupportedInputMethods() const = 0;

  
  
  virtual scoped_ptr<InputMethodDescriptors> GetActiveInputMethods() const = 0;

  
  
  
  virtual const std::vector<std::string>& GetActiveInputMethodIds() const = 0;

  
  
  virtual size_t GetNumActiveInputMethods() const = 0;

  
  
  virtual void ChangeInputMethod(const std::string& input_method_id) = 0;

  
  
  
  
  
  
  
  
  virtual void EnableLayouts(const std::string& language_code,
                             const std::string& initial_layout) = 0;

  
  virtual void ActivateInputMethodProperty(const std::string& key) = 0;

  
  
  virtual bool EnableInputMethods(
      const std::vector<std::string>& new_active_input_method_ids) = 0;

  
  
  virtual bool EnableInputMethod(
      const std::string& new_active_input_method_id) = 0;

  
  
  virtual void AddInputMethodExtension(
      const std::string& id,
      const std::string& name,
      const std::vector<std::string>& layouts,
      const std::vector<std::string>& languages,
      const GURL& options_url,
      const GURL& inputview_url,
      InputMethodEngineInterface* instance) = 0;

  
  virtual void RemoveInputMethodExtension(const std::string& id) = 0;

  
  virtual void GetInputMethodExtensions(InputMethodDescriptors* result) = 0;

  
  virtual void SetEnabledExtensionImes(std::vector<std::string>* ids) = 0;

  
  virtual void SetInputMethodDefault() = 0;

  
  virtual InputMethodDescriptor GetCurrentInputMethod() const = 0;

  
  virtual InputMethodPropertyList GetCurrentInputMethodProperties() const = 0;

  
  virtual void SetCurrentInputMethodProperties(
      const InputMethodPropertyList& property_list) = 0;

  
  
  virtual XKeyboard* GetXKeyboard() = 0;

  
  virtual InputMethodUtil* GetInputMethodUtil() = 0;

  
  virtual ComponentExtensionIMEManager* GetComponentExtensionIMEManager() = 0;

  
  virtual bool SwitchToNextInputMethod() = 0;

  
  virtual bool SwitchToPreviousInputMethod(
      const ui::Accelerator& accelerator) = 0;

  
  
  virtual bool SwitchInputMethod(const ui::Accelerator& accelerator) = 0;

  
  virtual bool IsLoginKeyboard(const std::string& layout) const = 0;
};

}  
}  

#endif  

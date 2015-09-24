// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_MANAGER_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_MANAGER_IMPL_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/threading/thread_checker.h"
#include "chrome/browser/chromeos/input_method/candidate_window_controller.h"
#include "chrome/browser/chromeos/input_method/input_method_util.h"
#include "chromeos/ime/input_method_manager.h"
#include "chromeos/ime/input_method_whitelist.h"

namespace chromeos {
class ComponentExtensionIMEManager;
class ComponentExtensionIMEManagerDelegate;
class InputMethodEngine;
namespace input_method {
class InputMethodDelegate;
class XKeyboard;

class InputMethodManagerImpl : public InputMethodManager,
                               public CandidateWindowController::Observer {
 public:
  
  
  explicit InputMethodManagerImpl(scoped_ptr<InputMethodDelegate> delegate);
  virtual ~InputMethodManagerImpl();

  
  
  
  
  void Init(base::SequencedTaskRunner* ui_task_runner);

  
  void SetState(State new_state);

  
  virtual void AddObserver(InputMethodManager::Observer* observer) OVERRIDE;
  virtual void AddCandidateWindowObserver(
      InputMethodManager::CandidateWindowObserver* observer) OVERRIDE;
  virtual void RemoveObserver(InputMethodManager::Observer* observer) OVERRIDE;
  virtual void RemoveCandidateWindowObserver(
      InputMethodManager::CandidateWindowObserver* observer) OVERRIDE;
  virtual scoped_ptr<InputMethodDescriptors>
      GetSupportedInputMethods() const OVERRIDE;
  virtual scoped_ptr<InputMethodDescriptors>
      GetActiveInputMethods() const OVERRIDE;
  virtual const std::vector<std::string>& GetActiveInputMethodIds() const
      OVERRIDE;
  virtual size_t GetNumActiveInputMethods() const OVERRIDE;
  virtual void EnableLayouts(const std::string& language_code,
                             const std::string& initial_layout) OVERRIDE;
  virtual bool EnableInputMethods(
      const std::vector<std::string>& new_active_input_method_ids) OVERRIDE;
  virtual bool EnableInputMethod(const std::string& new_active_input_method_id)
      OVERRIDE;
  virtual void ChangeInputMethod(const std::string& input_method_id) OVERRIDE;
  virtual void ActivateInputMethodProperty(const std::string& key) OVERRIDE;
  virtual void AddInputMethodExtension(
      const std::string& id,
      const std::string& name,
      const std::vector<std::string>& layouts,
      const std::vector<std::string>& languages,
      const GURL& options_page,
      const GURL& input_view,
      InputMethodEngineInterface* instance) OVERRIDE;
  virtual void RemoveInputMethodExtension(const std::string& id) OVERRIDE;
  virtual void GetInputMethodExtensions(
      InputMethodDescriptors* result) OVERRIDE;
  virtual void SetEnabledExtensionImes(std::vector<std::string>* ids) OVERRIDE;
  virtual void SetInputMethodDefault() OVERRIDE;
  virtual bool SwitchToNextInputMethod() OVERRIDE;
  virtual bool SwitchToPreviousInputMethod(
      const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool SwitchInputMethod(const ui::Accelerator& accelerator) OVERRIDE;
  virtual InputMethodDescriptor GetCurrentInputMethod() const OVERRIDE;
  virtual InputMethodPropertyList
      GetCurrentInputMethodProperties() const OVERRIDE;
  virtual void SetCurrentInputMethodProperties(
      const InputMethodPropertyList& property_list) OVERRIDE;

  virtual XKeyboard* GetXKeyboard() OVERRIDE;
  virtual InputMethodUtil* GetInputMethodUtil() OVERRIDE;
  virtual ComponentExtensionIMEManager*
      GetComponentExtensionIMEManager() OVERRIDE;
  virtual bool IsLoginKeyboard(const std::string& layout) const OVERRIDE;

  
  void SetCandidateWindowControllerForTesting(
      CandidateWindowController* candidate_window_controller);
  
  void SetXKeyboardForTesting(XKeyboard* xkeyboard);
  
  void InitializeComponentExtensionForTesting(
      scoped_ptr<ComponentExtensionIMEManagerDelegate> delegate);

 private:
  
  void PropertyChanged();

  
  virtual void CandidateClicked(int index) OVERRIDE;
  virtual void CandidateWindowOpened() OVERRIDE;
  virtual void CandidateWindowClosed() OVERRIDE;

  
  
  
  
  void OnScreenLocked();

  
  
  void OnScreenUnlocked();

  
  bool InputMethodIsActivated(const std::string& input_method_id);

  
  
  bool ContainsOnlyKeyboardLayout(const std::vector<std::string>& value);

  
  
  void MaybeInitializeCandidateWindowController();

  
  
  
  void SwitchToNextInputMethodInternal(
      const std::vector<std::string>& input_method_ids,
      const std::string& current_input_method_id);

  
  
  bool ChangeInputMethodInternal(const std::string& input_method_id,
                                 bool show_message);

  
  void OnComponentExtensionInitialized(
      scoped_ptr<ComponentExtensionIMEManagerDelegate> delegate);
  void InitializeComponentExtension();

  
  
  void LoadNecessaryComponentExtensions();

  
  bool EnableInputMethodImpl(
      const std::string& input_method_id,
      std::vector<std::string>* new_active_input_method_ids) const;

  
  
  void ReconfigureIMFramework();

  scoped_ptr<InputMethodDelegate> delegate_;

  
  State state_;

  
  ObserverList<InputMethodManager::Observer> observers_;
  ObserverList<CandidateWindowObserver> candidate_window_observers_;

  
  InputMethodDescriptor previous_input_method_;
  InputMethodDescriptor current_input_method_;
  
  std::vector<std::string> active_input_method_ids_;

  
  std::vector<std::string> enabled_extension_imes_;

  
  
  
  InputMethodDescriptor saved_previous_input_method_;
  InputMethodDescriptor saved_current_input_method_;
  std::vector<std::string> saved_active_input_method_ids_;

  
  
  std::map<std::string, InputMethodDescriptor> extra_input_methods_;

  
  InputMethodPropertyList property_list_;

  
  
  scoped_ptr<CandidateWindowController> candidate_window_controller_;

  
  InputMethodWhitelist whitelist_;

  
  
  InputMethodUtil util_;

  
  scoped_ptr<ComponentExtensionIMEManager> component_extension_ime_manager_;

  
  
  scoped_ptr<XKeyboard> xkeyboard_;

  std::string pending_input_method_;

  base::ThreadChecker thread_checker_;

  base::WeakPtrFactory<InputMethodManagerImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodManagerImpl);
};

}  
}  

#endif  

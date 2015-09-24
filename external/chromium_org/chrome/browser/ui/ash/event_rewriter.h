// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_EVENT_REWRITER_H_
#define CHROME_BROWSER_UI_ASH_EVENT_REWRITER_H_

#include <map>
#include <string>

#include "ash/event_rewriter_delegate.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/root_window_observer.h"
#include "ui/events/keycodes/keyboard_codes.h"

#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/device_hierarchy_observer.h"
#endif

class PrefService;

namespace aura {
class RootWindow;
}

#if defined(OS_CHROMEOS)
namespace chromeos {

class KeyboardDrivenEventRewriter;

namespace input_method {
class XKeyboard;
}
}
#endif

class EventRewriter : public ash::EventRewriterDelegate,
                      public aura::RootWindowObserver
#if defined(OS_CHROMEOS)
                    , public chromeos::DeviceHierarchyObserver
#endif
{
 public:
  enum DeviceType {
    kDeviceUnknown = 0,
    kDeviceAppleKeyboard,
  };

  EventRewriter();
  virtual ~EventRewriter();

  
  DeviceType DeviceAddedForTesting(int device_id,
                                   const std::string& device_name);
  
  void RewriteForTesting(ui::KeyEvent* event);

  const std::map<int, DeviceType>& device_id_to_type_for_testing() const {
    return device_id_to_type_;
  }
  void set_last_device_id_for_testing(int device_id) {
    last_device_id_ = device_id;
  }
  void set_pref_service_for_testing(const PrefService* pref_service) {
    pref_service_for_testing_ = pref_service;
  }
#if defined(OS_CHROMEOS)
  void set_xkeyboard_for_testing(chromeos::input_method::XKeyboard* xkeyboard) {
    xkeyboard_for_testing_ = xkeyboard;
  }
#endif

  
  static DeviceType GetDeviceType(const std::string& device_name);

 private:
  friend class EventRewriterAshTest;

  
  virtual ash::EventRewriterDelegate::Action RewriteOrFilterKeyEvent(
      ui::KeyEvent* event) OVERRIDE;
  virtual ash::EventRewriterDelegate::Action RewriteOrFilterLocatedEvent(
      ui::LocatedEvent* event) OVERRIDE;

  
  virtual void OnKeyboardMappingChanged(const aura::RootWindow* root) OVERRIDE;

#if defined(OS_CHROMEOS)
  
  virtual void DeviceHierarchyChanged() OVERRIDE {}
  virtual void DeviceAdded(int device_id) OVERRIDE;
  virtual void DeviceRemoved(int device_id) OVERRIDE;
  virtual void DeviceKeyPressedOrReleased(int device_id) OVERRIDE;

  
  
  typedef unsigned long KeySym;
  typedef unsigned char KeyCode;

  
  void RefreshKeycodes();
  
  unsigned char NativeKeySymToNativeKeycode(KeySym keysym);

  struct KeyboardRemapping {
    KeySym input_keysym;
    unsigned int input_mods;
    unsigned int input_native_mods;
    KeySym output_keysym;
    ui::KeyboardCode output_keycode;
    unsigned int output_mods;
    unsigned int output_native_mods;
  };

  
  // keys instead of having them rewritten into back, forward, brightness,
  
  
  bool TopRowKeysAreFunctionKeys(ui::KeyEvent* event) const;

  
  
  
  bool RewriteWithKeyboardRemappingsByKeySym(
      const KeyboardRemapping* remappings,
      size_t num_remappings,
      KeySym keysym,
      unsigned int native_mods,
      unsigned int mods,
      KeySym* remapped_native_keysym,
      unsigned int* remapped_native_mods,
      ui::KeyboardCode* remapped_keycode,
      unsigned int* remapped_mods);

  
  
  
  
  
  bool RewriteWithKeyboardRemappingsByKeyCode(
      const KeyboardRemapping* remappings,
      size_t num_remappings,
      KeyCode keycode,
      unsigned int native_mods,
      unsigned int mods,
      KeySym* remapped_native_keysym,
      unsigned int* remapped_native_mods,
      ui::KeyboardCode* remapped_keycode,
      unsigned int* remapped_mods);
#endif

  
  const PrefService* GetPrefService() const;

  
  void Rewrite(ui::KeyEvent* event);

  
  
  bool RewriteModifiers(ui::KeyEvent* event);

  
  
  
  bool RewriteFnKey(ui::KeyEvent* event);

  
  // press/release with the lock.  Returns true when |event| is rewritten.
  bool RewriteNumPadKeys(ui::KeyEvent* event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  // Returns true when the |event| is rewritten.
  bool RewriteExtendedKeys(ui::KeyEvent* event);

  
  
  // rewritten.
  bool RewriteFunctionKeys(ui::KeyEvent* event);

  
  void RewriteLocatedEvent(ui::LocatedEvent* event);

  
  void OverwriteEvent(ui::KeyEvent* event,
                      unsigned int new_native_keycode,
                      unsigned int new_native_state,
                      ui::KeyboardCode new_keycode,
                      int new_flags);

  
  
  DeviceType DeviceAddedInternal(int device_id, const std::string& device_name);

  
  bool IsAppleKeyboard() const;

  
  
  void GetRemappedModifierMasks(int original_flags,
                                unsigned int original_native_modifiers,
                                int* remapped_flags,
                                unsigned int* remapped_native_modifiers) const;

  std::map<int, DeviceType> device_id_to_type_;
  int last_device_id_;

#if defined(OS_CHROMEOS)
  
  base::hash_map<unsigned long, unsigned long> keysym_to_keycode_map_;

  chromeos::input_method::XKeyboard* xkeyboard_for_testing_;

  scoped_ptr<chromeos::KeyboardDrivenEventRewriter>
      keyboard_driven_event_rewriter_;
#endif

  const PrefService* pref_service_for_testing_;

  DISALLOW_COPY_AND_ASSIGN(EventRewriter);
};

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_KEYCODES_DOM4_KEYCODE_CONVERTER_H_
#define UI_EVENTS_KEYCODES_DOM4_KEYCODE_CONVERTER_H_

#include <stdint.h>
#include "base/basictypes.h"
#include "base/memory/singleton.h"


namespace ui {

typedef struct {
  
  
  
  uint32_t usb_keycode;

  
  
  
  
  uint16_t native_keycode;

  
  
  const char* code;
} KeycodeMapEntry;

class KeycodeConverter {
 public:
  static KeycodeConverter* GetInstance();

  
  uint16_t InvalidNativeKeycode();

  
  
  const char* InvalidKeyboardEventCode();

  
  
  const char* NativeKeycodeToCode(uint16_t native_keycode);

  
  uint16_t CodeToNativeKeycode(const char* code);

  
  
  

  
  uint16_t InvalidUsbKeycode();

  
  uint16_t UsbKeycodeToNativeKeycode(uint32_t usb_keycode);

  
  uint32_t NativeKeycodeToUsbKeycode(uint16_t native_keycode);

  
  
  const char* UsbKeycodeToCode(uint32_t usb_keycode);

  
  uint32_t CodeToUsbKeycode(const char* code);

  
  size_t NumKeycodeMapEntriesForTest();
  const KeycodeMapEntry* GetKeycodeMapForTest();

 private:
  KeycodeConverter();
  friend struct DefaultSingletonTraits<KeycodeConverter>;

  DISALLOW_COPY_AND_ASSIGN(KeycodeConverter);
};

}  

#endif  

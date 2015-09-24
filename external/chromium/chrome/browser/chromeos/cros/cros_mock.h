// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_CROS_MOCK_H_
#define CHROME_BROWSER_CHROMEOS_CROS_CROS_MOCK_H_

#include "chrome/browser/chromeos/cros/cros_library.h"
#include "chrome/browser/chromeos/cros/network_library.h"
#include "chrome/test/in_process_browser_test.h"
#include "third_party/cros/chromeos_input_method.h"

namespace chromeos {

class MockCryptohomeLibrary;
class MockKeyboardLibrary;
class MockInputMethodLibrary;
class MockLibraryLoader;
class MockNetworkLibrary;
class MockPowerLibrary;
class MockScreenLockLibrary;
class MockSpeechSynthesisLibrary;
class MockTouchpadLibrary;

class CrosMock {
 public:
  CrosMock();
  virtual ~CrosMock();

  
  
  
  
  void InitStatusAreaMocks();

  
  
  void InitMockLibraryLoader();

  
  void InitMockCryptohomeLibrary();
  void InitMockKeyboardLibrary();
  void InitMockInputMethodLibrary();
  void InitMockNetworkLibrary();
  void InitMockPowerLibrary();
  void InitMockScreenLockLibrary();
  void InitMockSpeechSynthesisLibrary();
  void InitMockTouchpadLibrary();

  
  MockCryptohomeLibrary* mock_cryptohome_library();
  MockKeyboardLibrary* mock_keyboard_library();
  MockInputMethodLibrary* mock_input_method_library();
  MockNetworkLibrary* mock_network_library();
  MockPowerLibrary* mock_power_library();
  MockScreenLockLibrary* mock_screen_lock_library();
  MockSpeechSynthesisLibrary* mock_speech_synthesis_library();
  MockTouchpadLibrary* mock_touchpad_library();

  
  
  
  
  
  
  void SetStatusAreaMocksExpectations();

  
  void SetKeyboardLibraryStatusAreaExpectations();
  void SetInputMethodLibraryStatusAreaExpectations();
  void SetNetworkLibraryStatusAreaExpectations();
  void SetPowerLibraryStatusAreaExpectations();
  void SetPowerLibraryExpectations();
  void SetSpeechSynthesisLibraryExpectations();
  void SetTouchpadLibraryExpectations();

  void TearDownMocks();

  
  
  static InputMethodDescriptors* CreateInputMethodDescriptors();

  
  chromeos::CrosLibrary::TestApi* test_api();

 private:
  
  MockLibraryLoader* loader_;
  MockCryptohomeLibrary* mock_cryptohome_library_;
  MockKeyboardLibrary* mock_keyboard_library_;
  MockInputMethodLibrary* mock_input_method_library_;
  MockNetworkLibrary* mock_network_library_;
  MockPowerLibrary* mock_power_library_;
  MockScreenLockLibrary* mock_screen_lock_library_;
  MockSpeechSynthesisLibrary* mock_speech_synthesis_library_;
  MockTouchpadLibrary* mock_touchpad_library_;

  ImePropertyList ime_properties_;
  InputMethodDescriptor current_input_method_;
  InputMethodDescriptor previous_input_method_;
  WifiNetworkVector wifi_networks_;
  CellularNetworkVector cellular_networks_;
  VirtualNetworkVector virtual_networks_;
  std::string empty_string_;

  DISALLOW_COPY_AND_ASSIGN(CrosMock);
};

}  

#endif  

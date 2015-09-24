// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GAMEPAD_GAMEPAD_TEST_HELPERS_H_
#define CONTENT_BROWSER_GAMEPAD_GAMEPAD_TEST_HELPERS_H_

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "content/browser/gamepad/gamepad_data_fetcher.h"
#include "third_party/WebKit/public/platform/WebGamepads.h"

namespace content {

class GamepadService;

class MockGamepadDataFetcher : public GamepadDataFetcher {
 public:
  
  
  explicit MockGamepadDataFetcher(const blink::WebGamepads& test_data);

  virtual ~MockGamepadDataFetcher();

  
  virtual void GetGamepadData(blink::WebGamepads* pads,
                              bool devices_changed_hint) OVERRIDE;

  
  
  void WaitForDataRead();

  
  void SetTestData(const blink::WebGamepads& new_data);

 private:
  base::Lock lock_;
  blink::WebGamepads test_data_;
  base::WaitableEvent read_data_;

  DISALLOW_COPY_AND_ASSIGN(MockGamepadDataFetcher);
};

class GamepadTestHelper {
 public:
  GamepadTestHelper();
  virtual ~GamepadTestHelper();

  base::MessageLoop& message_loop() { return message_loop_; }

 private:
  
  base::MessageLoop message_loop_;

  DISALLOW_COPY_AND_ASSIGN(GamepadTestHelper);
};

class GamepadServiceTestConstructor : public GamepadTestHelper {
 public:
  explicit GamepadServiceTestConstructor(const blink::WebGamepads& test_data);
  virtual ~GamepadServiceTestConstructor();

  GamepadService* gamepad_service() { return gamepad_service_; }
  MockGamepadDataFetcher* data_fetcher() { return data_fetcher_; }

 private:
  
  GamepadService* gamepad_service_;

  
  MockGamepadDataFetcher* data_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(GamepadServiceTestConstructor);
};

}  

#endif  

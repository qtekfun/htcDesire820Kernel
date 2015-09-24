// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_CONFIGURATION_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_INPUT_METHOD_CONFIGURATION_H_

#include "base/sequenced_task_runner.h"

namespace chromeos {
namespace input_method {

class InputMethodManager;

void Initialize(
    const scoped_refptr<base::SequencedTaskRunner>& ui_task_runner,
    const scoped_refptr<base::SequencedTaskRunner>& file_task_runner);

void InitializeForTesting(InputMethodManager* mock_manager);

void Shutdown();

}  
}  

#endif  

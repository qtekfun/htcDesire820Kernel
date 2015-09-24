// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_UTIL_H_
#define CHROME_BROWSER_INVALIDATION_INVALIDATION_SERVICE_UTIL_H_

#include "jingle/notifier/base/notifier_options.h"

class CommandLine;

namespace invalidation {

notifier::NotifierOptions ParseNotifierOptions(const CommandLine& command_line);

std::string GenerateInvalidatorClientId();

}  

#endif  

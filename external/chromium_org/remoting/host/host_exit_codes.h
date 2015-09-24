// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_HOST_EXIT_CODES_H_
#define REMOTING_HOST_HOST_EXIT_CODES_H_

namespace remoting {

enum HostExitCodes {
  
  kSuccessExitCode = 0,
  kReservedForX11ExitCode = 1,
  kInitializationFailed = 2,
  kUsageExitCode = 3,

  
  kInvalidHostConfigurationExitCode = 100,
  kInvalidHostIdExitCode = 101,
  kInvalidOauthCredentialsExitCode = 102,
  kInvalidHostDomainExitCode = 103,
  kLoginScreenNotSupportedExitCode = 104,
  kUsernameMismatchExitCode = 105,

  
  
  kMinPermanentErrorExitCode = kInvalidHostConfigurationExitCode,
  kMaxPermanentErrorExitCode = kUsernameMismatchExitCode
};

const char* ExitCodeToString(HostExitCodes exit_code);
}  

#endif  

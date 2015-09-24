// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_NACL_COMMON_NACL_CMD_LINE_H_
#define COMPONENTS_NACL_COMMON_NACL_CMD_LINE_H_

class CommandLine;

namespace nacl {
  
  
  void CopyNaClCommandLineArguments(CommandLine* cmd_line);
}

#endif  

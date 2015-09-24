// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_JANKOMETER_H_
#define CHROME_BROWSER_JANKOMETER_H_
#pragma once

class CommandLine;

void InstallJankometer(const CommandLine& parsed_command_line);

void UninstallJankometer();

#endif  

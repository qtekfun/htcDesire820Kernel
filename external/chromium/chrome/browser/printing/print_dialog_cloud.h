// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_DIALOG_CLOUD_H_
#define CHROME_BROWSER_PRINTING_PRINT_DIALOG_CLOUD_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/string16.h"

class FilePath;
class CommandLine;

namespace print_dialog_cloud {

void CreatePrintDialogForFile(const FilePath& path_to_file,
                              const string16& print_job_title,
                              const std::string& file_type,
                              bool modal);

bool CreatePrintDialogFromCommandLine(const CommandLine& command_line);

}  

#endif  

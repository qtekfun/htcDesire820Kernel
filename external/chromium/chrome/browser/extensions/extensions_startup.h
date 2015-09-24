// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSIONS_STARTUP_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSIONS_STARTUP_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/pack_extension_job.h"

class CommandLine;
class Profile;

class ExtensionsStartupUtil : public PackExtensionJob::Client {
 public:
  ExtensionsStartupUtil();
  virtual ~ExtensionsStartupUtil();

  virtual void OnPackSuccess(const FilePath& crx_path,
                             const FilePath& output_private_key_path);
  virtual void OnPackFailure(const std::string& error_message);

  
  
  bool PackExtension(const CommandLine& cmd_line);

  
  
  
  bool UninstallExtension(const CommandLine& cmd_line, Profile* profile);

 private:
  void ShowPackExtensionMessage(const std::wstring& caption,
                                const std::wstring& message);
  scoped_refptr<PackExtensionJob> pack_job_;
  bool pack_job_succeeded_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionsStartupUtil);
};

#endif  

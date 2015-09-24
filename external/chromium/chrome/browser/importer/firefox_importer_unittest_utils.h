// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IMPORTER_FIREFOX_IMPORTER_UNITTEST_UTILS_H_
#define CHROME_BROWSER_IMPORTER_FIREFOX_IMPORTER_UNITTEST_UTILS_H_
#pragma once

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/process_util.h"
#include "chrome/browser/importer/nss_decryptor.h"

class FFDecryptorServerChannelListener;
namespace IPC {
  class Channel;
}  
class MessageLoopForIO;

class FFUnitTestDecryptorProxy {
 public:
  FFUnitTestDecryptorProxy();
  virtual ~FFUnitTestDecryptorProxy();

  
  
  bool Setup(const FilePath& nss_path);

  
  bool DecryptorInit(const FilePath& dll_path, const FilePath& db_path);
  string16 Decrypt(const std::string& crypt);

 private:
#if defined(OS_MACOSX)
  
  
  
  
  bool WaitForClientResponse();

  base::ProcessHandle child_process_;
  scoped_ptr<IPC::Channel> channel_;
  scoped_ptr<FFDecryptorServerChannelListener> listener_;
  scoped_ptr<MessageLoopForIO> message_loop_;
#else
  NSSDecryptor decryptor_;
#endif  
  DISALLOW_COPY_AND_ASSIGN(FFUnitTestDecryptorProxy);
};

#if !defined(OS_MACOSX)
FFUnitTestDecryptorProxy::FFUnitTestDecryptorProxy() {
}

FFUnitTestDecryptorProxy::~FFUnitTestDecryptorProxy() {
}

bool FFUnitTestDecryptorProxy::Setup(const FilePath& nss_path) {
  return true;
}

bool FFUnitTestDecryptorProxy::DecryptorInit(const FilePath& dll_path,
                                             const FilePath& db_path) {
  return decryptor_.Init(dll_path, db_path);
}

string16 FFUnitTestDecryptorProxy::Decrypt(const std::string& crypt) {
  return decryptor_.Decrypt(crypt);
}
#endif  

#endif  

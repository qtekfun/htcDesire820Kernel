// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_TEST_SERVER_H_
#define NET_TEST_TEST_SERVER_H_
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "build/build_config.h"

#include "base/compiler_specific.h"
#include "base/file_path.h"
#include "base/file_util.h"
#include "base/process_util.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_util.h"

#if defined(OS_WIN)
#include "base/win/scoped_handle.h"
#endif

class CommandLine;
class DictionaryValue;
class GURL;

namespace net {

class AddressList;

class TestServer {
 public:
  typedef std::pair<std::string, std::string> StringPair;

  enum Type {
    TYPE_FTP,
    TYPE_HTTP,
    TYPE_HTTPS,
    TYPE_SYNC,
  };

  
  
  struct HTTPSOptions {
    enum ServerCertificate {
      CERT_OK,
      CERT_MISMATCHED_NAME,
      CERT_EXPIRED,
    };

    
    
    enum BulkCipher {
      
      
      BULK_CIPHER_ANY    = 0,

      BULK_CIPHER_RC4    = (1 << 0),
      BULK_CIPHER_AES128 = (1 << 1),
      BULK_CIPHER_AES256 = (1 << 2),

      
      
      
      BULK_CIPHER_3DES   = (1 << 3),
    };

    
    HTTPSOptions();

    
    explicit HTTPSOptions(ServerCertificate cert);
    ~HTTPSOptions();

    
    
    FilePath GetCertificateFile() const;

    
    ServerCertificate server_certificate;

    
    
    bool request_client_certificate;

    
    
    
    
    std::vector<FilePath> client_authorities;

    
    
    
    int bulk_ciphers;
  };

  TestServer(Type type, const FilePath& document_root);

  
  TestServer(const HTTPSOptions& https_options,
             const FilePath& document_root);

  ~TestServer();

  bool Start() WARN_UNUSED_RESULT;

  
  bool Stop();

  const FilePath& document_root() const { return document_root_; }
  const HostPortPair& host_port_pair() const;
  const DictionaryValue& server_data() const;
  std::string GetScheme() const;
  bool GetAddressList(AddressList* address_list) const WARN_UNUSED_RESULT;

  GURL GetURL(const std::string& path) const;

  GURL GetURLWithUser(const std::string& path,
                      const std::string& user) const;

  GURL GetURLWithUserAndPassword(const std::string& path,
                                 const std::string& user,
                                 const std::string& password) const;

  static bool GetFilePathWithReplacements(
      const std::string& original_path,
      const std::vector<StringPair>& text_to_replace,
      std::string* replacement_path);

 private:
  void Init(const FilePath& document_root);

  
  bool SetPythonPath() WARN_UNUSED_RESULT;

  
  bool LaunchPython(const FilePath& testserver_path) WARN_UNUSED_RESULT;

  
  bool WaitToStart() WARN_UNUSED_RESULT;

  
  
  bool ParseServerData(const std::string& server_data) WARN_UNUSED_RESULT;

  
  FilePath GetRootCertificatePath() const;

  
  bool LoadTestRootCert() WARN_UNUSED_RESULT;

  
  
  bool AddCommandLineArguments(CommandLine* command_line) const;

  
  FilePath document_root_;

  
  FilePath certificates_dir_;

  
  HostPortPair host_port_pair_;

  
  scoped_ptr<DictionaryValue> server_data_;

  
  base::ProcessHandle process_handle_;

  scoped_ptr<net::ScopedPortException> allowed_port_;

#if defined(OS_WIN)
  
  base::win::ScopedHandle job_handle_;

  
  base::win::ScopedHandle child_read_fd_;

  
  base::win::ScopedHandle child_write_fd_;
#endif

#if defined(OS_POSIX)
  
  int child_fd_;
  file_util::ScopedFD child_fd_closer_;
#endif

  
  HTTPSOptions https_options_;

  Type type_;

  
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(TestServer);
};

}  

#endif  

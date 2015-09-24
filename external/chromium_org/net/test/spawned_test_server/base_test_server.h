// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TEST_SPAWNED_TEST_SERVER_BASE_TEST_SERVER_H_
#define NET_TEST_SPAWNED_TEST_SERVER_BASE_TEST_SERVER_H_

#include <string>
#include <utility>
#include <vector>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/host_port_pair.h"

class GURL;

namespace base {
class DictionaryValue;
}

namespace net {

class AddressList;
class ScopedPortException;

class BaseTestServer {
 public:
  typedef std::pair<std::string, std::string> StringPair;

  
  
  enum Type {
    TYPE_BASIC_AUTH_PROXY,
    TYPE_FTP,
    TYPE_HTTP,
    TYPE_HTTPS,
    TYPE_WS,
    TYPE_WSS,
    TYPE_TCP_ECHO,
    TYPE_UDP_ECHO,
  };

  
  
  struct SSLOptions {
    enum ServerCertificate {
      CERT_OK,

      
      
      CERT_AUTO,

      CERT_MISMATCHED_NAME,
      CERT_EXPIRED,
      
      
      
      
      CERT_CHAIN_WRONG_ROOT,
    };

    
    
    enum OCSPStatus {
      OCSP_OK,
      OCSP_REVOKED,
      OCSP_INVALID,
      OCSP_UNAUTHORIZED,
      OCSP_UNKNOWN,
    };

    
    
    enum BulkCipher {
      
      
      BULK_CIPHER_ANY    = 0,

      BULK_CIPHER_RC4    = (1 << 0),
      BULK_CIPHER_AES128 = (1 << 1),
      BULK_CIPHER_AES256 = (1 << 2),

      
      
      
      BULK_CIPHER_3DES   = (1 << 3),
    };

    
    
    enum TLSIntolerantLevel {
      TLS_INTOLERANT_NONE = 0,
      TLS_INTOLERANT_ALL = 1,  
      TLS_INTOLERANT_TLS1_1 = 2,  
      TLS_INTOLERANT_TLS1_2 = 3,  
    };

    
    SSLOptions();

    
    explicit SSLOptions(ServerCertificate cert);
    ~SSLOptions();

    
    
    base::FilePath GetCertificateFile() const;

    
    
    std::string GetOCSPArgument() const;

    
    ServerCertificate server_certificate;

    
    
    OCSPStatus ocsp_status;

    
    
    uint64 cert_serial;

    
    
    bool request_client_certificate;

    
    
    
    
    std::vector<base::FilePath> client_authorities;

    
    
    
    int bulk_ciphers;

    
    
    
    bool record_resume;

    
    
    TLSIntolerantLevel tls_intolerant;

    
    
    
    
    
    bool fallback_scsv_enabled;

    
    
    
    
    
    
    std::string signed_cert_timestamps_tls_ext;

    
    bool staple_ocsp_response;
  };

  
  static const char kLocalhost[];

  
  BaseTestServer(Type type,  const std::string& host);

  
  BaseTestServer(Type type, const SSLOptions& ssl_options);

  
  
  const HostPortPair& host_port_pair() const;

  const base::FilePath& document_root() const { return document_root_; }
  const base::DictionaryValue& server_data() const;
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

  static bool UsingSSL(Type type) {
    return type == BaseTestServer::TYPE_HTTPS ||
           type == BaseTestServer::TYPE_WSS;
  }

 protected:
  virtual ~BaseTestServer();
  Type type() const { return type_; }

  
  
  uint16 GetPort();

  
  void SetPort(uint16 port);

  
  bool SetupWhenServerStarted() WARN_UNUSED_RESULT;

  
  void CleanUpWhenStoppingServer();

  
  void SetResourcePath(const base::FilePath& document_root,
                       const base::FilePath& certificates_dir);

  
  
  bool ParseServerData(const std::string& server_data) WARN_UNUSED_RESULT;

  
  
  bool GenerateArguments(base::DictionaryValue* arguments) const
    WARN_UNUSED_RESULT;

  
  
  virtual bool GenerateAdditionalArguments(
      base::DictionaryValue* arguments) const WARN_UNUSED_RESULT;

 private:
  void Init(const std::string& host);

  
  
  bool LoadTestRootCert() const WARN_UNUSED_RESULT;

  
  base::FilePath document_root_;

  
  base::FilePath certificates_dir_;

  
  HostPortPair host_port_pair_;

  
  scoped_ptr<base::DictionaryValue> server_data_;

  
  
  SSLOptions ssl_options_;

  Type type_;

  
  bool started_;

  
  bool log_to_console_;

  scoped_ptr<ScopedPortException> allowed_port_;

  DISALLOW_COPY_AND_ASSIGN(BaseTestServer);
};

}  

#endif  

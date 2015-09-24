// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_TEST_LOCAL_POLICY_TEST_SERVER_H_
#define CHROME_BROWSER_POLICY_TEST_LOCAL_POLICY_TEST_SERVER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/values.h"
#include "net/test/spawned_test_server/local_test_server.h"
#include "url/gurl.h"

namespace crypto {
class RSAPrivateKey;
}

namespace policy {

class LocalPolicyTestServer : public net::LocalTestServer {
 public:
  
  
  LocalPolicyTestServer();

  
  
  explicit LocalPolicyTestServer(const base::FilePath& config_file);

  
  
  explicit LocalPolicyTestServer(const std::string& test_name);

  virtual ~LocalPolicyTestServer();

  
  
  
  bool SetSigningKey(const crypto::RSAPrivateKey* key);

  
  
  
  
  void RegisterClient(const std::string& dm_token,
                      const std::string& device_id);

  
  
  
  
  
  
  
  bool UpdatePolicy(const std::string& type,
                    const std::string& entity_id,
                    const std::string& policy);

  
  
  
  
  
  
  
  
  
  
  bool UpdatePolicyData(const std::string& type,
                        const std::string& entity_id,
                        const std::string& data);

  
  GURL GetServiceURL() const;

  
  virtual bool SetPythonPath() const OVERRIDE;
  virtual bool GetTestServerPath(
      base::FilePath* testserver_path) const OVERRIDE;
  virtual bool GenerateAdditionalArguments(
      base::DictionaryValue* arguments) const OVERRIDE;

 private:
  std::string GetSelector(const std::string& type,
                          const std::string& entity_id);

  base::FilePath config_file_;
  base::FilePath policy_key_;
  base::DictionaryValue clients_;
  base::ScopedTempDir server_data_dir_;

  DISALLOW_COPY_AND_ASSIGN(LocalPolicyTestServer);
};

}  

#endif  

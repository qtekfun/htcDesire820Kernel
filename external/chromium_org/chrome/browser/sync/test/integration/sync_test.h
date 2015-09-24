// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_TEST_INTEGRATION_SYNC_TEST_H_
#define CHROME_BROWSER_SYNC_TEST_INTEGRATION_SYNC_TEST_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "net/dns/mock_host_resolver.h"
#include "net/http/http_status_code.h"
#include "net/url_request/url_request_status.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/protocol/sync_protocol_error.h"
#include "sync/test/local_sync_test_server.h"


class CommandLine;
class Profile;
class ProfileSyncServiceHarness;

namespace net {
class FakeURLFetcherFactory;
class ProxyConfig;
class ScopedDefaultHostResolverProc;
class URLFetcherImplFactory;
class URLRequestContextGetter;
}

class SyncTest : public InProcessBrowserTest {
 public:
  
  enum TestType {
    
    
    SINGLE_CLIENT,

    
    
    TWO_CLIENT,

    
    
    
    MULTIPLE_CLIENT
  };

  
  enum ServerType {
    SERVER_TYPE_UNDECIDED,
    LOCAL_PYTHON_SERVER,   
                           
    LOCAL_LIVE_SERVER,     
                           
                           
    EXTERNAL_LIVE_SERVER,  
                           
                           
  };

  
  
  enum SyncErrorFrequency {
    
    ERROR_FREQUENCY_NONE,

    
    ERROR_FREQUENCY_ALWAYS,

    
    
    
    ERROR_FREQUENCY_TWO_THIRDS
  };

  
  enum PythonServerAuthState {
    
    AUTHENTICATED_TRUE,

    
    AUTHENTICATED_FALSE
  };

  
  explicit SyncTest(TestType test_type);

  virtual ~SyncTest();

  
  
  virtual void SetUp() OVERRIDE;

  
  virtual void TearDown() OVERRIDE;

  
  virtual void SetUpCommandLine(CommandLine* cl) OVERRIDE;

  
  int num_clients() WARN_UNUSED_RESULT { return num_clients_; }

  
  
  Profile* GetProfile(int index) WARN_UNUSED_RESULT;

  
  
  Browser* GetBrowser(int index) WARN_UNUSED_RESULT;

  
  
  ProfileSyncServiceHarness* GetClient(int index) WARN_UNUSED_RESULT;

  
  
  std::vector<ProfileSyncServiceHarness*>& clients() WARN_UNUSED_RESULT {
    return clients_.get();
  }

  
  
  Profile* verifier() WARN_UNUSED_RESULT;

  
  bool use_verifier() WARN_UNUSED_RESULT { return use_verifier_; }

  
  
  
  void DisableVerifier();

  
  virtual bool SetupClients() WARN_UNUSED_RESULT;

  
  virtual bool SetupSync() WARN_UNUSED_RESULT;

  
  virtual void EnableNetwork(Profile* profile);

  
  virtual void DisableNetwork(Profile* profile);

  
  bool EnableEncryption(int index);

  
  bool IsEncryptionComplete(int index);

  
  
  bool AwaitQuiescence();

  
  
  bool ServerSupportsNotificationControl() const;

  
  
  void DisableNotifications();

  
  
  void EnableNotifications();

  
  
  void SetOAuth2TokenResponse(const std::string& response_data,
                              net::HttpStatusCode response_code,
                              net::URLRequestStatus::Status status);

  
  
  
  void TriggerNotification(syncer::ModelTypeSet changed_types);

  
  bool ServerSupportsErrorTriggering() const;

  
  
  
  void TriggerMigrationDoneError(syncer::ModelTypeSet model_types);

  
  
  void TriggerBirthdayError();

  
  
  void TriggerTransientError();

  
  
  
  
  void TriggerAuthState(PythonServerAuthState auth_state);

  
  
  void TriggerXmppAuthError();

  
  
  
  void TriggerSyncError(const syncer::SyncProtocolError& error,
                        SyncErrorFrequency frequency);

  
  void TriggerCreateSyncedBookmarks();

  
  int NumberOfDefaultSyncItems() const;

 protected:
  
  virtual void AddTestSwitches(CommandLine* cl);

  
  
  virtual void AddOptionalTypesToCommandLine(CommandLine* cl);

  
  
  virtual void CleanUpOnMainThread() OVERRIDE;

  
  
  virtual void SetUpInProcessBrowserTestFixture() OVERRIDE;

  
  
  virtual void TearDownInProcessBrowserTestFixture() OVERRIDE;

  
  
  virtual void InitializeInstance(int index);

  
  
  void DisableNotificationsImpl();
  void EnableNotificationsImpl();

  
  std::string username_;

  
  std::string password_;

  
  base::FilePath password_file_;

 private:
  
  static Profile* MakeProfile(const base::FilePath::StringType name);

  
  
  
  
  void ReadPasswordFile();

  
  void SetUpTestServerIfRequired();

  
  
  
  bool SetUpLocalPythonTestServer();

  
  
  bool SetUpLocalTestServer();

  
  
  bool TearDownLocalPythonTestServer();

  
  
  bool TearDownLocalTestServer();

  
  
  
  
  bool WaitForTestServerToStart(base::TimeDelta wait, int intervals);

  
  bool IsTestServerRunning();

  
  
  void SetProxyConfig(net::URLRequestContextGetter* context,
                      const net::ProxyConfig& proxy_config);

  
  
  
  void SetupMockGaiaResponses();

  
  
  
  void ClearMockGaiaResponses();

  
  syncer::LocalSyncTestServer sync_server_;

  
  scoped_ptr<net::ScopedPortException> xmpp_port_;

  
  
  TestType test_type_;

  
  
  ServerType server_type_;

  
  int num_clients_;

  
  
  
  std::vector<Profile*> profiles_;

  
  
  
  std::vector<Browser*> browsers_;

  
  
  
  ScopedVector<ProfileSyncServiceHarness> clients_;

  
  
  
  Profile* verifier_;

  
  
  bool use_verifier_;

  
  
  bool notifications_enabled_;

  
  
  
  scoped_ptr<net::ScopedDefaultHostResolverProc> mock_host_resolver_override_;

  
  base::ProcessHandle test_server_handle_;

  
  scoped_ptr<net::FakeURLFetcherFactory> fake_factory_;

  
  scoped_ptr<net::URLFetcherImplFactory> factory_;

  
  
  size_t number_of_default_sync_items_;

  DISALLOW_COPY_AND_ASSIGN(SyncTest);
};

#endif  

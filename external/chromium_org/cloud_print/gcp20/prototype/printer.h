// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GCP20_PROTOTYPE_PRINTER_H_
#define GCP20_PROTOTYPE_PRINTER_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "cloud_print/gcp20/prototype/cloud_print_requester.h"
#include "cloud_print/gcp20/prototype/cloud_print_xmpp_listener.h"
#include "cloud_print/gcp20/prototype/dns_sd_server.h"
#include "cloud_print/gcp20/prototype/print_job_handler.h"
#include "cloud_print/gcp20/prototype/printer_state.h"
#include "cloud_print/gcp20/prototype/privet_http_server.h"
#include "cloud_print/gcp20/prototype/x_privet_token.h"

extern const base::FilePath::CharType kPrinterStatePath[];

class Printer : public base::SupportsWeakPtr<Printer>,
                public PrivetHttpServer::Delegate,
                public CloudPrintRequester::Delegate,
                public CloudPrintXmppListener::Delegate {
 public:
  
  Printer();

  
  virtual ~Printer();

  
  bool Start();

  
  bool IsRunning() const;

  
  void Stop();

 private:
  FRIEND_TEST_ALL_PREFIXES(Printer, ValidateCapabilities);

  enum ConnectionState {
    NOT_CONFIGURED,
    OFFLINE,
    ONLINE,
    CONNECTING
  };

  std::string GetRawCdd();

  
  virtual PrivetHttpServer::RegistrationErrorStatus RegistrationStart(
      const std::string& user) OVERRIDE;
  virtual PrivetHttpServer::RegistrationErrorStatus RegistrationGetClaimToken(
      const std::string& user,
      std::string* token,
      std::string* claim_url) OVERRIDE;
  virtual PrivetHttpServer::RegistrationErrorStatus RegistrationComplete(
      const std::string& user,
      std::string* device_id) OVERRIDE;
  virtual PrivetHttpServer::RegistrationErrorStatus RegistrationCancel(
      const std::string& user) OVERRIDE;
  virtual void GetRegistrationServerError(std::string* description) OVERRIDE;
  virtual void CreateInfo(PrivetHttpServer::DeviceInfo* info) OVERRIDE;
  virtual bool IsRegistered() const OVERRIDE;
  virtual bool IsLocalPrintingAllowed() const OVERRIDE;
  virtual bool CheckXPrivetTokenHeader(const std::string& token) const OVERRIDE;
  virtual const base::DictionaryValue& GetCapabilities() OVERRIDE;
  virtual LocalPrintJob::CreateResult CreateJob(
      const std::string& ticket,
      std::string* job_id,
      int* expires_in,
      int* error_timeout,
      std::string* error_description) OVERRIDE;
  virtual LocalPrintJob::SaveResult SubmitDoc(
      const LocalPrintJob& job,
      std::string* job_id,
      int* expires_in,
      std::string* error_description,
      int* timeout) OVERRIDE;
  virtual LocalPrintJob::SaveResult SubmitDocWithId(
      const LocalPrintJob& job,
      const std::string& job_id,
      int* expires_in,
      std::string* error_description,
      int* timeout) OVERRIDE;
  virtual bool GetJobState(const std::string& id,
                           LocalPrintJob::Info* info) OVERRIDE;

  
  virtual void OnRegistrationStartResponseParsed(
      const std::string& registration_token,
      const std::string& complete_invite_url,
      const std::string& device_id) OVERRIDE;
  virtual void OnRegistrationFinished(
      const std::string& refresh_token,
      const std::string& access_token,
      int access_token_expires_in_seconds) OVERRIDE;
  virtual void OnXmppJidReceived(const std::string& xmpp_jid) OVERRIDE;
  virtual void OnAccesstokenReceviced(const std::string& access_token,
                                      int expires_in_seconds) OVERRIDE;
  virtual void OnRegistrationError(const std::string& description) OVERRIDE;
  virtual void OnNetworkError() OVERRIDE;
  virtual void OnServerError(const std::string& description) OVERRIDE;
  virtual void OnAuthError() OVERRIDE;
  virtual std::string GetAccessToken() OVERRIDE;
  virtual void OnPrintJobsAvailable(
      const std::vector<cloud_print_response_parser::Job>& jobs) OVERRIDE;
  virtual void OnPrintJobDownloaded(
      const cloud_print_response_parser::Job& job) OVERRIDE;
  virtual void OnPrintJobDone() OVERRIDE;
  virtual void OnLocalSettingsReceived(
      LocalSettings::State state,
      const LocalSettings& settings) OVERRIDE;
  virtual void OnLocalSettingsUpdated() OVERRIDE;

  
  virtual void OnXmppConnected() OVERRIDE;
  virtual void OnXmppAuthError() OVERRIDE;
  virtual void OnXmppNetworkError() OVERRIDE;
  virtual void OnXmppNewPrintJob(const std::string& device_id) OVERRIDE;
  virtual void OnXmppNewLocalSettings(const std::string& device_id) OVERRIDE;
  virtual void OnXmppDeleteNotification(const std::string& device_id) OVERRIDE;

  
  void TryConnect();

  
  void ConnectXmpp();

  
  
  void OnIdle();

  
  void FetchPrintJobs();

  
  void GetLocalSettings();

  
  void ApplyLocalSettings(const LocalSettings& settings);

  
  void OnPrinterDeleted();

  
  void RememberAccessToken(const std::string& access_token,
                           int expires_in_seconds);

  
  void SetRegistrationError(const std::string& description);

  
  
  
  PrivetHttpServer::RegistrationErrorStatus CheckCommonRegErrors(
      const std::string& user);

  
  void WaitUserConfirmation(base::Time valid_until);

  
  std::string GenerateProxyId() const;

  
  std::vector<std::string> CreateTxt() const;

  
  void SaveToFile();
  bool LoadFromFile();

  
  void PostOnIdle();

  
  void CheckRegistrationExpiration();

  
  void UpdateRegistrationExpiration();

  
  void InvalidateRegistrationExpiration();

  
  
  bool StartLocalDiscoveryServers();

  
  
  bool StartDnsServer();
  bool StartHttpServer();

  
  PrivetHttpServer::RegistrationErrorStatus ConfirmationToRegistrationError(
      PrinterState::ConfirmationState state);

  std::string ConnectionStateToString(ConnectionState state) const;

  
  
  void FallOffline(bool instant_reconnect);

  
  
  bool ChangeState(ConnectionState new_state);

  
  PrinterState state_;

  
  ConnectionState connection_state_;

  
  DnsSdServer dns_server_;

  
  PrivetHttpServer http_server_;

  
  scoped_ptr<CloudPrintRequester> requester_;

  
  scoped_ptr<CloudPrintXmppListener> xmpp_listener_;

  XPrivetToken xtoken_;

  scoped_ptr<PrintJobHandler> print_job_handler_;

  
  base::Time starttime_;

  
  base::Time registration_expiration_;

  
  bool on_idle_posted_;

  
  bool pending_local_settings_check_;

  
  bool pending_print_jobs_check_;

  
  bool pending_deletion_;

  DISALLOW_COPY_AND_ASSIGN(Printer);
};

#endif  


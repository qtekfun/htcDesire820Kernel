// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_CLOUD_REQUESTER_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_CLOUD_REQUESTER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "cloud_print/gcp20/prototype/cloud_print_request.h"
#include "cloud_print/gcp20/prototype/cloud_print_response_parser.h"
#include "cloud_print/gcp20/prototype/local_settings.h"
#include "google_apis/gaia/gaia_oauth_client.h"

class CloudPrintURLRequestContextGetter;
class GURL;
class URLRequestContextGetter;

extern const char kCloudPrintUrl[];

class CloudPrintRequester : public base::SupportsWeakPtr<CloudPrintRequester>,
                            public gaia::GaiaOAuthClient::Delegate,
                            public CloudPrintRequest::Delegate {
 public:
  class Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}

    
    
    virtual void OnRegistrationStartResponseParsed(
        const std::string& registration_token,
        const std::string& complete_invite_url,
        const std::string& device_id) = 0;

    
    
    virtual void OnRegistrationFinished(
        const std::string& refresh_token,
        const std::string& access_token,
        int access_token_expires_in_seconds) = 0;

    
    virtual void OnXmppJidReceived(const std::string& xmpp_jid) = 0;

    
    virtual void OnAccesstokenReceviced(const std::string& access_token,
                                        int expires_in_seconds) = 0;

    
    
    virtual void OnRegistrationError(const std::string& description) = 0;

    
    virtual void OnNetworkError() = 0;

    
    virtual void OnServerError(const std::string& description) = 0;

    
    virtual void OnAuthError() = 0;

    
    virtual std::string GetAccessToken() = 0;

    
    virtual void OnPrintJobsAvailable(
        const std::vector<cloud_print_response_parser::Job>& jobs) = 0;

    
    virtual void OnPrintJobDownloaded(
        const cloud_print_response_parser::Job& job) = 0;

    
    virtual void OnPrintJobDone() = 0;

    
    virtual void OnLocalSettingsReceived(
        LocalSettings::State state,
        const LocalSettings& settings) = 0;

    
    virtual void OnLocalSettingsUpdated() = 0;
  };

  
  CloudPrintRequester(scoped_refptr<base::SingleThreadTaskRunner> task_runner,
                      Delegate* delegate);

  
  virtual ~CloudPrintRequester();

  
  bool IsBusy() const;

  
  void StartRegistration(const std::string& proxy_id,
                         const std::string& device_name,
                         const std::string& user,
                         const LocalSettings& settings,
                         const std::string& cdd);

  
  void CompleteRegistration();

  
  void FetchPrintJobs(const std::string& device_id);

  
  
  void UpdateAccesstoken(const std::string& refresh_token);

  
  void RequestPrintJob(const cloud_print_response_parser::Job& job);

  
  void SendPrintJobDone(const std::string& job_id);

  
  void RequestLocalSettings(const std::string& device_id);

  
  void SendLocalSettings(const std::string& device_id,
                         const LocalSettings& settings);

 private:
  typedef base::Callback<void(const std::string&)> ParserCallback;

  
  virtual void OnFetchComplete(const std::string& response) OVERRIDE;
  virtual void OnFetchError(const std::string& server_api,
                            int server_code,
                            int server_http_code) OVERRIDE;
  virtual void OnFetchTimeoutReached() OVERRIDE;

  
  virtual void OnGetTokensResponse(const std::string& refresh_token,
                                   const std::string& access_token,
                                   int expires_in_seconds) OVERRIDE;
  virtual void OnRefreshTokenResponse(const std::string& access_token,
                                      int expires_in_seconds) OVERRIDE;
  virtual void OnOAuthError() OVERRIDE;
  virtual void OnNetworkError(int response_code) OVERRIDE;

  
  scoped_ptr<CloudPrintRequest> CreateGet(const GURL& url,
                                          const ParserCallback& callback);

  
  scoped_ptr<CloudPrintRequest> CreatePost(const GURL& url,
                                           const std::string& content,
                                           const std::string& mimetype,
                                           const ParserCallback& callback);

  
  void EraseRequest();

  
  void ParseRegisterStart(const std::string& response);

  
  
  void ParseRegisterComplete(const std::string& response);

  
  void ParseFetch(const std::string& response);

  
  void ParseGetPrintJobTicket(const std::string& response);

  
  void ParseGetPrintJobData(const std::string& response);

  
  void ParsePrintJobDone(const std::string& response);

  
  void ParsePrintJobInProgress(const std::string& response);

  
  void ParseLocalSettings(const std::string& response);

  
  void ParseLocalSettingUpdated(const std::string& response);

  
  
  scoped_ptr<CloudPrintRequest> request_;

  
  
  scoped_ptr<cloud_print_response_parser::Job> current_print_job_;

  
  scoped_refptr<net::URLRequestContextGetter> context_getter_;

  
  std::string polling_url_;

  
  gaia::OAuthClientInfo oauth_client_info_;

  
  scoped_ptr<gaia::GaiaOAuthClient> gaia_;

  ParserCallback parser_callback_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintRequester);
};

#endif  


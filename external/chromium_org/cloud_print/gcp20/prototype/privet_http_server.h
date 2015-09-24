// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_PRIVET_HTTP_SERVER_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_PRIVET_HTTP_SERVER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/values.h"
#include "cloud_print/gcp20/prototype/local_print_job.h"
#include "net/server/http_server.h"
#include "net/server/http_server_request_info.h"

class GURL;

class PrivetHttpServer: public net::HttpServer::Delegate {
 public:
  
  enum RegistrationErrorStatus {
    REG_ERROR_OK,

    REG_ERROR_INVALID_PARAMS,
    REG_ERROR_DEVICE_BUSY,
    REG_ERROR_PENDING_USER_ACTION,
    REG_ERROR_USER_CANCEL,
    REG_ERROR_CONFIRMATION_TIMEOUT,
    REG_ERROR_INVALID_ACTION,
    REG_ERROR_OFFLINE,
    REG_ERROR_SERVER_ERROR
  };

  
  struct DeviceInfo {
    DeviceInfo();
    ~DeviceInfo();

    std::string version;
    std::string name;
    std::string description;
    std::string url;
    std::string id;
    std::string device_state;
    std::string connection_state;
    std::string manufacturer;
    std::string model;
    std::string serial_number;
    std::string firmware;
    int uptime;
    std::string x_privet_token;

    std::vector<std::string> api;
    std::vector<std::string> type;
  };

  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual RegistrationErrorStatus RegistrationStart(
        const std::string& user) = 0;

    
    virtual RegistrationErrorStatus RegistrationGetClaimToken(
        const std::string& user,
        std::string* token,
        std::string* claim_url) = 0;

    
    virtual RegistrationErrorStatus RegistrationComplete(
        const std::string& user,
        std::string* device_id) = 0;

    
    virtual RegistrationErrorStatus RegistrationCancel(
        const std::string& user) = 0;

    
    virtual void GetRegistrationServerError(std::string* description) = 0;

    
    virtual void CreateInfo(DeviceInfo* info) = 0;

    
    virtual bool IsRegistered() const = 0;

    
    virtual bool IsLocalPrintingAllowed() const = 0;

    
    virtual bool CheckXPrivetTokenHeader(const std::string& token) const = 0;

    
    virtual const base::DictionaryValue& GetCapabilities() = 0;

    
    virtual LocalPrintJob::CreateResult CreateJob(
        const std::string& ticket,
        std::string* job_id,
        int* expires_in,
        
        int* error_timeout,
        std::string* error_description) = 0;

    
    virtual LocalPrintJob::SaveResult SubmitDoc(
        const LocalPrintJob& job,
        std::string* job_id,
        int* expires_in,
        std::string* error_description,
        int* timeout) = 0;

    
    virtual LocalPrintJob::SaveResult SubmitDocWithId(
        const LocalPrintJob& job,
        const std::string& job_id,
        int* expires_in,
        std::string* error_description,
        int* timeout) = 0;

    
    virtual bool GetJobState(const std::string& job_id,
                             LocalPrintJob::Info* info) = 0;
  };

  
  explicit PrivetHttpServer(Delegate* delegate);

  
  virtual ~PrivetHttpServer();

  
  bool Start(uint16 port);

  
  void Shutdown();

 private:
  
  virtual void OnHttpRequest(
      int connection_id,
      const net::HttpServerRequestInfo& info) OVERRIDE;
  virtual void OnWebSocketRequest(
      int connection_id,
      const net::HttpServerRequestInfo& info) OVERRIDE;
  virtual void OnWebSocketMessage(int connection_id,
                                  const std::string& data) OVERRIDE;
  virtual void OnClose(int connection_id) OVERRIDE;

  
  void ReportInvalidMethod(int connection_id);

  
  
  
  bool ValidateRequestMethod(int connection_id,
                             const std::string& request,
                             const std::string& method);

  
  
  net::HttpStatusCode ProcessHttpRequest(
      const GURL& url,
      const net::HttpServerRequestInfo& info,
      std::string* response);

  
  scoped_ptr<base::DictionaryValue> ProcessInfo(
      net::HttpStatusCode* status_code) const;

  scoped_ptr<base::DictionaryValue> ProcessCapabilities(
      net::HttpStatusCode* status_code) const;

  scoped_ptr<base::DictionaryValue> ProcessCreateJob(
      const GURL& url,
      const std::string& body,
      net::HttpStatusCode* status_code) const;

  scoped_ptr<base::DictionaryValue> ProcessSubmitDoc(
      const GURL& url,
      const net::HttpServerRequestInfo& info,
      net::HttpStatusCode* status_code) const;

  scoped_ptr<base::DictionaryValue> ProcessJobState(
      const GURL& url,
      net::HttpStatusCode* status_code) const;

  scoped_ptr<base::DictionaryValue> ProcessRegister(
      const GURL& url,
      net::HttpStatusCode* status_code) const;

  
  
  void ProcessRegistrationStatus(
      RegistrationErrorStatus status,
      scoped_ptr<base::DictionaryValue>* current_response) const;

  

  uint16 port_;

  
  scoped_refptr<net::HttpServer> server_;

  Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(PrivetHttpServer);
};

#endif  


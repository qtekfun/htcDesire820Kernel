// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_CONNECTOR_H_
#define CHROME_SERVICE_CLOUD_PRINT_CLOUD_PRINT_CONNECTOR_H_

#include <list>
#include <map>
#include <string>

#include "base/threading/thread.h"
#include "base/values.h"
#include "chrome/service/cloud_print/connector_settings.h"
#include "chrome/service/cloud_print/print_system.h"
#include "chrome/service/cloud_print/printer_job_handler.h"

namespace cloud_print {

class CloudPrintConnector
    : public base::RefCountedThreadSafe<CloudPrintConnector>,
      private PrintSystem::PrintServerWatcher::Delegate,
      private PrinterJobHandlerDelegate,
      private CloudPrintURLFetcherDelegate {
 public:
  class Client {
   public:
    virtual void OnAuthFailed() = 0;
    virtual void OnXmppPingUpdated(int ping_timeout) = 0;
   protected:
     virtual ~Client() {}
  };

  CloudPrintConnector(Client* client, const ConnectorSettings& settings);

  bool Start();
  void Stop();
  bool IsRunning();

  
  void GetPrinterIds(std::list<std::string>* printer_ids);

  
  
  void CheckForJobs(const std::string& reason, const std::string& printer_id);

  
  void UpdatePrinterSettings(const std::string& printer_id);

 private:
  friend class base::RefCountedThreadSafe<CloudPrintConnector>;

  
  typedef CloudPrintURLFetcher::ResponseAction
      (CloudPrintConnector::*ResponseHandler)(
          const net::URLFetcher* source,
          const GURL& url,
          DictionaryValue* json_data,
          bool succeeded);

  enum PendingTaskType {
    PENDING_PRINTERS_NONE,
    PENDING_PRINTERS_AVAILABLE,
    PENDING_PRINTER_REGISTER,
    PENDING_PRINTER_DELETE
  };

  
  struct PendingTask {
    PendingTaskType type;
    
    std::string printer_id;  
    printing::PrinterBasicInfo printer_info;  

    PendingTask() : type(PENDING_PRINTERS_NONE) {}
    ~PendingTask() {}
  };

  virtual ~CloudPrintConnector();
  
  virtual void OnPrinterAdded() OVERRIDE;
  
  virtual void OnPrinterDeleted(const std::string& printer_name) OVERRIDE;
  virtual void OnAuthError() OVERRIDE;

  
  virtual CloudPrintURLFetcher::ResponseAction HandleRawData(
      const net::URLFetcher* source,
      const GURL& url,
      const std::string& data) OVERRIDE;
  virtual CloudPrintURLFetcher::ResponseAction HandleJSONData(
      const net::URLFetcher* source,
      const GURL& url,
      base::DictionaryValue* json_data,
      bool succeeded) OVERRIDE;
  virtual CloudPrintURLFetcher::ResponseAction OnRequestAuthError() OVERRIDE;
  virtual std::string GetAuthHeader() OVERRIDE;

  
  CloudPrintURLFetcher::ResponseAction HandlePrinterListResponse(
      const net::URLFetcher* source,
      const GURL& url,
      DictionaryValue* json_data,
      bool succeeded);

  CloudPrintURLFetcher::ResponseAction HandlePrinterListResponseSettingsUpdate(
      const net::URLFetcher* source,
      const GURL& url,
      DictionaryValue* json_data,
      bool succeeded);

  CloudPrintURLFetcher::ResponseAction HandlePrinterDeleteResponse(
      const net::URLFetcher* source,
      const GURL& url,
      DictionaryValue* json_data,
      bool succeeded);

  CloudPrintURLFetcher::ResponseAction HandleRegisterPrinterResponse(
      const net::URLFetcher* source,
      const GURL& url,
      DictionaryValue* json_data,
      bool succeeded);
  

  
  void StartGetRequest(const GURL& url,
                       int max_retries,
                       ResponseHandler handler);
  void StartPostRequest(CloudPrintURLFetcher::RequestType type,
                        const GURL& url,
                        int max_retries,
                        const std::string& mime_type,
                        const std::string& post_data,
                        ResponseHandler handler);

  
  void ReportUserMessage(const std::string& message_id,
                         const std::string& failure_message);

  bool RemovePrinterFromList(const std::string& printer_name,
                             printing::PrinterList* printer_list);

  void InitJobHandlerForPrinter(DictionaryValue* printer_data);

  void UpdateSettingsFromPrintersList(DictionaryValue* json_data);

  void AddPendingAvailableTask();
  void AddPendingDeleteTask(const std::string& id);
  void AddPendingRegisterTask(const printing::PrinterBasicInfo& info);
  void AddPendingTask(const PendingTask& task);
  void ProcessPendingTask();
  void ContinuePendingTaskProcessing();
  void OnPrintersAvailable();
  void OnPrinterRegister(const printing::PrinterBasicInfo& info);
  void OnPrinterDelete(const std::string& name);

  void OnReceivePrinterCaps(
      bool succeeded,
      const std::string& printer_name,
      const printing::PrinterCapsAndDefaults& caps_and_defaults);

  
  void RegisterPrinters(const printing::PrinterList& printers);

  bool IsSamePrinter(const std::string& name1, const std::string& name2) const;
  bool InitPrintSystem();

  void ScheduleStatsReport();
  void ReportStats();

  
  Client* client_;
  
  ConnectorSettings settings_;
  
  scoped_refptr<PrintSystem> print_system_;
  
  scoped_refptr<PrintSystem::PrintServerWatcher>
      print_server_watcher_;
  
  typedef std::map<std::string, scoped_refptr<PrinterJobHandler> >
      JobHandlerMap;
  JobHandlerMap job_handler_map_;
  
  ResponseHandler next_response_handler_;
  
  std::list<PendingTask> pending_tasks_;
  
  scoped_refptr<CloudPrintURLFetcher> request_;
  
  scoped_refptr<CloudPrintURLFetcher> user_message_request_;
  base::WeakPtrFactory<CloudPrintConnector> stats_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(CloudPrintConnector);
};

}  

#endif  


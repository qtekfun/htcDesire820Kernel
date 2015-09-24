// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_PRINTER_JOB_HANDLER_H_
#define CHROME_SERVICE_CLOUD_PRINT_PRINTER_JOB_HANDLER_H_

#include <list>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/threading/thread.h"
#include "base/time/time.h"
#include "chrome/service/cloud_print/cloud_print_url_fetcher.h"
#include "chrome/service/cloud_print/job_status_updater.h"
#include "chrome/service/cloud_print/printer_job_queue_handler.h"
#include "net/url_request/url_request_status.h"
#include "printing/backend/print_backend.h"
#include "url/gurl.h"

class URLFetcher;


namespace cloud_print {

class PrinterJobHandler : public base::RefCountedThreadSafe<PrinterJobHandler>,
                          public CloudPrintURLFetcherDelegate,
                          public JobStatusUpdaterDelegate,
                          public PrintSystem::PrinterWatcher::Delegate,
                          public PrintSystem::JobSpooler::Delegate {
 public:
  class Delegate {
   public:
     
     virtual void OnAuthError() = 0;
     
     virtual void OnPrinterDeleted(const std::string& printer_name) = 0;

   protected:
     virtual ~Delegate() {}
  };

  struct PrinterInfoFromCloud {
    std::string printer_id;
    std::string caps_hash;
    std::string tags_hash;
    int current_xmpp_timeout;
    int pending_xmpp_timeout;

    PrinterInfoFromCloud();
  };

  
  PrinterJobHandler(const printing::PrinterBasicInfo& printer_info,
                    const PrinterInfoFromCloud& printer_info_from_server,
                    const GURL& cloud_print_server_url,
                    PrintSystem* print_system,
                    Delegate* delegate);

  bool Initialize();

  std::string GetPrinterName() const;

  
  
  void CheckForJobs(const std::string& reason);

  
  void Shutdown();

  base::TimeTicks last_job_fetch_time() const { return last_job_fetch_time_; }
  

  

  
  virtual CloudPrintURLFetcher::ResponseAction HandleRawResponse(
      const net::URLFetcher* source,
      const GURL& url,
      const net::URLRequestStatus& status,
      int response_code,
      const net::ResponseCookies& cookies,
      const std::string& data) OVERRIDE;
  virtual CloudPrintURLFetcher::ResponseAction HandleRawData(
      const net::URLFetcher* source,
      const GURL& url,
      const std::string& data) OVERRIDE;
  virtual CloudPrintURLFetcher::ResponseAction HandleJSONData(
      const net::URLFetcher* source,
      const GURL& url,
      base::DictionaryValue* json_data,
      bool succeeded) OVERRIDE;
  virtual void OnRequestGiveUp() OVERRIDE;
  virtual CloudPrintURLFetcher::ResponseAction OnRequestAuthError() OVERRIDE;
  virtual std::string GetAuthHeader() OVERRIDE;

  
  virtual bool OnJobCompleted(JobStatusUpdater* updater) OVERRIDE;
  virtual void OnAuthError() OVERRIDE;

  
  virtual void OnPrinterDeleted() OVERRIDE;
  virtual void OnPrinterChanged() OVERRIDE;
  virtual void OnJobChanged() OVERRIDE;

  
  
  virtual void OnJobSpoolSucceeded(const PlatformJobId& job_id) OVERRIDE;
  virtual void OnJobSpoolFailed() OVERRIDE;

  

  static void ReportsStats();

 private:
  friend class base::RefCountedThreadSafe<PrinterJobHandler>;

  enum PrintJobError {
    JOB_SUCCESS,
    JOB_DOWNLOAD_FAILED,
    JOB_VALIDATE_TICKET_FAILED,
    JOB_FAILED,
    JOB_MAX,
  };

  
  typedef CloudPrintURLFetcher::ResponseAction
      (PrinterJobHandler::*JSONDataHandler)(const net::URLFetcher* source,
                                            const GURL& url,
                                            base::DictionaryValue* json_data,
                                            bool succeeded);
  
  typedef CloudPrintURLFetcher::ResponseAction
      (PrinterJobHandler::*DataHandler)(const net::URLFetcher* source,
                                        const GURL& url,
                                        const std::string& data);

  virtual ~PrinterJobHandler();

  
  CloudPrintURLFetcher::ResponseAction HandlePrinterUpdateResponse(
      const net::URLFetcher* source,
      const GURL& url,
      base::DictionaryValue* json_data,
      bool succeeded);

  CloudPrintURLFetcher::ResponseAction HandleJobMetadataResponse(
      const net::URLFetcher* source,
      const GURL& url,
      base::DictionaryValue* json_data,
      bool succeeded);

  CloudPrintURLFetcher::ResponseAction HandlePrintTicketResponse(
      const net::URLFetcher* source,
      const GURL& url,
      const std::string& data);

  CloudPrintURLFetcher::ResponseAction HandlePrintDataResponse(
      const net::URLFetcher* source,
      const GURL& url,
      const std::string& data);

  CloudPrintURLFetcher::ResponseAction HandleInProgressStatusUpdateResponse(
      const net::URLFetcher* source,
      const GURL& url,
      base::DictionaryValue* json_data,
      bool succeeded);

  CloudPrintURLFetcher::ResponseAction HandleFailureStatusUpdateResponse(
      const net::URLFetcher* source,
      const GURL& url,
      base::DictionaryValue* json_data,
      bool succeeded);
  

  
  
  
  void Start();

  
  
  void Stop();

  void StartPrinting();
  void Reset();
  void UpdateJobStatus(PrintJobStatus status, PrintJobError error);

  
  void RunScheduledJobCheck();

  
  void SetNextJSONHandler(JSONDataHandler handler);
  
  void SetNextDataHandler(DataHandler handler);

  void JobFailed(PrintJobError error);
  void JobSpooled(PlatformJobId local_job_id);
  
  bool UpdatePrinterInfo();
  bool HavePendingTasks();
  void ValidatePrintTicketFailed();

  
  void OnReceivePrinterCaps(
    bool succeeded,
    const std::string& printer_name,
    const printing::PrinterCapsAndDefaults& caps_and_defaults);

  
  void DoPrint(const JobDetails& job_details,
               const std::string& printer_name);

  scoped_refptr<CloudPrintURLFetcher> request_;
  scoped_refptr<PrintSystem> print_system_;
  printing::PrinterBasicInfo printer_info_;
  PrinterInfoFromCloud printer_info_cloud_;
  GURL cloud_print_server_url_;
  std::string print_data_url_;
  JobDetails job_details_;
  Delegate* delegate_;
  
  
  PlatformJobId local_job_id_;

  
  
  JSONDataHandler next_json_data_handler_;
  DataHandler next_data_handler_;
  
  int server_error_count_;
  
  base::Thread print_thread_;
  
  
  scoped_refptr<PrintSystem::JobSpooler> job_spooler_;
  
  
  scoped_refptr<base::MessageLoopProxy> job_handler_message_loop_proxy_;

  
  
  bool shutting_down_;

  
  
  std::string job_fetch_reason_;
  
  bool job_check_pending_;
  bool printer_update_pending_;

  
  int xmpp_ping_interval_;

  
  bool task_in_progress_;
  scoped_refptr<PrintSystem::PrinterWatcher> printer_watcher_;
  typedef std::list< scoped_refptr<JobStatusUpdater> > JobStatusUpdaterList;
  JobStatusUpdaterList job_status_updater_list_;

  
  PrinterJobQueueHandler job_queue_handler_;

  base::TimeTicks last_job_fetch_time_;
  base::WeakPtrFactory<PrinterJobHandler> weak_ptr_factory_;

  base::Time job_start_time_;
  base::Time spooling_start_time_;
  base::Time last_caps_update_time_;

  DISALLOW_COPY_AND_ASSIGN(PrinterJobHandler);
};

typedef PrinterJobHandler::Delegate PrinterJobHandlerDelegate;

}  

#endif  

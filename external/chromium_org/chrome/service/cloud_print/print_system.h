// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_CLOUD_PRINT_PRINT_SYSTEM_H_
#define CHROME_SERVICE_CLOUD_PRINT_PRINT_SYSTEM_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "printing/backend/print_backend.h"

namespace base {
class DictionaryValue;
class FilePath;
}

namespace printing {
struct PrinterBasicInfo;
struct PrinterCapsAndDefaults;
}

namespace cloud_print {

typedef int PlatformJobId;

enum PrintJobStatus {
  PRINT_JOB_STATUS_INVALID,
  PRINT_JOB_STATUS_IN_PROGRESS,
  PRINT_JOB_STATUS_ERROR,
  PRINT_JOB_STATUS_COMPLETED,
  PRINT_JOB_STATUS_MAX,
};

struct PrintJobDetails {
  PrintJobDetails();

  void Clear();

  bool operator ==(const PrintJobDetails& other) const {
    return (status == other.status) &&
           (platform_status_flags == other.platform_status_flags) &&
           (status_message == other.status_message) &&
           (total_pages == other.total_pages) &&
           (pages_printed == other.pages_printed);
  }

  bool operator !=(const PrintJobDetails& other) const {
    return !(*this == other);
  }

  PrintJobStatus status;
  int platform_status_flags;
  std::string status_message;
  int total_pages;
  int pages_printed;
};

class PrintSystem : public base::RefCountedThreadSafe<PrintSystem> {
 public:
  class PrintServerWatcher
      : public base::RefCountedThreadSafe<PrintServerWatcher> {
   public:
    
    class Delegate {
      public:
        virtual void OnPrinterAdded() = 0;
        

      protected:
        virtual ~Delegate() {}
    };

    virtual bool StartWatching(PrintServerWatcher::Delegate* delegate) = 0;
    virtual bool StopWatching() = 0;

   protected:
    friend class base::RefCountedThreadSafe<PrintServerWatcher>;
    virtual ~PrintServerWatcher();
  };

  class PrinterWatcher : public base::RefCountedThreadSafe<PrinterWatcher> {
   public:
    
    class Delegate {
      public:
        virtual void OnPrinterDeleted() = 0;
        virtual void OnPrinterChanged() = 0;
        virtual void OnJobChanged() = 0;

      protected:
        virtual ~Delegate() {}
    };

    virtual bool StartWatching(PrinterWatcher::Delegate* delegate) = 0;
    virtual bool StopWatching() = 0;
    virtual bool GetCurrentPrinterInfo(
        printing::PrinterBasicInfo* printer_info) = 0;

   protected:
    friend class base::RefCountedThreadSafe<PrinterWatcher>;
    virtual ~PrinterWatcher();
  };

  class JobSpooler : public base::RefCountedThreadSafe<JobSpooler> {
   public:
    
    class Delegate {
     public:
      virtual void OnJobSpoolSucceeded(const PlatformJobId& job_id) = 0;
      virtual void OnJobSpoolFailed() = 0;

     protected:
      virtual ~Delegate() {}
    };

    
    
    
    
    virtual bool Spool(const std::string& print_ticket,
                       const base::FilePath& print_data_file_path,
                       const std::string& print_data_mime_type,
                       const std::string& printer_name,
                       const std::string& job_title,
                       const std::vector<std::string>& tags,
                       JobSpooler::Delegate* delegate) = 0;
   protected:
    friend class base::RefCountedThreadSafe<JobSpooler>;
    virtual ~JobSpooler();
  };

  class PrintSystemResult {
   public:
    PrintSystemResult(bool succeeded, const std::string& message)
        : succeeded_(succeeded), message_(message) { }
    bool succeeded() const { return succeeded_; }
    std::string message() const { return message_; }

   private:
    PrintSystemResult() {}

    bool succeeded_;
    std::string message_;
  };

  typedef base::Callback<void(bool,
                              const std::string&,
                              const printing::PrinterCapsAndDefaults&)>
      PrinterCapsAndDefaultsCallback;

  
  
  virtual PrintSystemResult Init() = 0;

  
  virtual PrintSystemResult EnumeratePrinters(
      printing::PrinterList* printer_list) = 0;

  
  virtual void GetPrinterCapsAndDefaults(
      const std::string& printer_name,
      const PrinterCapsAndDefaultsCallback& callback) = 0;

  
  virtual bool IsValidPrinter(const std::string& printer_name) = 0;

  
  virtual bool ValidatePrintTicket(const std::string& printer_name,
                                   const std::string& print_ticket_data) = 0;

  
  virtual bool GetJobDetails(const std::string& printer_name,
                             PlatformJobId job_id,
                             PrintJobDetails* job_details) = 0;

  
  
  virtual PrintServerWatcher* CreatePrintServerWatcher() = 0;
  virtual PrinterWatcher* CreatePrinterWatcher(
      const std::string& printer_name) = 0;
  virtual JobSpooler* CreateJobSpooler() = 0;

  
  
  
  virtual std::string GetSupportedMimeTypes() = 0;

  
  static std::string GenerateProxyId();

  
  
  
  static scoped_refptr<PrintSystem> CreateInstance(
      const base::DictionaryValue* print_system_settings);

 protected:
  friend class base::RefCountedThreadSafe<PrintSystem>;
  virtual ~PrintSystem();
};

}  

#endif  

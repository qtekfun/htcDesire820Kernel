// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_PRINT_JOB_HANDLER_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_PRINT_JOB_HANDLER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "cloud_print/gcp20/prototype/local_print_job.h"

namespace base {

class DictionaryValue;
class Time;

}  

class PrintJobHandler : public base::SupportsWeakPtr<PrintJobHandler> {
 public:
  PrintJobHandler();
  ~PrintJobHandler();

  
  LocalPrintJob::CreateResult CreatePrintJob(
      const std::string& ticket,
      std::string* job_id_out,
      int* expires_in_out,
      int* error_timeout_out,
      std::string* error_description_out);

  
  LocalPrintJob::SaveResult SaveLocalPrintJob(
      const LocalPrintJob& job,
      std::string* job_id_out,
      int* expires_in_out,
      std::string* error_description_out,
      int* timeout_out);

  
  LocalPrintJob::SaveResult CompleteLocalPrintJob(
      const LocalPrintJob& job,
      const std::string& job_id,
      int* expires_in_out,
      std::string* error_description_out,
      int* timeout_out);

  
  bool GetJobState(const std::string& id, LocalPrintJob::Info* info_out);

  
  bool SavePrintJob(const std::string& content,
                    const std::string& ticket,
                    const base::Time& create_time,
                    const std::string& id,
                    const std::string& title,
                    const std::string& file_extension);

 private:
  
  struct LocalPrintJobExtended;

  
  struct LocalPrintJobDraft;

  
  std::map<std::string, LocalPrintJobDraft> drafts;  

  
  std::map<std::string, LocalPrintJobExtended> jobs;  

  
  void SetJobState(const std::string& id, LocalPrintJob::State);

  
  void CompleteDraft(const std::string& id, const LocalPrintJob& job);

  
  
  base::TimeDelta GetJobExpiration(const std::string& id) const;

  
  void ForgetDraft(const std::string& id);

  
  void ForgetLocalJob(const std::string& id);

  DISALLOW_COPY_AND_ASSIGN(PrintJobHandler);
};

#endif  


// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_SERVICE_SERVICE_UTILITY_PROCESS_HOST_H_
#define CHROME_SERVICE_SERVICE_UTILITY_PROCESS_HOST_H_

#include "build/build_config.h"

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process.h"
#include "content/public/common/child_process_host_delegate.h"
#include "ipc/ipc_channel.h"
#include "printing/pdf_render_settings.h"

class CommandLine;

namespace base {
class MessageLoopProxy;
class ScopedTempDir;
}  

namespace content {
class ChildProcessHost;
}

namespace printing {
class Emf;
struct PageRange;
struct PrinterCapsAndDefaults;
}  

class ServiceUtilityProcessHost : public content::ChildProcessHostDelegate {
 public:
  
  
  
  class Client : public base::RefCountedThreadSafe<Client> {
   public:
    Client() {}

    
    virtual void OnChildDied() {}

    
    
    virtual void OnRenderPDFPagesToMetafileSucceeded(
        const printing::Emf& metafile,
        int highest_rendered_page_number,
        double scale_factor) {}
    
    virtual void OnRenderPDFPagesToMetafileFailed() {}

    
    
    virtual void OnGetPrinterCapsAndDefaultsSucceeded(
        const std::string& printer_name,
        const printing::PrinterCapsAndDefaults& caps_and_defaults) {}

    
    
    virtual void OnGetPrinterCapsAndDefaultsFailed(
        const std::string& printer_name) {}

   protected:
    virtual ~Client() {}

   private:
    friend class base::RefCountedThreadSafe<Client>;
    friend class ServiceUtilityProcessHost;

    
    void MetafileAvailable(const base::FilePath& metafile_path,
                           int highest_rendered_page_number,
                           double scale_factor);

    DISALLOW_COPY_AND_ASSIGN(Client);
  };

  ServiceUtilityProcessHost(Client* client,
                            base::MessageLoopProxy* client_message_loop_proxy);
  virtual ~ServiceUtilityProcessHost();

  
  
  
  bool StartRenderPDFPagesToMetafile(
      const base::FilePath& pdf_path,
      const printing::PdfRenderSettings& render_settings,
      const std::vector<printing::PageRange>& page_ranges);

  
  
  
  
  bool StartGetPrinterCapsAndDefaults(const std::string& printer_name);

 protected:
  
  virtual base::FilePath GetUtilityProcessCmd();

  
  virtual void OnChildDisconnected() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  
  
  
  bool StartProcess(bool no_sandbox, const base::FilePath& exposed_dir);

  
  
  
  
  bool Launch(CommandLine* cmd_line,
              bool no_sandbox,
              const base::FilePath& exposed_dir);

  base::ProcessHandle handle() const { return handle_; }

  
  void OnRenderPDFPagesToMetafileSucceeded(int highest_rendered_page_number,
                                           double scale_factor);
  void OnRenderPDFPagesToMetafileFailed();
  void OnGetPrinterCapsAndDefaultsSucceeded(
      const std::string& printer_name,
      const printing::PrinterCapsAndDefaults& caps_and_defaults);
  void OnGetPrinterCapsAndDefaultsFailed(const std::string& printer_name);

  scoped_ptr<content::ChildProcessHost> child_process_host_;
  base::ProcessHandle handle_;
  
  scoped_refptr<Client> client_;
  scoped_refptr<base::MessageLoopProxy> client_message_loop_proxy_;
  bool waiting_for_reply_;
  // The path to the temp file where the metafile will be written to.
  base::FilePath metafile_path_;
  
  scoped_ptr<base::ScopedTempDir> scratch_metafile_dir_;
  
  base::Time start_time_;

  DISALLOW_COPY_AND_ASSIGN(ServiceUtilityProcessHost);
};

#endif  

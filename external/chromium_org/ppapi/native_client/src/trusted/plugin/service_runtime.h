/*
 * Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_SERVICE_RUNTIME_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_SERVICE_RUNTIME_H_

#include <set>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_scoped_ptr.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/shared/platform/nacl_sync.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "native_client/src/trusted/desc/nacl_desc_wrapper.h"
#include "native_client/src/trusted/nonnacl_util/sel_ldr_launcher.h"
#include "native_client/src/trusted/reverse_service/reverse_service.h"
#include "native_client/src/trusted/weak_ref/weak_ref.h"

#include "ppapi/cpp/completion_callback.h"

#include "ppapi/native_client/src/trusted/plugin/utility.h"

struct NaClFileInfo;

namespace nacl {
class DescWrapper;
}  

namespace pp {
class FileIO;
}  

namespace plugin {

class ErrorInfo;
class Manifest;
class Plugin;
class SrpcClient;
class ServiceRuntime;

struct SelLdrStartParams {
  SelLdrStartParams(const nacl::string& url,
                    ErrorInfo* error_info,
                    bool uses_irt,
                    bool uses_ppapi,
                    bool enable_dev_interfaces,
                    bool enable_dyncode_syscalls,
                    bool enable_exception_handling,
                    bool enable_crash_throttling)
      : url(url),
        error_info(error_info),
        uses_irt(uses_irt),
        uses_ppapi(uses_ppapi),
        enable_dev_interfaces(enable_dev_interfaces),
        enable_dyncode_syscalls(enable_dyncode_syscalls),
        enable_exception_handling(enable_exception_handling),
        enable_crash_throttling(enable_crash_throttling) {
  }
  nacl::string url;
  ErrorInfo* error_info;
  bool uses_irt;
  bool uses_ppapi;
  bool enable_dev_interfaces;
  bool enable_dyncode_syscalls;
  bool enable_exception_handling;
  bool enable_crash_throttling;
};


struct LogToJavaScriptConsoleResource {
 public:
  explicit LogToJavaScriptConsoleResource(std::string msg)
      : message(msg) {}
  std::string message;
};

struct PostMessageResource {
 public:
  explicit PostMessageResource(std::string msg)
      : message(msg) {}
  std::string message;
};

struct OpenManifestEntryResource {
 public:
  OpenManifestEntryResource(const std::string& target_url,
                            struct NaClFileInfo* finfo,
                            ErrorInfo* infop,
                            bool* op_complete)
      : url(target_url),
        file_info(finfo),
        error_info(infop),
        op_complete_ptr(op_complete) {}
  std::string url;
  struct NaClFileInfo* file_info;
  ErrorInfo* error_info;
  bool* op_complete_ptr;
};

struct CloseManifestEntryResource {
 public:
  CloseManifestEntryResource(int32_t desc_to_close,
                             bool* op_complete,
                             bool* op_result)
      : desc(desc_to_close),
        op_complete_ptr(op_complete),
        op_result_ptr(op_result) {}

  int32_t desc;
  bool* op_complete_ptr;
  bool* op_result_ptr;
};

struct QuotaRequest {
 public:
  QuotaRequest(PP_Resource pp_resource,
               int64_t start_offset,
               int64_t quota_bytes_requested,
               int64_t* quota_bytes_granted,
               bool* op_complete)
      : resource(pp_resource),
        offset(start_offset),
        bytes_requested(quota_bytes_requested),
        bytes_granted(quota_bytes_granted),
        op_complete_ptr(op_complete) { }

  PP_Resource resource;
  int64_t offset;
  int64_t bytes_requested;
  int64_t* bytes_granted;
  bool* op_complete_ptr;
};

class PluginReverseInterface: public nacl::ReverseInterface {
 public:
  PluginReverseInterface(nacl::WeakRefAnchor* anchor,
                         Plugin* plugin,
                         const Manifest* manifest,
                         ServiceRuntime* service_runtime,
                         pp::CompletionCallback init_done_cb,
                         pp::CompletionCallback crash_cb);

  virtual ~PluginReverseInterface();

  void ShutDown();

  virtual void Log(nacl::string message);

  virtual void DoPostMessage(nacl::string message);

  virtual void StartupInitializationComplete();

  virtual bool EnumerateManifestKeys(std::set<nacl::string>* out_keys);

  virtual bool OpenManifestEntry(nacl::string url_key,
                                 struct NaClFileInfo *info);

  virtual bool CloseManifestEntry(int32_t desc);

  virtual void ReportCrash();

  virtual void ReportExitStatus(int exit_status);

  virtual int64_t RequestQuotaForWrite(nacl::string file_id,
                                       int64_t offset,
                                       int64_t bytes_to_write);

  void AddQuotaManagedFile(const nacl::string& file_id,
                           const pp::FileIO& file_io);
  void AddTempQuotaManagedFile(const nacl::string& file_id);

 protected:
  virtual void Log_MainThreadContinuation(LogToJavaScriptConsoleResource* p,
                                          int32_t err);

  virtual void PostMessage_MainThreadContinuation(PostMessageResource* p,
                                                  int32_t err);

  virtual void OpenManifestEntry_MainThreadContinuation(
      OpenManifestEntryResource* p,
      int32_t err);

  virtual void StreamAsFile_MainThreadContinuation(
      OpenManifestEntryResource* p,
      int32_t result);

  virtual void CloseManifestEntry_MainThreadContinuation(
      CloseManifestEntryResource* cls,
      int32_t err);

 private:
  nacl::WeakRefAnchor* anchor_;  
  Plugin* plugin_;  
                    
  const Manifest* manifest_;
  ServiceRuntime* service_runtime_;
  NaClMutex mu_;
  NaClCondVar cv_;
  std::set<int64_t> quota_files_;
  bool shutting_down_;

  pp::CompletionCallback init_done_cb_;
  pp::CompletionCallback crash_cb_;
};

class ServiceRuntime {
 public:
  
  
  ServiceRuntime(Plugin* plugin,
                 const Manifest* manifest,
                 bool should_report_uma,
                 pp::CompletionCallback init_done_cb,
                 pp::CompletionCallback crash_cb);
  
  ~ServiceRuntime();

  
  
  
  bool StartSelLdr(const SelLdrStartParams& params);

  
  
  void WaitForSelLdrStart();

  
  
  
  void SignalStartSelLdrDone();

  
  
  
  
  bool LoadNexeAndStart(nacl::DescWrapper* nacl_file_desc,
                        ErrorInfo* error_info,
                        const pp::CompletionCallback& crash_cb);

  
  SrpcClient* SetupAppChannel();

  bool Log(int severity, const nacl::string& msg);
  Plugin* plugin() const { return plugin_; }
  void Shutdown();

  
  
  
  
  int exit_status();  
  void set_exit_status(int exit_status);

  nacl::string GetCrashLogOutput();

  
  
  PluginReverseInterface* rev_interface() const { return rev_interface_; }

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(ServiceRuntime);
  bool InitCommunication(nacl::DescWrapper* shm, ErrorInfo* error_info);

  NaClSrpcChannel command_channel_;
  Plugin* plugin_;
  bool main_service_runtime_;
  nacl::ReverseService* reverse_service_;
  nacl::scoped_ptr<nacl::SelLdrLauncherBase> subprocess_;

  nacl::WeakRefAnchor* anchor_;

  PluginReverseInterface* rev_interface_;

  
  
  
  NaClMutex mu_;
  NaClCondVar cond_;
  int exit_status_;
  bool start_sel_ldr_done_;
};

}  

#endif  

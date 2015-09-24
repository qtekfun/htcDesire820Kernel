// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PLUGIN_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PLUGIN_H_

#include <stdio.h>

#include <map>
#include <queue>
#include <set>
#include <string>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_scoped_ptr.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/trusted/validator/nacl_file_info.h"

#include "ppapi/c/private/ppb_nacl_private.h"
#include "ppapi/cpp/private/instance_private.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/view.h"

#include "ppapi/native_client/src/trusted/plugin/file_downloader.h"
#include "ppapi/native_client/src/trusted/plugin/nacl_subprocess.h"
#include "ppapi/native_client/src/trusted/plugin/pnacl_coordinator.h"
#include "ppapi/native_client/src/trusted/plugin/service_runtime.h"
#include "ppapi/native_client/src/trusted/plugin/utility.h"

namespace nacl {
class DescWrapper;
class DescWrapperFactory;
}  

namespace pp {
class URLLoader;
class URLUtil_Dev;
}

namespace ppapi_proxy {
class BrowserPpp;
}

namespace plugin {

class ErrorInfo;
class Manifest;
class ProgressEvent;
class ScriptablePlugin;

class Plugin : public pp::InstancePrivate {
 public:
  
  static Plugin* New(PP_Instance instance);

  

  
  
  
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

  
  virtual bool HandleDocumentLoad(const pp::URLLoader& url_loader);

  
  
  virtual pp::Var GetInstanceObject();

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool LoadNaClModule(nacl::DescWrapper* wrapper, ErrorInfo* error_info,
                      bool enable_dyncode_syscalls,
                      bool enable_exception_handling,
                      bool enable_crash_throttling,
                      const pp::CompletionCallback& init_done_cb,
                      const pp::CompletionCallback& crash_cb);

  
  
  bool LoadNaClModuleContinuationIntern(ErrorInfo* error_info);

  
  
  
  
  
  
  bool LoadNaClModuleContinuation(int32_t pp_error);

  
  
  
  
  
  NaClSubprocess* LoadHelperNaClModule(nacl::DescWrapper* wrapper,
                                       const Manifest* manifest,
                                       ErrorInfo* error_info);

  
  
  char* LookupArgument(const char* key);

  enum LengthComputable {
    LENGTH_IS_NOT_COMPUTABLE = 0,
    LENGTH_IS_COMPUTABLE = 1
  };
  
  void ReportLoadSuccess(LengthComputable length_computable,
                         uint64_t loaded_bytes,
                         uint64_t total_bytes);
  
  void ReportLoadError(const ErrorInfo& error_info);
  
  void ReportLoadAbort();

  
  void AddToConsole(const nacl::string& text);

  
  
  
  
  
  void EnqueueProgressEvent(PP_NaClEventType event_type);
  void EnqueueProgressEvent(PP_NaClEventType event_type,
                            const nacl::string& url,
                            LengthComputable length_computable,
                            uint64_t loaded_bytes,
                            uint64_t total_bytes);

  
  void ReportSelLdrLoadStatus(int status);

  
  void ReportDeadNexe();

  
  
  
  nacl::string plugin_base_url() const { return plugin_base_url_; }
  void set_plugin_base_url(const nacl::string& url) { plugin_base_url_ = url; }
  
  
  nacl::string manifest_base_url() const { return manifest_base_url_; }
  void set_manifest_base_url(const nacl::string& url) {
    manifest_base_url_ = url;
  }

  
  
  const nacl::string& manifest_url() const { return manifest_url_; }
  void set_manifest_url(const nacl::string& manifest_url) {
    manifest_url_ = manifest_url;
  }

  
  enum ReadyState {
    
    UNSENT = 0,
    
    OPENED = 1,
    
    HEADERS_RECEIVED = 2,
    
    LOADING = 3,
    
    
    DONE = 4
  };
  bool nexe_error_reported() const { return nexe_error_reported_; }
  void set_nexe_error_reported(bool val) {
    nexe_error_reported_ = val;
  }

  nacl::DescWrapperFactory* wrapper_factory() const { return wrapper_factory_; }

  
  void RequestNaClManifest(const nacl::string& url);

  
  
  
  static const uint64_t kUnknownBytes = 0;

  
  
  void DispatchProgressEvent(int32_t result);

  
  
  
  bool StreamAsFile(const nacl::string& url,
                    PP_CompletionCallback pp_callback);

  
  
  
  struct NaClFileInfo GetFileInfo(const nacl::string& url);

  
  
  UrlSchemeType GetUrlScheme(const std::string& url);

  
  
  
  bool DocumentCanRequest(const std::string& url);

  void set_last_error_string(const nacl::string& error);

  
  
  
  
  const nacl::string& mime_type() const { return mime_type_; }
  
  static const char* const kNaClMIMEType;
  
  static const char* const kPnaclMIMEType;
  
  bool enable_dev_interfaces() { return enable_dev_interfaces_; }

  Manifest const* manifest() const { return manifest_.get(); }
  const pp::URLUtil_Dev* url_util() const { return url_util_; }

  int exit_status() const { return exit_status_; }
  
  void set_exit_status(int exit_status);

  const PPB_NaCl_Private* nacl_interface() const { return nacl_interface_; }

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(Plugin);
  
  
  explicit Plugin(PP_Instance instance);
  
  
  ~Plugin();

  bool EarlyInit(int argc, const char* argn[], const char* argv[]);
  
  
  void ShutDownSubprocesses();

  ScriptablePlugin* scriptable_plugin() const { return scriptable_plugin_; }
  void set_scriptable_plugin(ScriptablePlugin* scriptable_plugin) {
    scriptable_plugin_ = scriptable_plugin;
  }

  
  ServiceRuntime* main_service_runtime() const {
    return main_subprocess_.service_runtime();
  }

  
  
  bool LoadNaClModuleCommon(nacl::DescWrapper* wrapper,
                            NaClSubprocess* subprocess,
                            const Manifest* manifest,
                            bool should_report_uma,
                            const SelLdrStartParams& params,
                            const pp::CompletionCallback& init_done_cb,
                            const pp::CompletionCallback& crash_cb);

  
  
  
  void StartSelLdrOnMainThread(int32_t pp_error,
                               ServiceRuntime* service_runtime,
                               const SelLdrStartParams& params,
                               bool* success);

  
  
  
  void NexeFileDidOpen(int32_t pp_error);
  void NexeFileDidOpenContinuation(int32_t pp_error);

  
  
  
  
  
  
  
  
  
  
  void NexeDidCrash(int32_t pp_error);

  
  
  
  void BitcodeDidTranslate(int32_t pp_error);
  void BitcodeDidTranslateContinuation(int32_t pp_error);

  
  
  
  
  

  
  void NaClManifestBufferReady(int32_t pp_error);

  
  void NaClManifestFileDidOpen(int32_t pp_error);

  
  void ProcessNaClManifest(const nacl::string& manifest_json);

  
  
  
  
  
  bool SetManifestObject(const nacl::string& manifest_json,
                         ErrorInfo* error_info);

  
  
  void HistogramStartupTimeSmall(const std::string& name, float dt);
  void HistogramStartupTimeMedium(const std::string& name, float dt);

  
  
  bool NexeIsContentHandler() const;

  
  void UrlDidOpenForStreamAsFile(int32_t pp_error,
                                 FileDownloader*& url_downloader,
                                 PP_CompletionCallback pp_callback);

  
  
  
  
  void CopyCrashLogToJsConsole();

  
  
  
  bool OpenURLFast(const nacl::string& url, FileDownloader* downloader);

  void set_nacl_ready_state(ReadyState state);

  void SetExitStatusOnMainThread(int32_t pp_error, int exit_status);

  ScriptablePlugin* scriptable_plugin_;

  int argc_;
  char** argn_;
  char** argv_;

  
  NaClSubprocess main_subprocess_;

  nacl::string plugin_base_url_;
  nacl::string manifest_base_url_;
  nacl::string manifest_url_;
  ReadyState nacl_ready_state_;
  bool nexe_error_reported_;  

  nacl::DescWrapperFactory* wrapper_factory_;

  
  
  
  
  
  
  FileDownloader nexe_downloader_;
  pp::CompletionCallbackFactory<Plugin> callback_factory_;

  nacl::scoped_ptr<PnaclCoordinator> pnacl_coordinator_;

  
  nacl::scoped_ptr<Manifest> manifest_;
  
  const pp::URLUtil_Dev* url_util_;

  
  bool enable_dev_interfaces_;

  
  
  
  
  
  
  
  
  
  bool is_installed_;

  
  
  
  pp::View view_to_replay_;

  
  
  
  pp::URLLoader document_load_to_replay_;

  nacl::string mime_type_;

  
  std::set<FileDownloader*> url_downloaders_;
  
  
  std::map<nacl::string, struct NaClFileInfo> url_file_info_map_;

  
  std::queue<ProgressEvent*> progress_events_;

  
  int64_t load_start_;

  int64_t init_time_;
  int64_t ready_time_;
  size_t nexe_size_;

  
  static void UpdateDownloadProgress(
      PP_Instance pp_instance,
      PP_Resource pp_resource,
      int64_t bytes_sent,
      int64_t total_bytes_to_be_sent,
      int64_t bytes_received,
      int64_t total_bytes_to_be_received);

  
  
  
  const FileDownloader* FindFileDownloader(PP_Resource url_loader) const;

  int64_t time_of_last_progress_event_;
  int exit_status_;

  const PPB_NaCl_Private* nacl_interface_;
};

}  

#endif  

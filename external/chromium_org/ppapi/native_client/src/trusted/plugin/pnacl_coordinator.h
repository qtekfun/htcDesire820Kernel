// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PNACL_COORDINATOR_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PNACL_COORDINATOR_H_

#include <set>
#include <map>
#include <vector>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/shared/platform/nacl_sync_raii.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "native_client/src/trusted/desc/nacl_desc_wrapper.h"

#include "ppapi/cpp/completion_callback.h"

#include "ppapi/native_client/src/trusted/plugin/callback_source.h"
#include "ppapi/native_client/src/trusted/plugin/file_downloader.h"
#include "ppapi/native_client/src/trusted/plugin/nacl_subprocess.h"
#include "ppapi/native_client/src/trusted/plugin/plugin_error.h"
#include "ppapi/native_client/src/trusted/plugin/pnacl_options.h"
#include "ppapi/native_client/src/trusted/plugin/pnacl_resources.h"


namespace plugin {

class Manifest;
class Plugin;
class PnaclCoordinator;
class PnaclTranslateThread;
class TempFile;

class PnaclCoordinator: public CallbackSource<FileStreamData> {
 public:
  virtual ~PnaclCoordinator();

  
  static PnaclCoordinator* BitcodeToNative(
      Plugin* plugin,
      const nacl::string& pexe_url,
      const PnaclOptions& pnacl_options,
      const pp::CompletionCallback& translate_notify_callback);

  
  
  nacl::DescWrapper* ReleaseTranslatedFD();

  
  
  void ReportNonPpapiError(PluginErrorCode err, const nacl::string& message);
  
  
  
  void ReportPpapiError(PluginErrorCode err,
                        int32_t pp_error, const nacl::string& message);
  
  
  
  void ExitWithError();

  
  
  
  
  StreamCallback GetCallback();

  
  
  pp::CompletionCallback GetCompileProgressCallback(int64_t bytes_compiled);

  
  void GetCurrentProgress(int64_t* bytes_loaded, int64_t* bytes_total);

  
  bool ExpectedProgressKnown() { return expected_pexe_size_ != -1; }

  
  
  
  
  bool ShouldDelayProgressEvent() {
    const uint32_t kProgressEventSlopPct = 5;
    return ((expected_pexe_size_ - pexe_bytes_compiled_) * 100 /
            expected_pexe_size_) < kProgressEventSlopPct;
  }

 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(PnaclCoordinator);

  
  
  PnaclCoordinator(Plugin* plugin,
                   const nacl::string& pexe_url,
                   const PnaclOptions& pnacl_options,
                   const pp::CompletionCallback& translate_notify_callback);

  
  void OpenBitcodeStream();
  
  
  void BitcodeStreamDidOpen(int32_t pp_error);

  
  void ResourceInfoWasRead(int32_t pp_error);

  
  void ResourcesDidLoad(int32_t pp_error);
  
  
  void NexeFdDidOpen(int32_t pp_error);
  
  void BitcodeStreamGotData(int32_t pp_error, FileStreamData data);
  
  void BitcodeGotCompiled(int32_t pp_error, int64_t bytes_compiled);
  
  void BitcodeStreamDidFinish(int32_t pp_error);
  
  void ObjectFileDidOpen(int32_t pp_error);
  
  
  
  void RunTranslate(int32_t pp_error);

  
  void TranslateFinished(int32_t pp_error);

  
  void NexeReadDidOpen(int32_t pp_error);

  
  
  int32_t translate_finish_error_;

  
  Plugin* plugin_;

  pp::CompletionCallback translate_notify_callback_;
  
  
  
  
  
  bool translation_finished_reported_;
  
  pp::CompletionCallbackFactory<PnaclCoordinator,
                                pp::ThreadSafeThreadTraits> callback_factory_;

  
  
  nacl::scoped_ptr<const Manifest> manifest_;
  
  nacl::scoped_ptr<PnaclResources> resources_;

  
  nacl::string pexe_url_;
  
  PnaclOptions pnacl_options_;

  
  nacl::scoped_ptr<TempFile> obj_file_;
  
  nacl::scoped_ptr<TempFile> temp_nexe_file_;
  
  
  PP_Bool is_cache_hit_;

  
  nacl::scoped_ptr<FileDownloader> streaming_downloader_;

  
  ErrorInfo error_info_;

  
  
  bool error_already_reported_;

  
  int64_t pnacl_init_time_;
  int64_t pexe_size_;  
  int64_t pexe_bytes_compiled_;  
  int64_t expected_pexe_size_;   

  
  
  
  nacl::scoped_ptr<PnaclTranslateThread> translate_thread_;
};


}  
#endif  

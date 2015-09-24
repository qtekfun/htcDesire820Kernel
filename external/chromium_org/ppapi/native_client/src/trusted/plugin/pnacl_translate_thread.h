// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PNACL_TRANSLATE_THREAD_H_
#define NATIVE_CLIENT_SRC_TRUSTED_PLUGIN_PNACL_TRANSLATE_THREAD_H_

#include <deque>
#include <vector>

#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/nacl_scoped_ptr.h"
#include "native_client/src/include/nacl_string.h"
#include "native_client/src/shared/platform/nacl_threads.h"
#include "native_client/src/shared/platform/nacl_sync_checked.h"

#include "ppapi/cpp/completion_callback.h"

#include "ppapi/native_client/src/trusted/plugin/plugin_error.h"
#include "ppapi/native_client/src/trusted/plugin/service_runtime.h"

namespace nacl {
class DescWrapper;
}


namespace plugin {

class Manifest;
class NaClSubprocess;
class Plugin;
class PnaclCoordinator;
class PnaclOptions;
class PnaclResources;
class TempFile;

struct PnaclTimeStats {
  int64_t pnacl_llc_load_time;
  int64_t pnacl_compile_time;
  int64_t pnacl_ld_load_time;
  int64_t pnacl_link_time;
};

class PnaclTranslateThread {
 public:
  PnaclTranslateThread();
  ~PnaclTranslateThread();

  
  
  void RunTranslate(const pp::CompletionCallback& finish_callback,
                    const Manifest* manifest,
                    TempFile* obj_file,
                    TempFile* nexe_file,
                    ErrorInfo* error_info,
                    PnaclResources* resources,
                    PnaclOptions* pnacl_options,
                    PnaclCoordinator* coordinator,
                    Plugin* plugin);

  
  
  
  
  
  
  void AbortSubprocesses();

  
  void PutBytes(std::vector<char>* data, int count);

  const PnaclTimeStats& GetTimeStats() const { return time_stats_; }

 private:
  
  NaClSubprocess* StartSubprocess(const nacl::string& url,
                                  const Manifest* manifest,
                                  ErrorInfo* error_info);
  
  
  static void WINAPI DoTranslateThread(void* arg);
  
  void DoTranslate() ;
  
  void TranslateFailed(enum PluginErrorCode err_code,
                       const nacl::string& error_string);
  
  bool RunLdSubprocess(int is_shared_library,
                       const nacl::string& soname,
                       const nacl::string& lib_dependencies);


  
  pp::CompletionCallback report_translate_finished_;

  nacl::scoped_ptr<NaClThread> translate_thread_;

  
  struct NaClMutex subprocess_mu_;
  nacl::scoped_ptr<NaClSubprocess> llc_subprocess_;
  nacl::scoped_ptr<NaClSubprocess> ld_subprocess_;
  
  bool llc_subprocess_active_;
  bool ld_subprocess_active_;

  
  
  
  
  struct NaClCondVar buffer_cond_;
  
  struct NaClMutex cond_mu_;
  
  
  std::deque<std::vector<char> > data_buffers_;
  
  
  bool done_;

  PnaclTimeStats time_stats_;

  
  const Manifest* manifest_;
  TempFile* obj_file_;
  TempFile* nexe_file_;
  ErrorInfo* coordinator_error_info_;
  PnaclResources* resources_;
  PnaclOptions* pnacl_options_;
  PnaclCoordinator* coordinator_;
  Plugin* plugin_;
 private:
  NACL_DISALLOW_COPY_AND_ASSIGN(PnaclTranslateThread);
};

}
#endif 

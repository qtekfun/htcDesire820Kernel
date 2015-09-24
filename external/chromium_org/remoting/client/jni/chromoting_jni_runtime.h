// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_JNI_CHROMOTING_JNI_RUNTIME_H_
#define REMOTING_CLIENT_JNI_CHROMOTING_JNI_RUNTIME_H_

#include <jni.h>
#include <string>

#include "base/android/scoped_java_ref.h"
#include "base/at_exit.h"
#include "net/url_request/url_request_context_getter.h"
#include "remoting/base/auto_thread.h"
#include "remoting/client/jni/chromoting_jni_instance.h"
#include "remoting/protocol/connection_to_host.h"

template<typename T> struct DefaultSingletonTraits;

namespace remoting {

bool RegisterJni(JNIEnv* env);

class ChromotingJniRuntime {
 public:
  
  
  static ChromotingJniRuntime* GetInstance();

  scoped_refptr<AutoThreadTaskRunner> ui_task_runner() {
    return ui_task_runner_;
  }

  scoped_refptr<AutoThreadTaskRunner> network_task_runner() {
    return network_task_runner_;
  }

  scoped_refptr<AutoThreadTaskRunner> display_task_runner() {
    return display_task_runner_;
  }

  scoped_refptr<net::URLRequestContextGetter> url_requester() {
    return url_requester_;
  }

  
  
  
  
  void ConnectToHost(const char* username,
                     const char* auth_token,
                     const char* host_jid,
                     const char* host_id,
                     const char* host_pubkey,
                     const char* pairing_id,
                     const char* pairing_secret);

  
  
  void DisconnectFromHost();

  
  
  scoped_refptr<ChromotingJniInstance> session() {
    DCHECK(session_);
    return session_;
  }

  
  void ReportConnectionStatus(protocol::ConnectionToHost::State state,
                              protocol::ErrorCode error);

  
  void DisplayAuthenticationPrompt(bool pairing_supported);

  
  void CommitPairingCredentials(const std::string& host,
                                const std::string& id,
                                const std::string& secret);

  
  base::android::ScopedJavaLocalRef<jobject> NewBitmap(
      webrtc::DesktopSize size);

  
  
  void UpdateFrameBitmap(jobject bitmap);

  
  void UpdateCursorShape(const protocol::CursorShapeInfo& cursor_shape);

  
  void RedrawCanvas();

 private:
  ChromotingJniRuntime();

  
  
  
  
  
  virtual ~ChromotingJniRuntime();

  
  void DetachFromVmAndSignal(base::WaitableEvent* waiter);

  
  
  scoped_ptr<base::AtExitManager> at_exit_manager_;

  
  scoped_ptr<base::MessageLoopForUI> ui_loop_;

  
  scoped_refptr<AutoThreadTaskRunner> ui_task_runner_;
  scoped_refptr<AutoThreadTaskRunner> network_task_runner_;
  scoped_refptr<AutoThreadTaskRunner> display_task_runner_;

  scoped_refptr<net::URLRequestContextGetter> url_requester_;

  
  scoped_refptr<ChromotingJniInstance> session_;

  friend struct DefaultSingletonTraits<ChromotingJniRuntime>;

  DISALLOW_COPY_AND_ASSIGN(ChromotingJniRuntime);
};

}  

#endif

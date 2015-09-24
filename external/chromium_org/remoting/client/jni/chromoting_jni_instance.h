// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_CLIENT_CHROMOTING_JNI_INSTANCE_H_
#define REMOTING_CLIENT_CHROMOTING_JNI_INSTANCE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "remoting/client/chromoting_client.h"
#include "remoting/client/client_config.h"
#include "remoting/client/client_context.h"
#include "remoting/client/client_user_interface.h"
#include "remoting/client/frame_consumer_proxy.h"
#include "remoting/client/jni/jni_frame_consumer.h"
#include "remoting/jingle_glue/xmpp_signal_strategy.h"
#include "remoting/protocol/clipboard_stub.h"
#include "remoting/protocol/connection_to_host.h"
#include "remoting/protocol/cursor_shape_stub.h"

namespace remoting {

namespace protocol {
class ClipboardEvent;
class CursorShapeInfo;
}  

class ChromotingJniInstance
  : public ClientUserInterface,
    public protocol::ClipboardStub,
    public protocol::CursorShapeStub,
    public base::RefCountedThreadSafe<ChromotingJniInstance> {
 public:
  
  
  
  ChromotingJniInstance(ChromotingJniRuntime* jni_runtime,
                        const char* username,
                        const char* auth_token,
                        const char* host_jid,
                        const char* host_id,
                        const char* host_pubkey,
                        const char* pairing_id,
                        const char* pairing_secret);

  
  
  void Cleanup();

  
  
  
  void ProvideSecret(const std::string& pin, bool create_pair);

  
  void RedrawDesktop();

  
  
  void PerformMouseAction(int x, int y,
                          protocol::MouseEvent_MouseButton button,
                          bool button_down);

  void PerformMouseWheelDeltaAction(int delta_x, int delta_y);

  
  void PerformKeyboardAction(int key_code, bool key_down);

  
  
  void RecordPaintTime(int64 paint_time_ms);

  
  virtual void OnConnectionState(
      protocol::ConnectionToHost::State state,
      protocol::ErrorCode error) OVERRIDE;
  virtual void OnConnectionReady(bool ready) OVERRIDE;
  virtual void SetCapabilities(const std::string& capabilities) OVERRIDE;
  virtual void SetPairingResponse(
      const protocol::PairingResponse& response) OVERRIDE;
  virtual void DeliverHostMessage(
      const protocol::ExtensionMessage& message) OVERRIDE;
  virtual protocol::ClipboardStub* GetClipboardStub() OVERRIDE;
  virtual protocol::CursorShapeStub* GetCursorShapeStub() OVERRIDE;
  virtual scoped_ptr<protocol::ThirdPartyClientAuthenticator::TokenFetcher>
      GetTokenFetcher(const std::string& host_public_key) OVERRIDE;

  
  virtual void InjectClipboardEvent(
      const protocol::ClipboardEvent& event) OVERRIDE;

  
  virtual void SetCursorShape(const protocol::CursorShapeInfo& shape) OVERRIDE;

 private:
  
  virtual ~ChromotingJniInstance();

  void ConnectToHostOnDisplayThread();
  void ConnectToHostOnNetworkThread();
  void DisconnectFromHostOnNetworkThread();

  
  
  
  void FetchSecret(bool pairable,
                   const protocol::SecretFetchedCallback& callback);

  
  
  void EnableStatsLogging(bool enabled);

  
  
  
  void LogPerfStats();

  
  ChromotingJniRuntime* jni_runtime_;

  
  std::string host_id_;

  
  scoped_refptr<FrameConsumerProxy> frame_consumer_;
  scoped_ptr<JniFrameConsumer> view_;
  scoped_ptr<base::WeakPtrFactory<JniFrameConsumer> > view_weak_factory_;

  
  ClientConfig client_config_;
  scoped_ptr<ClientContext> client_context_;
  scoped_ptr<protocol::ConnectionToHost> connection_;
  scoped_ptr<ChromotingClient> client_;
  XmppSignalStrategy::XmppServerConfig xmpp_config_;
  scoped_ptr<XmppSignalStrategy> signaling_;  

  
  
  protocol::SecretFetchedCallback pin_callback_;

  
  
  
  
  bool create_pairing_;

  // If this is true, performance statistics will be periodically written to
  
  bool stats_logging_enabled_;

  friend class base::RefCountedThreadSafe<ChromotingJniInstance>;

  DISALLOW_COPY_AND_ASSIGN(ChromotingJniInstance);
};

}  

#endif

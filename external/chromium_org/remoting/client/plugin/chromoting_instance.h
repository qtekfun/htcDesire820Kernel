// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_CLIENT_PLUGIN_CHROMOTING_INSTANCE_H_
#define REMOTING_CLIENT_PLUGIN_CHROMOTING_INSTANCE_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/var.h"
#include "remoting/client/client_context.h"
#include "remoting/client/client_user_interface.h"
#include "remoting/client/key_event_mapper.h"
#include "remoting/client/plugin/normalizing_input_filter.h"
#include "remoting/client/plugin/pepper_input_handler.h"
#include "remoting/client/plugin/pepper_plugin_thread_delegate.h"
#include "remoting/proto/event.pb.h"
#include "remoting/protocol/client_stub.h"
#include "remoting/protocol/clipboard_stub.h"
#include "remoting/protocol/connection_to_host.h"
#include "remoting/protocol/cursor_shape_stub.h"
#include "remoting/protocol/input_event_tracker.h"
#include "remoting/protocol/mouse_input_filter.h"
#include "remoting/protocol/negotiating_client_authenticator.h"
#include "remoting/protocol/third_party_client_authenticator.h"

namespace base {
class DictionaryValue;
}  

namespace pp {
class InputEvent;
class Module;
}  

namespace webrtc {
class DesktopRegion;
class DesktopSize;
class DesktopVector;
}  

namespace remoting {

class ChromotingClient;
class ChromotingStats;
class ClientContext;
class DelegatingSignalStrategy;
class FrameConsumer;
class FrameConsumerProxy;
class PepperAudioPlayer;
class PepperTokenFetcher;
class PepperView;
class RectangleUpdateDecoder;
class SignalStrategy;

struct ClientConfig;

class ChromotingInstance :
      public ClientUserInterface,
      public protocol::ClipboardStub,
      public protocol::CursorShapeStub,
      public pp::Instance {
 public:
  
  
  static const int kApiVersion = 7;

  
  
  static const char kApiFeatures[];

  
  
  static const char kRequestedCapabilities[];

  
  
  static const char kSupportedCapabilities[];

  
  
  
  static const int kApiMinMessagingVersion = 5;

  
  
  
  static const int kApiMinScriptableVersion = 5;

  
  static bool ParseAuthMethods(const std::string& auth_methods,
                               ClientConfig* config);

  explicit ChromotingInstance(PP_Instance instance);
  virtual ~ChromotingInstance();

  
  virtual void DidChangeFocus(bool has_focus) OVERRIDE;
  virtual void DidChangeView(const pp::View& view) OVERRIDE;
  virtual bool Init(uint32_t argc, const char* argn[],
                    const char* argv[]) OVERRIDE;
  virtual void HandleMessage(const pp::Var& message) OVERRIDE;
  virtual bool HandleInputEvent(const pp::InputEvent& event) OVERRIDE;

  
  virtual void OnConnectionState(protocol::ConnectionToHost::State state,
                                 protocol::ErrorCode error) OVERRIDE;
  virtual void OnConnectionReady(bool ready) OVERRIDE;
  virtual void SetCapabilities(const std::string& capabilities) OVERRIDE;
  virtual void SetPairingResponse(
      const protocol::PairingResponse& pairing_response) OVERRIDE;
  virtual void DeliverHostMessage(
      const protocol::ExtensionMessage& message) OVERRIDE;
  virtual protocol::ClipboardStub* GetClipboardStub() OVERRIDE;
  virtual protocol::CursorShapeStub* GetCursorShapeStub() OVERRIDE;
  virtual scoped_ptr<protocol::ThirdPartyClientAuthenticator::TokenFetcher>
  GetTokenFetcher(const std::string& host_public_key) OVERRIDE;

  
  virtual void InjectClipboardEvent(
      const protocol::ClipboardEvent& event) OVERRIDE;

  
  virtual void SetCursorShape(
      const protocol::CursorShapeInfo& cursor_shape) OVERRIDE;

  
  void SetDesktopSize(const webrtc::DesktopSize& size,
                      const webrtc::DesktopVector& dpi);
  void SetDesktopShape(const webrtc::DesktopRegion& shape);
  void OnFirstFrameReceived();

  
  
  ChromotingStats* GetStats();

  
  
  
  
  
  static void RegisterLogMessageHandler();

  
  
  void RegisterLoggingInstance();

  
  
  
  void UnregisterLoggingInstance();

  
  
  
  static bool LogToUI(int severity, const char* file, int line,
                      size_t message_start, const std::string& str);

  
  void FetchThirdPartyToken(
      const GURL& token_url,
      const std::string& host_public_key,
      const std::string& scope,
      const base::WeakPtr<PepperTokenFetcher> pepper_token_fetcher);

 private:
  FRIEND_TEST_ALL_PREFIXES(ChromotingInstanceTest, TestCaseSetup);

  
  
  static void FetchSecretFromString(
      const std::string& shared_secret,
      bool pairing_supported,
      const protocol::SecretFetchedCallback& secret_fetched_callback);

  
  
  void HandleConnect(const base::DictionaryValue& data);
  void HandleDisconnect(const base::DictionaryValue& data);
  void HandleOnIncomingIq(const base::DictionaryValue& data);
  void HandleReleaseAllKeys(const base::DictionaryValue& data);
  void HandleInjectKeyEvent(const base::DictionaryValue& data);
  void HandleRemapKey(const base::DictionaryValue& data);
  void HandleTrapKey(const base::DictionaryValue& data);
  void HandleSendClipboardItem(const base::DictionaryValue& data);
  void HandleNotifyClientResolution(const base::DictionaryValue& data);
  void HandlePauseVideo(const base::DictionaryValue& data);
  void HandlePauseAudio(const base::DictionaryValue& data);
  void HandleOnPinFetched(const base::DictionaryValue& data);
  void HandleOnThirdPartyTokenFetched(const base::DictionaryValue& data);
  void HandleRequestPairing(const base::DictionaryValue& data);
  void HandleExtensionMessage(const base::DictionaryValue& data);
  void HandleAllowMouseLockMessage();

  
  void ConnectWithConfig(const ClientConfig& config,
                         const std::string& local_jid);

  
  void PostChromotingMessage(const std::string& method,
                             scoped_ptr<base::DictionaryValue> data);

  
  void SendTrappedKey(uint32 usb_keycode, bool pressed);

  
  void SendOutgoingIq(const std::string& iq);

  void SendPerfStats();

  void ProcessLogToUI(const std::string& message);

  
  bool IsCallerAppOrExtension();

  
  bool IsConnected();

  
  
  void FetchSecretFromDialog(
      bool pairing_supported,
      const protocol::SecretFetchedCallback& secret_fetched_callback);

  bool initialized_;

  PepperPluginThreadDelegate plugin_thread_delegate_;
  scoped_refptr<PluginThreadTaskRunner> plugin_task_runner_;
  ClientContext context_;
  scoped_refptr<RectangleUpdateDecoder> rectangle_decoder_;
  scoped_ptr<PepperView> view_;
  scoped_ptr<base::WeakPtrFactory<FrameConsumer> > view_weak_factory_;
  pp::View plugin_view_;

  
  scoped_ptr<webrtc::DesktopRegion> desktop_shape_;

  scoped_ptr<DelegatingSignalStrategy> signal_strategy_;

  scoped_ptr<protocol::ConnectionToHost> host_connection_;
  scoped_ptr<ChromotingClient> client_;

  
  protocol::MouseInputFilter mouse_input_filter_;
  protocol::InputEventTracker input_tracker_;
  KeyEventMapper key_mapper_;
  scoped_ptr<protocol::InputFilter> normalizing_input_filter_;
  PepperInputHandler input_handler_;

  
  bool use_async_pin_dialog_;
  protocol::SecretFetchedCallback secret_fetched_callback_;

  base::WeakPtr<PepperTokenFetcher> pepper_token_fetcher_;

  
  base::WeakPtrFactory<ChromotingInstance> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ChromotingInstance);
};

}  

#endif  

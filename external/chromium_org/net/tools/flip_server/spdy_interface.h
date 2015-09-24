// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_FLIP_SERVER_SPDY_INTERFACE_H_
#define NET_TOOLS_FLIP_SERVER_SPDY_INTERFACE_H_

#include <map>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "net/spdy/buffered_spdy_framer.h"
#include "net/spdy/spdy_protocol.h"
#include "net/tools/balsa/balsa_headers.h"
#include "net/tools/balsa/balsa_visitor_interface.h"
#include "net/tools/flip_server/output_ordering.h"
#include "net/tools/flip_server/sm_connection.h"
#include "net/tools/flip_server/sm_interface.h"

namespace net {

class FlipAcceptor;
class MemoryCache;

class SpdySM : public BufferedSpdyFramerVisitorInterface, public SMInterface {
 public:
  SpdySM(SMConnection* connection,
         SMInterface* sm_http_interface,
         EpollServer* epoll_server,
         MemoryCache* memory_cache,
         FlipAcceptor* acceptor,
         SpdyMajorVersion spdy_version);
  virtual ~SpdySM();

  virtual void InitSMInterface(SMInterface* sm_http_interface,
                               int32 server_idx) OVERRIDE {}

  virtual void InitSMConnection(SMConnectionPoolInterface* connection_pool,
                                SMInterface* sm_interface,
                                EpollServer* epoll_server,
                                int fd,
                                std::string server_ip,
                                std::string server_port,
                                std::string remote_ip,
                                bool use_ssl) OVERRIDE;

 private:
  virtual void set_is_request() OVERRIDE {}
  SMInterface* NewConnectionInterface();
  
  virtual SMInterface* FindOrMakeNewSMConnectionInterface(
      const std::string& server_ip,
      const std::string& server_port);
  int SpdyHandleNewStream(SpdyStreamId stream_id,
                          SpdyPriority priority,
                          const SpdyHeaderBlock& headers,
                          std::string& http_data,
                          bool* is_https_scheme);

  
  virtual void OnError(SpdyFramer::SpdyError error_code) OVERRIDE {}
  virtual void OnStreamError(SpdyStreamId stream_id,
                             const std::string& description) OVERRIDE {}
  
  virtual void OnSynStream(SpdyStreamId stream_id,
                           SpdyStreamId associated_stream_id,
                           SpdyPriority priority,
                           uint8 credential_slot,
                           bool fin,
                           bool unidirectional,
                           const SpdyHeaderBlock& headers) OVERRIDE;

  
  virtual void OnSynReply(SpdyStreamId stream_id,
                          bool fin,
                          const SpdyHeaderBlock& headers) OVERRIDE;

  
  virtual void OnHeaders(SpdyStreamId stream_id,
                         bool fin,
                         const SpdyHeaderBlock& headers) OVERRIDE;

  
  virtual void OnDataFrameHeader(SpdyStreamId stream_id,
                                 size_t length,
                                 bool fin) OVERRIDE {}

  
  
  
  
  
  
  virtual void OnStreamFrameData(SpdyStreamId stream_id,
                                 const char* data,
                                 size_t len,
                                 bool fin) OVERRIDE;

  
  
  virtual void OnSettings(bool clear_persisted) OVERRIDE {}

  
  
  virtual void OnSetting(SpdySettingsIds id,
                         uint8 flags,
                         uint32 value) OVERRIDE {}

  
  virtual void OnPing(uint32 unique_id) OVERRIDE {}

  
  virtual void OnRstStream(SpdyStreamId stream_id,
                           SpdyRstStreamStatus status) OVERRIDE;

  
  virtual void OnGoAway(SpdyStreamId last_accepted_stream_id,
                        SpdyGoAwayStatus status) OVERRIDE {}

  
  virtual void OnWindowUpdate(SpdyStreamId stream_id,
                              uint32 delta_window_size) OVERRIDE {}

  
  virtual void OnPushPromise(SpdyStreamId stream_id,
                             SpdyStreamId promised_stream_id) OVERRIDE {}

 public:
  virtual size_t ProcessReadInput(const char* data, size_t len) OVERRIDE;
  virtual size_t ProcessWriteInput(const char* data, size_t len) OVERRIDE;
  virtual bool MessageFullyRead() const OVERRIDE;
  virtual void SetStreamID(uint32 stream_id) OVERRIDE {}
  virtual bool Error() const OVERRIDE;
  virtual const char* ErrorAsString() const OVERRIDE;
  virtual void Reset() OVERRIDE {}
  virtual void ResetForNewInterface(int32 server_idx) OVERRIDE;
  virtual void ResetForNewConnection() OVERRIDE;
  
  
  
  
  
  virtual void Cleanup() OVERRIDE {}
  
  virtual int PostAcceptHook() OVERRIDE;
  virtual void NewStream(uint32 stream_id,
                         uint32 priority,
                         const std::string& filename) OVERRIDE;
  void AddToOutputOrder(const MemCacheIter& mci);
  virtual void SendEOF(uint32 stream_id) OVERRIDE;
  virtual void SendErrorNotFound(uint32 stream_id) OVERRIDE;
  virtual size_t SendSynStream(uint32 stream_id,
                               const BalsaHeaders& headers) OVERRIDE;
  virtual size_t SendSynReply(uint32 stream_id,
                              const BalsaHeaders& headers) OVERRIDE;
  virtual void SendDataFrame(uint32 stream_id,
                             const char* data,
                             int64 len,
                             uint32 flags,
                             bool compress) OVERRIDE;
  BufferedSpdyFramer* spdy_framer() { return buffered_spdy_framer_; }

  const OutputOrdering& output_ordering() const {
    return client_output_ordering_;
  }

  static std::string forward_ip_header() { return forward_ip_header_; }
  static void set_forward_ip_header(const std::string& value) {
    forward_ip_header_ = value;
  }
  SpdyMajorVersion spdy_version() const {
    return buffered_spdy_framer_->protocol_version();
  }

 private:
  void SendEOFImpl(uint32 stream_id);
  void SendErrorNotFoundImpl(uint32 stream_id);
  void KillStream(uint32 stream_id);
  void CopyHeaders(SpdyHeaderBlock& dest, const BalsaHeaders& headers);
  size_t SendSynStreamImpl(uint32 stream_id, const BalsaHeaders& headers);
  size_t SendSynReplyImpl(uint32 stream_id, const BalsaHeaders& headers);
  void SendDataFrameImpl(uint32 stream_id,
                         const char* data,
                         int64 len,
                         SpdyDataFlags flags,
                         bool compress);
  void EnqueueDataFrame(DataFrame* df);
  virtual void GetOutput() OVERRIDE;

 private:
  BufferedSpdyFramer* buffered_spdy_framer_;
  bool valid_spdy_session_;  
                             
                             

  SMConnection* connection_;
  OutputList* client_output_list_;
  OutputOrdering client_output_ordering_;
  uint32 next_outgoing_stream_id_;
  EpollServer* epoll_server_;
  FlipAcceptor* acceptor_;
  MemoryCache* memory_cache_;
  std::vector<SMInterface*> server_interface_list;
  std::vector<int32> unused_server_interface_list;
  typedef std::map<uint32, SMInterface*> StreamToSmif;
  StreamToSmif stream_to_smif_;
  bool close_on_error_;

  static std::string forward_ip_header_;
};

}  

#endif  

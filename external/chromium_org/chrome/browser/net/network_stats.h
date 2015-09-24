// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_NETWORK_STATS_H_
#define CHROME_BROWSER_NET_NETWORK_STATS_H_

#include <bitset>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/metrics/histogram.h"
#include "base/time/time.h"
#include "chrome/browser/io_thread.h"
#include "chrome/browser/net/probe_message.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/io_buffer.h"
#include "net/proxy/proxy_info.h"

namespace net {
class ClientSocketFactory;
class DatagramClientSocket;
class HostResolver;
class SingleRequestHostResolver;
}

namespace chrome_browser_net {


class NetworkStats {
 public:
  enum Status {            
    SUCCESS,               
    SOCKET_CREATE_FAILED,  
    RESOLVE_FAILED,        
    CONNECT_FAILED,        
    WRITE_FAILED,          
    READ_TIMED_OUT,        
    READ_FAILED,           
    STATUS_MAX,            
  };

  enum ReadState {         
    READ_STATE_IDLE,
    READ_STATE_READ_PENDING,
  };

  enum WriteState {        
    WRITE_STATE_IDLE,
    WRITE_STATE_WRITE_PENDING,
  };

  
  
  enum TestType {
    
    
    
    TOKEN_REQUEST,
    START_PACKET_TEST,      
    NON_PACED_PACKET_TEST,  
    PACED_PACKET_TEST,      
    
    NAT_BIND_TEST,
    PACKET_SIZE_TEST,
    TEST_TYPE_MAX,
  };

  
  explicit NetworkStats(net::ClientSocketFactory* socket_factory);
  
  ~NetworkStats();

  
  
  
  
  
  
  bool Start(net::HostResolver* host_resolver,
             const net::HostPortPair& server,
             uint16 histogram_port,
             bool has_proxy_server,
             uint32 probe_bytes,
             uint32 bytes_for_packet_size_test,
             const net::CompletionCallback& callback);

 private:
  friend class NetworkStatsTest;

  
  
  void StartOneTest();

  
  void ResetData();

  
  void OnResolveComplete(int result);

  
  
  
  bool DoConnect(int result);

  
  
  
  bool ConnectComplete(int result);

  
  
  
  void SendHelloRequest();

  
  
  void SendProbeRequest();

  
  
  
  
  bool ReadComplete(int result);

  
  void OnReadComplete(int result);
  void OnWriteComplete(int result);

  
  
  
  int ReadData();

  
  
  
  int SendData(const std::string& str);

  
  
  void UpdateSendBuffer(int bytes_sent);

  
  
  
  void StartReadDataTimer(uint32 milliseconds, uint32 test_index);

  
  
  
  
  void OnReadDataTimeout(uint32 test_index);

  
  
  
  
  void TestPhaseComplete(Status status, int result);

  
  
  void DoFinishCallback(int result);

  
  
  
  bool UpdateReception(const ProbePacket& probe_packet);

  
  
  
  
  void RecordHistograms(TestType test_type);

  
  
  
  
  
  void RecordPacketsReceivedHistograms(TestType test_type);

  
  
  
  
  
  void RecordPacketLossSeriesHistograms(TestType test_type);

  
  
  void RecordInterArrivalHistograms(TestType test_type);

  
  
  void RecordRTTHistograms(TestType test_type, uint32 index);

  
  
  void RecordNATTestReceivedHistograms(Status status);

  
  
  void RecordPacketSizeTestReceivedHistograms(Status status);

  
  
  
  void RecordSendToLastRecvDelayHistograms(TestType test_type);

  
  
  TestType GetNextTest();

  
  
  static uint32 maximum_tests_;
  
  static uint32 maximum_sequential_packets_;
  
  static uint32 maximum_NAT_packets_;
  
  static uint32 maximum_NAT_idle_seconds_;
  
  
  static bool start_test_after_connect_;

  
  scoped_ptr<net::DatagramClientSocket> socket_;

  net::ClientSocketFactory* socket_factory_;

  
  scoped_refptr<net::IOBuffer> read_buffer_;

  
  scoped_refptr<net::DrainableIOBuffer> write_buffer_;

  
  uint16 histogram_port_;

  
  bool has_proxy_server_;

  
  scoped_ptr<net::SingleRequestHostResolver> resolver_;

  
  net::AddressList addresses_;

  
  
  net::CompletionCallback finished_callback_;

  
  std::vector<base::TimeDelta> packet_rtt_;

  
  base::TimeTicks probe_request_time_;

  
  
  uint32 probe_packet_bytes_;

  
  uint32 bytes_for_packet_size_test_;

  
  std::bitset<21> packets_received_mask_;

  
  base::TimeTicks first_arrival_time_;
  
  base::TimeTicks last_arrival_time_;
  
  
  base::TimeDelta inter_arrival_time_;
  
  
  
  
  
  base::TimeDelta pacing_interval_;
  
  std::vector<TestType> test_sequence_;
  uint32 current_test_index_;  

  ProbeMessage probe_message_;

  
  ProbePacket_Token token_;

  
  ReadState read_state_;
  WriteState write_state_;

  
  base::WeakPtrFactory<NetworkStats> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(NetworkStats);
};

class ProxyDetector {
 public:
  
  typedef base::Callback<void(bool)> OnResolvedCallback;

  
  
  
  
  ProxyDetector(net::ProxyService* proxy_service,
                const net::HostPortPair& server_address,
                OnResolvedCallback callback);

  
  
  void StartResolveProxy();

 private:
  
  ~ProxyDetector();

  
  void OnResolveProxyComplete(int result);

  
  
  net::ProxyService* proxy_service_;

  
  
  net::HostPortPair server_address_;

  
  OnResolvedCallback callback_;

  
  net::ProxyInfo proxy_info_;

  
  
  bool has_pending_proxy_resolution_;
  DISALLOW_COPY_AND_ASSIGN(ProxyDetector);
};

void CollectNetworkStats(const std::string& network_stats_server_url,
                         IOThread* io_thread);

void StartNetworkStatsTest(net::HostResolver* host_resolver,
                           const net::HostPortPair& server_address,
                           uint16 histogram_port,
                           bool has_proxy_server);

}  

#endif  

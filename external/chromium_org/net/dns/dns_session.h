// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_SESSION_H_
#define NET_DNS_DNS_SESSION_H_

#include <vector>

#include "base/lazy_instance.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/metrics/bucket_ranges.h"
#include "base/time/time.h"
#include "net/base/net_export.h"
#include "net/base/rand_callback.h"
#include "net/dns/dns_config_service.h"
#include "net/dns/dns_socket_pool.h"

namespace base {
class BucketRanges;
class SampleVector;
}

namespace net {

class ClientSocketFactory;
class DatagramClientSocket;
class NetLog;
class StreamSocket;

class NET_EXPORT_PRIVATE DnsSession
    : NON_EXPORTED_BASE(public base::RefCounted<DnsSession>) {
 public:
  typedef base::Callback<int()> RandCallback;

  class NET_EXPORT_PRIVATE SocketLease {
   public:
    SocketLease(scoped_refptr<DnsSession> session,
                unsigned server_index,
                scoped_ptr<DatagramClientSocket> socket);
    ~SocketLease();

    unsigned server_index() const { return server_index_; }

    DatagramClientSocket* socket() { return socket_.get(); }

   private:
    scoped_refptr<DnsSession> session_;
    unsigned server_index_;
    scoped_ptr<DatagramClientSocket> socket_;

    DISALLOW_COPY_AND_ASSIGN(SocketLease);
  };

  DnsSession(const DnsConfig& config,
             scoped_ptr<DnsSocketPool> socket_pool,
             const RandIntCallback& rand_int_callback,
             NetLog* net_log);

  const DnsConfig& config() const { return config_; }
  NetLog* net_log() const { return net_log_; }

  
  int NextQueryId() const;

  
  unsigned NextFirstServerIndex();

  
  
  
  
  unsigned NextGoodServerIndex(unsigned server_index);

  
  void RecordServerFailure(unsigned server_index);

  
  void RecordServerSuccess(unsigned server_index);

  
  void RecordRTT(unsigned server_index, base::TimeDelta rtt);

  
  void RecordLostPacket(unsigned server_index, int attempt);

  
  void RecordServerStats();

  
  
  base::TimeDelta NextTimeout(unsigned server_index, int attempt);

  
  
  scoped_ptr<SocketLease> AllocateSocket(unsigned server_index,
                                         const NetLog::Source& source);

  
  
  scoped_ptr<StreamSocket> CreateTCPSocket(unsigned server_index,
                                           const NetLog::Source& source);

 private:
  friend class base::RefCounted<DnsSession>;
  ~DnsSession();

  
  void FreeSocket(unsigned server_index,
                  scoped_ptr<DatagramClientSocket> socket);

  
  base::TimeDelta NextTimeoutFromJacobson(unsigned server_index, int attempt);

  
  base::TimeDelta NextTimeoutFromHistogram(unsigned server_index, int attempt);

  const DnsConfig config_;
  scoped_ptr<DnsSocketPool> socket_pool_;
  RandCallback rand_callback_;
  NetLog* net_log_;

  
  int server_index_;

  struct ServerStats;

  
  ScopedVector<ServerStats> server_stats_;

  
  struct RttBuckets : public base::BucketRanges {
    RttBuckets();
  };
  static base::LazyInstance<RttBuckets>::Leaky rtt_buckets_;

  DISALLOW_COPY_AND_ASSIGN(DnsSession);
};

}  

#endif  

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DIAL_DIAL_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_API_DIAL_DIAL_SERVICE_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/threading/thread_checker.h"
#include "base/timer/timer.h"
#include "net/base/net_log.h"
#include "net/udp/udp_socket.h"

namespace net {
class IPEndPoint;
class IPAddress;
class IOBuffer;
class StringIOBuffer;
struct NetworkInterface;
}

namespace extensions {

class DialDeviceData;

class DialService {
 public:
  enum DialServiceErrorCode {
    DIAL_SERVICE_NO_INTERFACES = 0,
    DIAL_SERVICE_SOCKET_ERROR
  };

  class Observer {
   public:
    
    virtual void OnDiscoveryRequest(DialService* service) = 0;

    
    virtual void OnDeviceDiscovered(DialService* service,
                                    const DialDeviceData& device) = 0;

    
    virtual void OnDiscoveryFinished(DialService* service) = 0;

    
    virtual void OnError(DialService* service,
                         const DialServiceErrorCode& code) = 0;

   protected:
    virtual ~Observer() {}
  };

  virtual ~DialService() {}

  
  
  virtual bool Discover() = 0;

  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  virtual bool HasObserver(Observer* observer) = 0;
};

class DialServiceImpl : public DialService,
                        public base::SupportsWeakPtr<DialServiceImpl> {
 public:
  explicit DialServiceImpl(net::NetLog* net_log);
  virtual ~DialServiceImpl();

  
  virtual bool Discover() OVERRIDE;
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual bool HasObserver(Observer* observer) OVERRIDE;

 private:
  
  class DialSocket {
   public:
    
    
    
    DialSocket(
        const base::Closure& discovery_request_cb,
        const base::Callback<void(const DialDeviceData&)>& device_discovered_cb,
        const base::Closure& on_error_cb);
    ~DialSocket();

    
    
    bool CreateAndBindSocket(const net::IPAddressNumber& bind_ip_address,
                             net::NetLog* net_log,
                             net::NetLog::Source net_log_source);

    
    
    void SendOneRequest(const net::IPEndPoint& send_address,
                        const scoped_refptr<net::StringIOBuffer>& send_buffer);

    
    bool IsClosed();

   private:
    
    
    
    
    
    bool CheckResult(const char* operation, int result);

    
    void Close();

    
    void OnSocketWrite(int buffer_size, int result);

    
    
    bool ReadSocket();

    
    void OnSocketRead(int result);

    
    void HandleResponse(int bytes_read);

    
    
    
    static bool ParseResponse(const std::string& response,
                              const base::Time& response_time,
                              DialDeviceData* device);

    
    scoped_ptr<net::UDPSocket> socket_;

    
    scoped_refptr<net::IOBufferWithSize> recv_buffer_;

    
    net::IPEndPoint recv_address_;

    
    base::ThreadChecker thread_checker_;

    
    base::Closure discovery_request_cb_;

    
    base::Callback<void(const DialDeviceData&)> device_discovered_cb_;

    
    base::Closure on_error_cb_;

    
    bool is_writing_;

    
    bool is_reading_;

    FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestNotifyOnError);
    FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestOnDeviceDiscovered);
    FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestOnDiscoveryRequest);
    FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestResponseParsing);
    DISALLOW_COPY_AND_ASSIGN(DialSocket);
  };

  
  void StartDiscovery();

  
  
  void SendNetworkList(const net::NetworkInterfaceList& list);

  
  
  
  
  void DiscoverOnAddresses(
      const std::vector<net::IPAddressNumber>& ip_addresses);

  
  
  void BindAndAddSocket(const net::IPAddressNumber& bind_ip_address);

  
  scoped_ptr<DialSocket> CreateDialSocket();

  
  void SendOneRequest();

  
  void NotifyOnDiscoveryRequest();

  
  void NotifyOnDeviceDiscovered(const DialDeviceData& device_data);

  
  void NotifyOnError();

  
  
  void FinishDiscovery();

  
  bool HasOpenSockets();

  
  
  ScopedVector<DialSocket> dial_sockets_;

  
  net::NetLog* net_log_;

  
  net::NetLog::Source net_log_source_;

  
  net::IPEndPoint send_address_;

  
  scoped_refptr<net::StringIOBuffer> send_buffer_;

  
  bool discovery_active_;

  
  int num_requests_sent_;

  
  int max_requests_;

  
  base::OneShotTimer<DialServiceImpl> finish_timer_;

  
  
  base::TimeDelta finish_delay_;

  
  base::RepeatingTimer<DialServiceImpl> request_timer_;

  
  
  base::TimeDelta request_interval_;

  
  ObserverList<Observer> observer_list_;

  
  base::ThreadChecker thread_checker_;

  friend class DialServiceTest;
  FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestSendMultipleRequests);
  FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestMultipleNetworkInterfaces);
  FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestNotifyOnError);
  FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestOnDeviceDiscovered);
  FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestOnDiscoveryFinished);
  FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestOnDiscoveryRequest);
  FRIEND_TEST_ALL_PREFIXES(DialServiceTest, TestResponseParsing);
  DISALLOW_COPY_AND_ASSIGN(DialServiceImpl);
};

}  

#endif  

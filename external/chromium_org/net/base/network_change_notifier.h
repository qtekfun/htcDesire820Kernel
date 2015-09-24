// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_CHANGE_NOTIFIER_H_
#define NET_BASE_NETWORK_CHANGE_NOTIFIER_H_

#include "base/basictypes.h"
#include "base/observer_list_threadsafe.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

struct DnsConfig;
class HistogramWatcher;
class NetworkChangeNotifierFactory;
class URLRequest;

#if defined(OS_LINUX)
namespace internal {
class AddressTrackerLinux;
}
#endif

class NET_EXPORT NetworkChangeNotifier {
 public:
  
  
  enum ConnectionType {
    CONNECTION_UNKNOWN = 0, 
    CONNECTION_ETHERNET = 1,
    CONNECTION_WIFI = 2,
    CONNECTION_2G = 3,
    CONNECTION_3G = 4,
    CONNECTION_4G = 5,
    CONNECTION_NONE = 6     
  };

  class NET_EXPORT IPAddressObserver {
   public:
    
    
    virtual void OnIPAddressChanged() = 0;

   protected:
    IPAddressObserver() {}
    virtual ~IPAddressObserver() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(IPAddressObserver);
  };

  class NET_EXPORT ConnectionTypeObserver {
   public:
    
    
    
    
    virtual void OnConnectionTypeChanged(ConnectionType type) = 0;

   protected:
    ConnectionTypeObserver() {}
    virtual ~ConnectionTypeObserver() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(ConnectionTypeObserver);
  };

  class NET_EXPORT DNSObserver {
   public:
    
    
    virtual void OnDNSChanged() = 0;

   protected:
    DNSObserver() {}
    virtual ~DNSObserver() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(DNSObserver);
  };

  class NET_EXPORT NetworkChangeObserver {
   public:
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    virtual void OnNetworkChanged(ConnectionType type) = 0;

   protected:
    NetworkChangeObserver() {}
    virtual ~NetworkChangeObserver() {}

   private:
    DISALLOW_COPY_AND_ASSIGN(NetworkChangeObserver);
  };

  virtual ~NetworkChangeNotifier();

  
  
  
  virtual ConnectionType GetCurrentConnectionType() const = 0;

  
  
  static void SetFactory(NetworkChangeNotifierFactory* factory);

  
  
  
  
  
  
  static NetworkChangeNotifier* Create();

  
  
  
  
  
  
  
  
  
  
  static ConnectionType GetConnectionType();

  
  
  static void GetDnsConfig(DnsConfig* config);

#if defined(OS_LINUX)
  
  static const internal::AddressTrackerLinux* GetAddressTracker();
#endif

  
  
  
  
  
  
  
  
  static bool IsOffline();

  
  
  
  
  
  static bool IsConnectionCellular(ConnectionType type);

  
  
  static NetworkChangeNotifier* CreateMock();

  
  
  
  
  
  static void AddIPAddressObserver(IPAddressObserver* observer);
  static void AddConnectionTypeObserver(ConnectionTypeObserver* observer);
  static void AddDNSObserver(DNSObserver* observer);
  static void AddNetworkChangeObserver(NetworkChangeObserver* observer);

  
  
  
  
  
  
  
  static void RemoveIPAddressObserver(IPAddressObserver* observer);
  static void RemoveConnectionTypeObserver(ConnectionTypeObserver* observer);
  static void RemoveDNSObserver(DNSObserver* observer);
  static void RemoveNetworkChangeObserver(NetworkChangeObserver* observer);

  
  static void NotifyObserversOfIPAddressChangeForTests() {
    NotifyObserversOfIPAddressChange();
  }

  
  static const char* ConnectionTypeToString(ConnectionType type);

  
  
  
  
  static void NotifyDataReceived(const URLRequest& request, int bytes_read);

  
  
  static void InitHistogramWatcher();

  
  
  
  
  
  
  
  class NET_EXPORT DisableForTest {
   public:
    DisableForTest();
    ~DisableForTest();

   private:
    
    NetworkChangeNotifier* network_change_notifier_;
  };

 protected:
  
  
  
  
  
  
  struct NET_EXPORT NetworkChangeCalculatorParams {
    NetworkChangeCalculatorParams();
    
    
    base::TimeDelta ip_address_offline_delay_;
    
    
    base::TimeDelta ip_address_online_delay_;
    
    
    base::TimeDelta connection_type_offline_delay_;
    
    
    base::TimeDelta connection_type_online_delay_;
  };

  explicit NetworkChangeNotifier(
      const NetworkChangeCalculatorParams& params =
          NetworkChangeCalculatorParams());

#if defined(OS_LINUX)
  
  
  virtual const internal::AddressTrackerLinux*
      GetAddressTrackerInternal() const;
#endif

  
  
  
  static void NotifyObserversOfIPAddressChange();
  static void NotifyObserversOfConnectionTypeChange();
  static void NotifyObserversOfDNSChange();
  static void NotifyObserversOfNetworkChange(ConnectionType type);

  
  static void SetDnsConfig(const DnsConfig& config);

 private:
  friend class HostResolverImplDnsTest;
  friend class NetworkChangeNotifierAndroidTest;
  friend class NetworkChangeNotifierLinuxTest;
  friend class NetworkChangeNotifierWinTest;

  class NetworkState;
  class NetworkChangeCalculator;

  const scoped_refptr<ObserverListThreadSafe<IPAddressObserver> >
      ip_address_observer_list_;
  const scoped_refptr<ObserverListThreadSafe<ConnectionTypeObserver> >
      connection_type_observer_list_;
  const scoped_refptr<ObserverListThreadSafe<DNSObserver> >
      resolver_state_observer_list_;
  const scoped_refptr<ObserverListThreadSafe<NetworkChangeObserver> >
      network_change_observer_list_;

  
  scoped_ptr<NetworkState> network_state_;

  
  scoped_ptr<HistogramWatcher> histogram_watcher_;

  
  scoped_ptr<NetworkChangeCalculator> network_change_calculator_;

  DISALLOW_COPY_AND_ASSIGN(NetworkChangeNotifier);
};

}  

#endif  

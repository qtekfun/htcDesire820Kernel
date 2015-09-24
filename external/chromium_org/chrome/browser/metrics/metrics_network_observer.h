// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_METRICS_NETWORK_OBSERVER_H_
#define CHROME_BROWSER_METRICS_METRICS_NETWORK_OBSERVER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/common/metrics/proto/system_profile.pb.h"
#include "net/base/net_util.h"
#include "net/base/network_change_notifier.h"

using metrics::SystemProfileProto;

class MetricsNetworkObserver
    : public net::NetworkChangeNotifier::ConnectionTypeObserver {
 public:
  MetricsNetworkObserver();
  virtual ~MetricsNetworkObserver();

  
  void Reset();

  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;

  bool connection_type_is_ambiguous() const {
    return connection_type_is_ambiguous_;
  }

  SystemProfileProto::Network::ConnectionType connection_type() const;

  bool wifi_phy_layer_protocol_is_ambiguous() const {
    return wifi_phy_layer_protocol_is_ambiguous_;
  }

  SystemProfileProto::Network::WifiPHYLayerProtocol
  wifi_phy_layer_protocol() const;

 private:
  
  void ProbeWifiPHYLayerProtocol();
  
  
  void OnWifiPHYLayerProtocolResult(net::WifiPHYLayerProtocol mode);

  base::WeakPtrFactory<MetricsNetworkObserver> weak_ptr_factory_;

  
  bool connection_type_is_ambiguous_;
  
  net::NetworkChangeNotifier::ConnectionType connection_type_;

  
  bool wifi_phy_layer_protocol_is_ambiguous_;
  
  
  net::WifiPHYLayerProtocol wifi_phy_layer_protocol_;

  DISALLOW_COPY_AND_ASSIGN(MetricsNetworkObserver);
};

#endif  

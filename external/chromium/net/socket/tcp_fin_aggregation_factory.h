// Copyright (c) 2011, Code Aurora Forum. All rights reserved.

//     * Redistributions of source code must retain the above copyright
//       copyright notice, this list of conditions and the following
//       from this software without specific prior written permission.

// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS

#ifndef TCP_FIN_AGGREGATION_FACTORY_H_
#define TCP_FIN_AGGREGATION_FACTORY_H_

#include "tcp_fin_aggregation.h"
#include "client_socket_pool_base.h"
#include "base/synchronization/lock.h"
#include "tcp_fin_aggregation_bridge.h"

namespace net {
class ITCPFinAggregation;
namespace internal {
  class ClientSocketPoolBaseHelper;
}

static ITCPFinAggregation* (*tcpfin_create_)(internal::ClientSocketPoolBaseHelper* pool_base_helper) = NULL;

class TCPFinAggregationFactory {

public:


  static TCPFinAggregationFactory* GetTCPFinFactoryInstance(internal::ClientSocketPoolBaseHelper* pool_base_helper);

  ITCPFinAggregation* GetTCPFinAggregation(){ return m_pTCPFin;}

private:

  ITCPFinAggregation* m_pTCPFin;

  static TCPFinAggregationFactory* s_pFactory;

  static base::Lock m_mutex;

  TCPFinAggregationFactory(internal::ClientSocketPoolBaseHelper* pool_base_helper);

  ~TCPFinAggregationFactory();

  void InitTCPFinAggregation(internal::ClientSocketPoolBaseHelper* pool_base_helper);

  DISALLOW_COPY_AND_ASSIGN(TCPFinAggregationFactory);
};

} 
#endif

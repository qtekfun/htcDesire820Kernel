/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_BASE_NETWORK_H_
#define TALK_BASE_NETWORK_H_

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "talk/base/basictypes.h"

namespace talk_base {

class Network;
class NetworkSession;

class NetworkManager {
 public:
  virtual ~NetworkManager();

  
  
  
  
  bool GetNetworks(std::vector<Network*>* networks);

  
  void DumpNetworks(bool include_ignored);

  
  std::string GetState() const;
  void SetState(const std::string& str);

  
  static bool CreateNetworks(bool include_ignored,
                             std::vector<Network*>* networks);
  
  static bool IsIgnoredNetwork(const Network& network);

 protected:
  
  virtual bool EnumNetworks(bool include_ignored,
                            std::vector<Network*>* networks);

 private:
  typedef std::map<std::string, Network*> NetworkMap;

  NetworkMap networks_;
};

class Network {
 public:
  Network(const std::string& name, const std::string& description,
          uint32 ip, uint32 gateway_ip);

  
  
  const std::string& name() const { return name_; }

  
  
  const std::string& description() const { return description_; }

  
  uint32 ip() const { return ip_; }
  void set_ip(uint32 ip) { ip_ = ip; }

  
  uint32 gateway_ip() const { return gateway_ip_; }
  void set_gateway_ip(uint32 ip) { gateway_ip_ = ip; }

  
  
  bool ignored() const { return ignored_; }
  void set_ignored(bool ignored) { ignored_ = ignored; }

  
  void StartSession(NetworkSession* session);
  void StopSession(NetworkSession* session);

  
  
  void EstimateQuality();

  
  
  double quality() { return quality_; }

  
  std::string ToString() const;

 private:
  typedef std::vector<NetworkSession*> SessionList;

  std::string name_;
  std::string description_;
  uint32 ip_;
  uint32 gateway_ip_;
  bool ignored_;
  SessionList sessions_;
  double uniform_numerator_;
  double uniform_denominator_;
  double exponential_numerator_;
  double exponential_denominator_;
  uint32 last_data_time_;
  double quality_;

  
  void AddDataPoint(uint32 time, double quality);

  
  
  void SetState(const std::string& str);
  std::string GetState() const;

  friend class NetworkManager;
};

class NetworkSession {
 public:
  virtual ~NetworkSession() { }

  
  
  virtual bool HasQuality() = 0;

  
  
  virtual float GetCurrentQuality() = 0;
};

const double QUALITY_BAD  = 3.0;
const double QUALITY_FAIR = 3.35;
const double QUALITY_GOOD = 3.7;

}  

#endif  

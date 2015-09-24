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

#ifndef TALK_BASE_SOCKETADDRESS_H_
#define TALK_BASE_SOCKETADDRESS_H_

#include <string>
#include <vector>
#include <iosfwd>
#include "talk/base/basictypes.h"
#undef SetPort

struct sockaddr_in;

namespace talk_base {

class SocketAddress {
 public:
  
  SocketAddress();

  
  
  
  
  
  SocketAddress(const std::string& hostname, int port);

  
  SocketAddress(uint32 ip, int port);

  
  SocketAddress(const SocketAddress& addr);

  
  void Clear();

  
  bool IsNil() const;

  
  bool IsComplete() const;

  
  SocketAddress& operator=(const SocketAddress& addr);

  
  void SetIP(uint32 ip);

  
  
  void SetIP(const std::string& hostname);

  
  
  void SetResolvedIP(uint32 ip);

  
  void SetPort(int port);

  
  const std::string& hostname() const { return hostname_; }

  
  uint32 ip() const;

  
  uint16 port() const;

  
  std::string IPAsString() const;

  
  std::string PortAsString() const;

  
  std::string ToString() const;

  
  bool FromString(const std::string& str);

  friend std::ostream& operator<<(std::ostream& os, const SocketAddress& addr);

  
  
  bool IsAnyIP() const;
  inline bool IsAny() const { return IsAnyIP(); }  

  
  
  bool IsLoopbackIP() const;

  
  
  bool IsLocalIP() const;

  
  
  bool IsPrivateIP() const;

  
  bool IsUnresolvedIP() const;
  inline bool IsUnresolved() const { return IsUnresolvedIP(); }  

  
  
  
  bool ResolveIP(bool force = false, int* error = NULL);

  
  bool operator ==(const SocketAddress& addr) const;
  inline bool operator !=(const SocketAddress& addr) const {
    return !this->operator ==(addr);
  }

  
  bool operator <(const SocketAddress& addr) const;

  
  bool EqualIPs(const SocketAddress& addr) const;

  
  bool EqualPorts(const SocketAddress& addr) const;

  
  size_t Hash() const;

  // Returns the size of this address when written.
  size_t Size_() const;

  
  bool Write_(char* buf, int len) const;

  
  bool Read_(const char* buf, int len);

  
  void ToSockAddr(sockaddr_in* saddr) const;

  
  bool FromSockAddr(const sockaddr_in& saddr);

  
  static std::string IPToString(uint32 ip);

  
  
  static bool StringToIP(const std::string& str, uint32* ip);
  static uint32 StringToIP(const std::string& str);  

  
  static std::string GetHostname();

  
  static bool GetLocalIPs(std::vector<uint32>& ips);

 private:
  std::string hostname_;
  uint32 ip_;
  uint16 port_;
};

}  

#endif  

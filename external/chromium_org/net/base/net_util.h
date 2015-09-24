// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_UTIL_H_
#define NET_BASE_NET_UTIL_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#include <ws2tcpip.h>
#elif defined(OS_POSIX)
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "net/base/address_family.h"
#include "net/base/escape.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"

class GURL;

namespace base {
class FilePath;
class Time;
}

namespace url_canon {
struct CanonHostInfo;
}

namespace url_parse {
struct Parsed;
}

namespace net {

typedef uint32 FormatUrlType;
typedef uint32 FormatUrlTypes;

typedef std::vector<unsigned char> IPAddressNumber;
typedef std::vector<IPAddressNumber> IPAddressList;

static const size_t kIPv4AddressSize = 4;
static const size_t kIPv6AddressSize = 16;

NET_EXPORT extern const FormatUrlType kFormatUrlOmitNothing;

NET_EXPORT extern const FormatUrlType kFormatUrlOmitUsernamePassword;

NET_EXPORT extern const FormatUrlType kFormatUrlOmitHTTP;

NET_EXPORT extern const FormatUrlType kFormatUrlOmitTrailingSlashOnBareHostname;

NET_EXPORT extern const FormatUrlType kFormatUrlOmitAll;

NET_EXPORT_PRIVATE extern size_t GetCountOfExplicitlyAllowedPorts();

NET_EXPORT GURL FilePathToFileURL(const base::FilePath& path);

NET_EXPORT bool FileURLToFilePath(const GURL& url, base::FilePath* file_path);

NET_EXPORT bool ParseHostAndPort(
    std::string::const_iterator host_and_port_begin,
    std::string::const_iterator host_and_port_end,
    std::string* host,
    int* port);
NET_EXPORT bool ParseHostAndPort(
    const std::string& host_and_port,
    std::string* host,
    int* port);

NET_EXPORT std::string GetHostAndPort(const GURL& url);

NET_EXPORT_PRIVATE std::string GetHostAndOptionalPort(const GURL& url);

NET_EXPORT bool IsHostnameNonUnique(const std::string& hostname);

NET_EXPORT bool IsIPAddressReserved(const IPAddressNumber& address);

struct SockaddrStorage {
  SockaddrStorage() : addr_len(sizeof(addr_storage)),
                      addr(reinterpret_cast<struct sockaddr*>(&addr_storage)) {}
  struct sockaddr_storage addr_storage;
  socklen_t addr_len;
  struct sockaddr* const addr;
};

bool GetIPAddressFromSockAddr(const struct sockaddr* sock_addr,
                              socklen_t sock_addr_len,
                              const unsigned char** address,
                              size_t* address_len,
                              uint16* port);

NET_EXPORT std::string IPAddressToString(const uint8* address,
                                         size_t address_len);

NET_EXPORT std::string IPAddressToStringWithPort(const uint8* address,
                                                 size_t address_len,
                                                 uint16 port);

NET_EXPORT std::string NetAddressToString(const struct sockaddr* sa,
                                          socklen_t sock_addr_len);

NET_EXPORT std::string NetAddressToStringWithPort(const struct sockaddr* sa,
                                                  socklen_t sock_addr_len);

NET_EXPORT std::string IPAddressToString(const IPAddressNumber& addr);

NET_EXPORT std::string IPAddressToStringWithPort(
    const IPAddressNumber& addr, uint16 port);

NET_EXPORT std::string IPAddressToPackedString(const IPAddressNumber& addr);

NET_EXPORT std::string GetHostName();

NET_EXPORT_PRIVATE void GetIdentityFromURL(const GURL& url,
                                           base::string16* username,
                                           base::string16* password);

NET_EXPORT std::string GetHostOrSpecFromURL(const GURL& url);

NET_EXPORT std::string GetSpecificHeader(const std::string& headers,
                                         const std::string& name);

NET_EXPORT base::string16 IDNToUnicode(const std::string& host,
                                       const std::string& languages);

NET_EXPORT std::string CanonicalizeHost(const std::string& host,
                                        url_canon::CanonHostInfo* host_info);

NET_EXPORT bool IsCanonicalizedHostCompliant(const std::string& host,
                                             const std::string& desired_tld);

NET_EXPORT std::string GetDirectoryListingHeader(const base::string16& title);

NET_EXPORT std::string GetDirectoryListingEntry(const base::string16& name,
                                                const std::string& raw_bytes,
                                                bool is_dir, int64 size,
                                                base::Time modified);

NET_EXPORT base::string16 StripWWW(const base::string16& text);

NET_EXPORT base::string16 StripWWWFromHost(const GURL& url);

NET_EXPORT base::string16 GetSuggestedFilename(
    const GURL& url,
    const std::string& content_disposition,
    const std::string& referrer_charset,
    const std::string& suggested_name,
    const std::string& mime_type,
    const std::string& default_name);

NET_EXPORT base::FilePath GenerateFileName(
    const GURL& url,
    const std::string& content_disposition,
    const std::string& referrer_charset,
    const std::string& suggested_name,
    const std::string& mime_type,
    const std::string& default_name);

NET_EXPORT bool IsSafePortablePathComponent(const base::FilePath& component);

NET_EXPORT bool IsSafePortableRelativePath(const base::FilePath& path);

NET_EXPORT void GenerateSafeFileName(const std::string& mime_type,
                                     bool ignore_extension,
                                     base::FilePath* file_path);

NET_EXPORT bool IsPortAllowedByDefault(int port);

NET_EXPORT_PRIVATE bool IsPortAllowedByFtp(int port);

NET_EXPORT_PRIVATE bool IsPortAllowedByOverride(int port);

NET_EXPORT int SetNonBlocking(int fd);

NET_EXPORT void AppendFormattedHost(const GURL& url,
                                    const std::string& languages,
                                    base::string16* output);

NET_EXPORT base::string16 FormatUrl(const GURL& url,
                                    const std::string& languages,
                                    FormatUrlTypes format_types,
                                    UnescapeRule::Type unescape_rules,
                                    url_parse::Parsed* new_parsed,
                                    size_t* prefix_end,
                                    size_t* offset_for_adjustment);
NET_EXPORT base::string16 FormatUrlWithOffsets(
    const GURL& url,
    const std::string& languages,
    FormatUrlTypes format_types,
    UnescapeRule::Type unescape_rules,
    url_parse::Parsed* new_parsed,
    size_t* prefix_end,
    std::vector<size_t>* offsets_for_adjustment);

inline base::string16 FormatUrl(const GURL& url, const std::string& languages) {
  return FormatUrl(url, languages, kFormatUrlOmitAll, UnescapeRule::SPACES,
                   NULL, NULL, NULL);
}

NET_EXPORT bool CanStripTrailingSlash(const GURL& url);

NET_EXPORT_PRIVATE GURL SimplifyUrlForRequest(const GURL& url);

NET_EXPORT void SetExplicitlyAllowedPorts(const std::string& allowed_ports);

class NET_EXPORT ScopedPortException {
 public:
  explicit ScopedPortException(int port);
  ~ScopedPortException();

 private:
  int port_;

  DISALLOW_COPY_AND_ASSIGN(ScopedPortException);
};

bool HaveOnlyLoopbackAddresses();

NET_EXPORT_PRIVATE AddressFamily GetAddressFamily(
    const IPAddressNumber& address);

NET_EXPORT_PRIVATE int ConvertAddressFamily(AddressFamily address_family);

NET_EXPORT_PRIVATE bool ParseIPLiteralToNumber(const std::string& ip_literal,
                                               IPAddressNumber* ip_number);

NET_EXPORT_PRIVATE IPAddressNumber ConvertIPv4NumberToIPv6Number(
    const IPAddressNumber& ipv4_number);

NET_EXPORT_PRIVATE bool IsIPv4Mapped(const IPAddressNumber& address);

NET_EXPORT_PRIVATE IPAddressNumber ConvertIPv4MappedToIPv4(
    const IPAddressNumber& address);

NET_EXPORT bool ParseCIDRBlock(const std::string& cidr_literal,
                               IPAddressNumber* ip_number,
                               size_t* prefix_length_in_bits);

NET_EXPORT_PRIVATE bool IPNumberMatchesPrefix(const IPAddressNumber& ip_number,
                                              const IPAddressNumber& ip_prefix,
                                              size_t prefix_length_in_bits);

const uint16* GetPortFieldFromSockaddr(const struct sockaddr* address,
                                       socklen_t address_len);
NET_EXPORT_PRIVATE int GetPortFromSockaddr(const struct sockaddr* address,
                                           socklen_t address_len);

NET_EXPORT_PRIVATE bool IsLocalhost(const std::string& host);

struct NET_EXPORT NetworkInterface {
  NetworkInterface();
  NetworkInterface(const std::string& name,
                   uint32 interface_index,
                   const IPAddressNumber& address,
                   size_t network_prefix);
  ~NetworkInterface();

  std::string name;
  uint32 interface_index;  
  IPAddressNumber address;
  size_t network_prefix;
};

typedef std::vector<NetworkInterface> NetworkInterfaceList;

NET_EXPORT bool GetNetworkList(NetworkInterfaceList* networks);

enum WifiPHYLayerProtocol {
  
  WIFI_PHY_LAYER_PROTOCOL_NONE,
  
  WIFI_PHY_LAYER_PROTOCOL_ANCIENT,
  
  WIFI_PHY_LAYER_PROTOCOL_A,
  
  WIFI_PHY_LAYER_PROTOCOL_B,
  
  WIFI_PHY_LAYER_PROTOCOL_G,
  
  WIFI_PHY_LAYER_PROTOCOL_N,
  
  WIFI_PHY_LAYER_PROTOCOL_UNKNOWN
};

NET_EXPORT WifiPHYLayerProtocol GetWifiPHYLayerProtocol();

unsigned CommonPrefixLength(const IPAddressNumber& a1,
                            const IPAddressNumber& a2);

unsigned MaskPrefixLength(const IPAddressNumber& mask);

enum DiffServCodePoint {
  DSCP_NO_CHANGE = -1,
  DSCP_DEFAULT = 0,  
  DSCP_CS0  = 0,   
  DSCP_CS1  = 8,   
  DSCP_AF11 = 10,
  DSCP_AF12 = 12,
  DSCP_AF13 = 14,
  DSCP_CS2  = 16,
  DSCP_AF21 = 18,
  DSCP_AF22 = 20,
  DSCP_AF23 = 22,
  DSCP_CS3  = 24,
  DSCP_AF31 = 26,
  DSCP_AF32 = 28,
  DSCP_AF33 = 30,
  DSCP_CS4  = 32,
  DSCP_AF41 = 34,  
  DSCP_AF42 = 36,  
  DSCP_AF43 = 38,  
  DSCP_CS5  = 40,  
  DSCP_EF   = 46,  
  DSCP_CS6  = 48,  
  DSCP_CS7  = 56,  
};

}  

#endif  

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_UTIL_H_
#define NET_BASE_NET_UTIL_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#include <ws2tcpip.h>
#elif defined(OS_POSIX)
#include <sys/socket.h>
#endif

#include <list>
#include <string>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/string16.h"
#include "net/base/escape.h"
#include "net/base/net_export.h"

struct addrinfo;
class FilePath;
class GURL;

namespace base {
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

class QuoteRule {
 public:
  enum Type {
    KEEP_OUTER_QUOTES,
    REMOVE_OUTER_QUOTES,
  };

 private:
  QuoteRule();
};

extern const FormatUrlType kFormatUrlOmitNothing;

extern const FormatUrlType kFormatUrlOmitUsernamePassword;

extern const FormatUrlType kFormatUrlOmitHTTP;

extern const FormatUrlType kFormatUrlOmitTrailingSlashOnBareHostname;

extern const FormatUrlType kFormatUrlOmitAll;

extern std::multiset<int> explicitly_allowed_ports;

GURL FilePathToFileURL(const FilePath& path);

bool FileURLToFilePath(const GURL& url, FilePath* file_path);

bool ParseHostAndPort(std::string::const_iterator host_and_port_begin,
                      std::string::const_iterator host_and_port_end,
                      std::string* host,
                      int* port);
bool ParseHostAndPort(const std::string& host_and_port,
                      std::string* host,
                      int* port);

std::string GetHostAndPort(const GURL& url);

std::string GetHostAndOptionalPort(const GURL& url);

std::string NetAddressToString(const struct addrinfo* net_address);
std::string NetAddressToString(const struct sockaddr* net_address,
                               socklen_t address_len);

std::string NetAddressToStringWithPort(const struct addrinfo* net_address);
std::string NetAddressToStringWithPort(const struct sockaddr* net_address,
                                       socklen_t address_len);

std::string GetHostName();

void GetIdentityFromURL(const GURL& url,
                        string16* username,
                        string16* password);

std::string GetHostOrSpecFromURL(const GURL& url);

std::wstring GetSpecificHeader(const std::wstring& headers,
                               const std::wstring& name);
std::string GetSpecificHeader(const std::string& headers,
                              const std::string& name);

std::wstring GetHeaderParamValue(const std::wstring& field,
                                 const std::wstring& param_name,
                                 QuoteRule::Type quote_rule);
std::string GetHeaderParamValue(const std::string& field,
                                const std::string& param_name,
                                QuoteRule::Type quote_rule);

std::string GetFileNameFromCD(const std::string& header,
                              const std::string& referrer_charset);

NET_EXPORT std::wstring IDNToUnicode(const char* host,
                          size_t host_len,
                          const std::wstring& languages,
                          size_t* offset_for_adjustment);
std::wstring IDNToUnicodeWithOffsets(
    const char* host,
    size_t host_len,
    const std::wstring& languages,
    std::vector<size_t>* offsets_for_adjustment);

std::string CanonicalizeHost(const std::string& host,
                             url_canon::CanonHostInfo* host_info);
std::string CanonicalizeHost(const std::wstring& host,
                             url_canon::CanonHostInfo* host_info);

bool IsCanonicalizedHostCompliant(const std::string& host,
                                  const std::string& desired_tld);

std::string GetDirectoryListingHeader(const string16& title);

std::string GetDirectoryListingEntry(const string16& name,
                                     const std::string& raw_bytes,
                                     bool is_dir, int64 size,
                                     base::Time modified);

string16 StripWWW(const string16& text);


string16 GetSuggestedFilename(const GURL& url,
                              const std::string& content_disposition,
                              const std::string& referrer_charset,
                              const string16& default_name);

bool IsPortAllowedByDefault(int port);

bool IsPortAllowedByFtp(int port);

bool IsPortAllowedByOverride(int port);

int SetNonBlocking(int fd);

void AppendFormattedHost(const GURL& url,
                         const std::wstring& languages,
                         std::wstring* output,
                         url_parse::Parsed* new_parsed,
                         size_t* offset_for_adjustment);
void AppendFormattedHostWithOffsets(
    const GURL& url,
    const std::wstring& languages,
    std::wstring* output,
    url_parse::Parsed* new_parsed,
    std::vector<size_t>* offsets_for_adjustment);

string16 FormatUrl(const GURL& url,
                   const std::string& languages,
                   FormatUrlTypes format_types,
                   UnescapeRule::Type unescape_rules,
                   url_parse::Parsed* new_parsed,
                   size_t* prefix_end,
                   size_t* offset_for_adjustment);
string16 FormatUrlWithOffsets(const GURL& url,
                              const std::string& languages,
                              FormatUrlTypes format_types,
                              UnescapeRule::Type unescape_rules,
                              url_parse::Parsed* new_parsed,
                              size_t* prefix_end,
                              std::vector<size_t>* offsets_for_adjustment);

inline string16 FormatUrl(const GURL& url, const std::string& languages) {
  return FormatUrl(url, languages, kFormatUrlOmitAll, UnescapeRule::SPACES,
                   NULL, NULL, NULL);
}

bool CanStripTrailingSlash(const GURL& url);

GURL SimplifyUrlForRequest(const GURL& url);

void SetExplicitlyAllowedPorts(const std::string& allowed_ports);

class ScopedPortException {
 public:
  ScopedPortException(int port);
  ~ScopedPortException();

 private:
  int port_;

  DISALLOW_COPY_AND_ASSIGN(ScopedPortException);
};

bool IPv6Supported();

bool HaveOnlyLoopbackAddresses();

typedef std::vector<unsigned char> IPAddressNumber;

static const size_t kIPv4AddressSize = 4;
static const size_t kIPv6AddressSize = 16;

bool ParseIPLiteralToNumber(const std::string& ip_literal,
                            IPAddressNumber* ip_number);

IPAddressNumber ConvertIPv4NumberToIPv6Number(
    const IPAddressNumber& ipv4_number);

bool ParseCIDRBlock(const std::string& cidr_literal,
                    IPAddressNumber* ip_number,
                    size_t* prefix_length_in_bits);

bool IPNumberMatchesPrefix(const IPAddressNumber& ip_number,
                           const IPAddressNumber& ip_prefix,
                           size_t prefix_length_in_bits);

struct addrinfo* CreateCopyOfAddrinfo(const struct addrinfo* info,
                                      bool recursive);

void FreeCopyOfAddrinfo(struct addrinfo* info);

const uint16* GetPortFieldFromAddrinfo(const struct addrinfo* info);
uint16* GetPortFieldFromAddrinfo(struct addrinfo* info);

int GetPortFromAddrinfo(const struct addrinfo* info);

const uint16* GetPortFieldFromSockaddr(const struct sockaddr* address,
                                       socklen_t address_len);
int GetPortFromSockaddr(const struct sockaddr* address,
                        socklen_t address_len);

bool IsLocalhost(const std::string& host);

struct NetworkInterface {
  NetworkInterface();
  NetworkInterface(const std::string& name, const IPAddressNumber& address);
  ~NetworkInterface();

  std::string name;
  IPAddressNumber address;
};

typedef std::list<NetworkInterface> NetworkInterfaceList;

bool GetNetworkList(NetworkInterfaceList* networks);

struct ClampComponentOffset {
  explicit ClampComponentOffset(size_t component_start);
  size_t operator()(size_t offset);

  const size_t component_start;
};

}  

#endif  

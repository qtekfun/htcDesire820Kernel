// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_RESPONSE_HEADERS_H_
#define NET_HTTP_HTTP_RESPONSE_HEADERS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/http/http_version.h"

class Pickle;
class PickleIterator;

namespace base {
class Time;
class TimeDelta;
}

namespace net {

class NET_EXPORT HttpResponseHeaders
    : public base::RefCountedThreadSafe<HttpResponseHeaders> {
 public:
  
  typedef int PersistOptions;
  static const PersistOptions PERSIST_RAW = -1;  
  static const PersistOptions PERSIST_ALL = 0;  
  static const PersistOptions PERSIST_SANS_COOKIES = 1 << 0;
  static const PersistOptions PERSIST_SANS_CHALLENGES = 1 << 1;
  static const PersistOptions PERSIST_SANS_HOP_BY_HOP = 1 << 2;
  static const PersistOptions PERSIST_SANS_NON_CACHEABLE = 1 << 3;
  static const PersistOptions PERSIST_SANS_RANGES = 1 << 4;
  static const PersistOptions PERSIST_SANS_SECURITY_STATE = 1 << 5;

  
  
  
  
  
  
  
  
  explicit HttpResponseHeaders(const std::string& raw_headers);

  
  
  
  HttpResponseHeaders(const Pickle& pickle, PickleIterator* pickle_iter);

  
  
  void Persist(Pickle* pickle, PersistOptions options);

  
  void Update(const HttpResponseHeaders& new_headers);

  
  void RemoveHeader(const std::string& name);

  
  
  void RemoveHeaderLine(const std::string& name, const std::string& value);

  
  
  
  
  
  void AddHeader(const std::string& header);

  
  
  void ReplaceStatusLine(const std::string& new_status);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void GetNormalizedHeaders(std::string* output) const;

  
  
  
  
  
  
  
  
  
  
  bool GetNormalizedHeader(const std::string& name, std::string* value) const;

  
  
  
  std::string GetStatusLine() const;

  
  HttpVersion GetHttpVersion() const {
    return http_version_;
  }

  
  HttpVersion GetParsedHttpVersion() const {
    return parsed_http_version_;
  }

  
  std::string GetStatusText() const;

  
  
  
  
  
  
  
  
  
  bool EnumerateHeaderLines(void** iter,
                            std::string* name,
                            std::string* value) const;

  
  
  
  
  
  
  bool EnumerateHeader(void** iter,
                       const base::StringPiece& name,
                       std::string* value) const;

  
  
  bool HasHeaderValue(const base::StringPiece& name,
                      const base::StringPiece& value) const;

  
  
  bool HasHeader(const base::StringPiece& name) const;

  
  
  void GetMimeTypeAndCharset(std::string* mime_type,
                             std::string* charset) const;

  
  
  bool GetMimeType(std::string* mime_type) const;

  
  
  bool GetCharset(std::string* charset) const;

  
  
  bool IsRedirect(std::string* location) const;

  
  
  static bool IsRedirectResponseCode(int response_code);

  
  
  
  
  
  bool RequiresValidation(const base::Time& request_time,
                          const base::Time& response_time,
                          const base::Time& current_time) const;

  
  
  
  base::TimeDelta GetFreshnessLifetime(const base::Time& response_time) const;

  
  
  base::TimeDelta GetCurrentAge(const base::Time& request_time,
                                const base::Time& response_time,
                                const base::Time& current_time) const;

  
  
  
  bool GetMaxAgeValue(base::TimeDelta* value) const;
  bool GetAgeValue(base::TimeDelta* value) const;
  bool GetDateValue(base::Time* value) const;
  bool GetLastModifiedValue(base::Time* value) const;
  bool GetExpiresValue(base::Time* value) const;

  
  
  bool GetTimeValuedHeader(const std::string& name, base::Time* result) const;

  
  bool IsKeepAlive() const;

  
  
  bool HasStrongValidators() const;

  
  
  int64 GetContentLength() const;

  
  
  int64 GetInt64HeaderValue(const std::string& header) const;

  
  
  
  
  
  
  
  bool GetContentRange(int64* first_byte_position,
                       int64* last_byte_position,
                       int64* instance_length) const;

  
  bool IsChunkEncoded() const;

#if defined (SPDY_PROXY_AUTH_ORIGIN)
  
  struct ChromeProxyInfo {
    ChromeProxyInfo() : bypass_all(false) {}

    
    
    bool bypass_all;

    
    base::TimeDelta bypass_duration;
  };

  
  
  
  
  
  
  bool GetChromeProxyInfo(ChromeProxyInfo* proxy_info) const;
#endif

  
  base::Value* NetLogCallback(NetLog::LogLevel log_level) const;

  
  
  
  
  
  static bool FromNetLogParam(
      const base::Value* event_param,
      scoped_refptr<HttpResponseHeaders>* http_response_headers);

  
  
  
  int response_code() const { return response_code_; }

  
  const std::string& raw_headers() const { return raw_headers_; }

 private:
  friend class base::RefCountedThreadSafe<HttpResponseHeaders>;

  typedef base::hash_set<std::string> HeaderSet;

  
  struct ParsedHeader;
  typedef std::vector<ParsedHeader> HeaderList;

  HttpResponseHeaders();
  ~HttpResponseHeaders();

  
  void Parse(const std::string& raw_input);

  
  
  
  
  
  static HttpVersion ParseVersion(std::string::const_iterator line_begin,
                                  std::string::const_iterator line_end);

  
  
  
  
  
  
  void ParseStatusLine(std::string::const_iterator line_begin,
                       std::string::const_iterator line_end,
                       bool has_headers);

  
  
  size_t FindHeader(size_t from, const base::StringPiece& name) const;

  
  
  void AddHeader(std::string::const_iterator name_begin,
                 std::string::const_iterator name_end,
                 std::string::const_iterator value_begin,
                 std::string::const_iterator value_end);

  
  void AddToParsed(std::string::const_iterator name_begin,
                   std::string::const_iterator name_end,
                   std::string::const_iterator value_begin,
                   std::string::const_iterator value_end);

  
  
  
  
  void MergeWithHeaders(const std::string& raw_headers,
                        const HeaderSet& headers_to_remove);

  
  void AddNonCacheableHeaders(HeaderSet* header_names) const;

  
  static void AddSensitiveHeaders(HeaderSet* header_names);

  
  static void AddHopByHopHeaders(HeaderSet* header_names);

  
  static void AddChallengeHeaders(HeaderSet* header_names);

  
  static void AddCookieHeaders(HeaderSet* header_names);

  
  static void AddHopContentRangeHeaders(HeaderSet* header_names);

  
  static void AddSecurityStateHeaders(HeaderSet* header_names);

#if defined(SPDY_PROXY_AUTH_ORIGIN)
  
  
  bool GetChromeProxyBypassDuration(const std::string& action_prefix,
                                    base::TimeDelta* duration) const;
#endif

  
  
  HeaderList parsed_;

  
  
  
  
  
  
  std::string raw_headers_;

  
  int response_code_;

  
  HttpVersion http_version_;

  
  HttpVersion parsed_http_version_;

  DISALLOW_COPY_AND_ASSIGN(HttpResponseHeaders);
};

}  

#endif  

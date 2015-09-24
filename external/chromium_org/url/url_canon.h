// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef URL_URL_CANON_H_
#define URL_URL_CANON_H_

#include <stdlib.h>
#include <string.h>

#include "base/strings/string16.h"
#include "url/url_export.h"
#include "url/url_parse.h"

namespace url_canon {


template<typename T>
class CanonOutputT {
 public:
  CanonOutputT() : buffer_(NULL), buffer_len_(0), cur_len_(0) {
  }
  virtual ~CanonOutputT() {
  }

  
  
  
  
  
  virtual void Resize(int sz) = 0;

  
  
  inline char at(int offset) const {
    return buffer_[offset];
  }

  
  
  inline void set(int offset, int ch) {
    buffer_[offset] = ch;
  }

  
  inline int length() const {
    return cur_len_;
  }

  
  // characters that have been declared to be written, but the capacity() is
  // the number that can be written without reallocation. If the caller must
  
  
  int capacity() const {
    return buffer_len_;
  }

  
  
  
  const T* data() const {
    return buffer_;
  }
  T* data() {
    return buffer_;
  }

  
  
  
  
  
  
  void set_length(int new_len) {
    cur_len_ = new_len;
  }

  
  
  void push_back(T ch) {
    
    
    if (cur_len_ < buffer_len_) {
      buffer_[cur_len_] = ch;
      cur_len_++;
      return;
    }

    
    
    if (!Grow(1))
      return;

    
    buffer_[cur_len_] = ch;
    cur_len_++;
  }

  
  void Append(const T* str, int str_len) {
    if (cur_len_ + str_len > buffer_len_) {
      if (!Grow(cur_len_ + str_len - buffer_len_))
        return;
    }
    for (int i = 0; i < str_len; i++)
      buffer_[cur_len_ + i] = str[i];
    cur_len_ += str_len;
  }

 protected:
  
  
  bool Grow(int min_additional) {
    static const int kMinBufferLen = 16;
    int new_len = (buffer_len_ == 0) ? kMinBufferLen : buffer_len_;
    do {
      if (new_len >= (1 << 30))  
        return false;
      new_len *= 2;
    } while (new_len < buffer_len_ + min_additional);
    Resize(new_len);
    return true;
  }

  T* buffer_;
  int buffer_len_;

  
  int cur_len_;
};

template<typename T, int fixed_capacity = 1024>
class RawCanonOutputT : public CanonOutputT<T> {
 public:
  RawCanonOutputT() : CanonOutputT<T>() {
    this->buffer_ = fixed_buffer_;
    this->buffer_len_ = fixed_capacity;
  }
  virtual ~RawCanonOutputT() {
    if (this->buffer_ != fixed_buffer_)
      delete[] this->buffer_;
  }

  virtual void Resize(int sz) {
    T* new_buf = new T[sz];
    memcpy(new_buf, this->buffer_,
           sizeof(T) * (this->cur_len_ < sz ? this->cur_len_ : sz));
    if (this->buffer_ != fixed_buffer_)
      delete[] this->buffer_;
    this->buffer_ = new_buf;
    this->buffer_len_ = sz;
  }

 protected:
  T fixed_buffer_[fixed_capacity];
};

typedef CanonOutputT<char> CanonOutput;
typedef CanonOutputT<base::char16> CanonOutputW;

template<int fixed_capacity>
class RawCanonOutput : public RawCanonOutputT<char, fixed_capacity> {};
template<int fixed_capacity>
class RawCanonOutputW : public RawCanonOutputT<base::char16, fixed_capacity> {};


class URL_EXPORT CharsetConverter {
 public:
  CharsetConverter() {}
  virtual ~CharsetConverter() {}

  
  
  
  
  
  
  
  
  
  
  
  virtual void ConvertFromUTF16(const base::char16* input,
                                int input_len,
                                CanonOutput* output) = 0;
};


URL_EXPORT const char* RemoveURLWhitespace(const char* input, int input_len,
                                           CanonOutputT<char>* buffer,
                                           int* output_len);
URL_EXPORT const base::char16* RemoveURLWhitespace(
    const base::char16* input,
    int input_len,
    CanonOutputT<base::char16>* buffer,
    int* output_len);


URL_EXPORT bool IDNToASCII(const base::char16* src,
                           int src_len,
                           CanonOutputW* output);

// the canonicalized component will be written to the output component.

URL_EXPORT bool CanonicalizeScheme(const char* spec,
                                   const url_parse::Component& scheme,
                                   CanonOutput* output,
                                   url_parse::Component* out_scheme);
URL_EXPORT bool CanonicalizeScheme(const base::char16* spec,
                                   const url_parse::Component& scheme,
                                   CanonOutput* output,
                                   url_parse::Component* out_scheme);

URL_EXPORT bool CanonicalizeUserInfo(const char* username_source,
                                     const url_parse::Component& username,
                                     const char* password_source,
                                     const url_parse::Component& password,
                                     CanonOutput* output,
                                     url_parse::Component* out_username,
                                     url_parse::Component* out_password);
URL_EXPORT bool CanonicalizeUserInfo(const base::char16* username_source,
                                     const url_parse::Component& username,
                                     const base::char16* password_source,
                                     const url_parse::Component& password,
                                     CanonOutput* output,
                                     url_parse::Component* out_username,
                                     url_parse::Component* out_password);


struct CanonHostInfo {
  CanonHostInfo() : family(NEUTRAL), num_ipv4_components(0), out_host() {}

  
  bool IsIPAddress() const { return family == IPV4 || family == IPV6; }

  
  enum Family {
    NEUTRAL,   
               
               
    BROKEN,    
               
               
               
               
    IPV4,      
    IPV6,      
  };
  Family family;

  
  
  
  int num_ipv4_components;

  
  
  
  url_parse::Component out_host;

  
  
  
  unsigned char address[16];

  
  
  int AddressLength() const {
    return family == IPV4 ? 4 : (family == IPV6 ? 16 : 0);
  }
};


URL_EXPORT bool CanonicalizeHost(const char* spec,
                                 const url_parse::Component& host,
                                 CanonOutput* output,
                                 url_parse::Component* out_host);
URL_EXPORT bool CanonicalizeHost(const base::char16* spec,
                                 const url_parse::Component& host,
                                 CanonOutput* output,
                                 url_parse::Component* out_host);

URL_EXPORT void CanonicalizeHostVerbose(const char* spec,
                                        const url_parse::Component& host,
                                        CanonOutput* output,
                                        CanonHostInfo* host_info);
URL_EXPORT void CanonicalizeHostVerbose(const base::char16* spec,
                                        const url_parse::Component& host,
                                        CanonOutput* output,
                                        CanonHostInfo* host_info);


URL_EXPORT void CanonicalizeIPAddress(const char* spec,
                                      const url_parse::Component& host,
                                      CanonOutput* output,
                                      CanonHostInfo* host_info);
URL_EXPORT void CanonicalizeIPAddress(const base::char16* spec,
                                      const url_parse::Component& host,
                                      CanonOutput* output,
                                      CanonHostInfo* host_info);

URL_EXPORT bool CanonicalizePort(const char* spec,
                                 const url_parse::Component& port,
                                 int default_port_for_scheme,
                                 CanonOutput* output,
                                 url_parse::Component* out_port);
URL_EXPORT bool CanonicalizePort(const base::char16* spec,
                                 const url_parse::Component& port,
                                 int default_port_for_scheme,
                                 CanonOutput* output,
                                 url_parse::Component* out_port);

URL_EXPORT int DefaultPortForScheme(const char* scheme, int scheme_len);

URL_EXPORT bool CanonicalizePath(const char* spec,
                                 const url_parse::Component& path,
                                 CanonOutput* output,
                                 url_parse::Component* out_path);
URL_EXPORT bool CanonicalizePath(const base::char16* spec,
                                 const url_parse::Component& path,
                                 CanonOutput* output,
                                 url_parse::Component* out_path);

URL_EXPORT bool FileCanonicalizePath(const char* spec,
                                     const url_parse::Component& path,
                                     CanonOutput* output,
                                     url_parse::Component* out_path);
URL_EXPORT bool FileCanonicalizePath(const base::char16* spec,
                                     const url_parse::Component& path,
                                     CanonOutput* output,
                                     url_parse::Component* out_path);

URL_EXPORT void CanonicalizeQuery(const char* spec,
                                  const url_parse::Component& query,
                                  CharsetConverter* converter,
                                  CanonOutput* output,
                                  url_parse::Component* out_query);
URL_EXPORT void CanonicalizeQuery(const base::char16* spec,
                                  const url_parse::Component& query,
                                  CharsetConverter* converter,
                                  CanonOutput* output,
                                  url_parse::Component* out_query);

URL_EXPORT void CanonicalizeRef(const char* spec,
                                const url_parse::Component& path,
                                CanonOutput* output,
                                url_parse::Component* out_path);
URL_EXPORT void CanonicalizeRef(const base::char16* spec,
                                const url_parse::Component& path,
                                CanonOutput* output,
                                url_parse::Component* out_path);


URL_EXPORT bool CanonicalizeStandardURL(const char* spec,
                                        int spec_len,
                                        const url_parse::Parsed& parsed,
                                        CharsetConverter* query_converter,
                                        CanonOutput* output,
                                        url_parse::Parsed* new_parsed);
URL_EXPORT bool CanonicalizeStandardURL(const base::char16* spec,
                                        int spec_len,
                                        const url_parse::Parsed& parsed,
                                        CharsetConverter* query_converter,
                                        CanonOutput* output,
                                        url_parse::Parsed* new_parsed);

URL_EXPORT bool CanonicalizeFileURL(const char* spec,
                                    int spec_len,
                                    const url_parse::Parsed& parsed,
                                    CharsetConverter* query_converter,
                                    CanonOutput* output,
                                    url_parse::Parsed* new_parsed);
URL_EXPORT bool CanonicalizeFileURL(const base::char16* spec,
                                    int spec_len,
                                    const url_parse::Parsed& parsed,
                                    CharsetConverter* query_converter,
                                    CanonOutput* output,
                                    url_parse::Parsed* new_parsed);

URL_EXPORT bool CanonicalizeFileSystemURL(const char* spec,
                                          int spec_len,
                                          const url_parse::Parsed& parsed,
                                          CharsetConverter* query_converter,
                                          CanonOutput* output,
                                          url_parse::Parsed* new_parsed);
URL_EXPORT bool CanonicalizeFileSystemURL(const base::char16* spec,
                                          int spec_len,
                                          const url_parse::Parsed& parsed,
                                          CharsetConverter* query_converter,
                                          CanonOutput* output,
                                          url_parse::Parsed* new_parsed);

URL_EXPORT bool CanonicalizePathURL(const char* spec,
                                    int spec_len,
                                    const url_parse::Parsed& parsed,
                                    CanonOutput* output,
                                    url_parse::Parsed* new_parsed);
URL_EXPORT bool CanonicalizePathURL(const base::char16* spec,
                                    int spec_len,
                                    const url_parse::Parsed& parsed,
                                    CanonOutput* output,
                                    url_parse::Parsed* new_parsed);

URL_EXPORT bool CanonicalizeMailtoURL(const char* spec,
                                      int spec_len,
                                      const url_parse::Parsed& parsed,
                                      CanonOutput* output,
                                      url_parse::Parsed* new_parsed);
URL_EXPORT bool CanonicalizeMailtoURL(const base::char16* spec,
                                      int spec_len,
                                      const url_parse::Parsed& parsed,
                                      CanonOutput* output,
                                      url_parse::Parsed* new_parsed);


template<typename CHAR>
struct URLComponentSource {
  
  
  
  URLComponentSource()
      : scheme(NULL),
        username(NULL),
        password(NULL),
        host(NULL),
        port(NULL),
        path(NULL),
        query(NULL),
        ref(NULL) {
  }

  
  
  explicit URLComponentSource(const CHAR* default_value)
      : scheme(default_value),
        username(default_value),
        password(default_value),
        host(default_value),
        port(default_value),
        path(default_value),
        query(default_value),
        ref(default_value) {
  }

  const CHAR* scheme;
  const CHAR* username;
  const CHAR* password;
  const CHAR* host;
  const CHAR* port;
  const CHAR* path;
  const CHAR* query;
  const CHAR* ref;
};

template<typename CHAR>
class Replacements {
 public:
  Replacements() {
  }

  
  void SetScheme(const CHAR* s, const url_parse::Component& comp) {
    sources_.scheme = s;
    components_.scheme = comp;
  }
  
  bool IsSchemeOverridden() const { return sources_.scheme != NULL; }

  
  void SetUsername(const CHAR* s, const url_parse::Component& comp) {
    sources_.username = s;
    components_.username = comp;
  }
  void ClearUsername() {
    sources_.username = Placeholder();
    components_.username = url_parse::Component();
  }
  bool IsUsernameOverridden() const { return sources_.username != NULL; }

  
  void SetPassword(const CHAR* s, const url_parse::Component& comp) {
    sources_.password = s;
    components_.password = comp;
  }
  void ClearPassword() {
    sources_.password = Placeholder();
    components_.password = url_parse::Component();
  }
  bool IsPasswordOverridden() const { return sources_.password != NULL; }

  
  void SetHost(const CHAR* s, const url_parse::Component& comp) {
    sources_.host = s;
    components_.host = comp;
  }
  void ClearHost() {
    sources_.host = Placeholder();
    components_.host = url_parse::Component();
  }
  bool IsHostOverridden() const { return sources_.host != NULL; }

  
  void SetPort(const CHAR* s, const url_parse::Component& comp) {
    sources_.port = s;
    components_.port = comp;
  }
  void ClearPort() {
    sources_.port = Placeholder();
    components_.port = url_parse::Component();
  }
  bool IsPortOverridden() const { return sources_.port != NULL; }

  
  void SetPath(const CHAR* s, const url_parse::Component& comp) {
    sources_.path = s;
    components_.path = comp;
  }
  void ClearPath() {
    sources_.path = Placeholder();
    components_.path = url_parse::Component();
  }
  bool IsPathOverridden() const { return sources_.path != NULL; }

  
  void SetQuery(const CHAR* s, const url_parse::Component& comp) {
    sources_.query = s;
    components_.query = comp;
  }
  void ClearQuery() {
    sources_.query = Placeholder();
    components_.query = url_parse::Component();
  }
  bool IsQueryOverridden() const { return sources_.query != NULL; }

  
  void SetRef(const CHAR* s, const url_parse::Component& comp) {
    sources_.ref = s;
    components_.ref = comp;
  }
  void ClearRef() {
    sources_.ref = Placeholder();
    components_.ref = url_parse::Component();
  }
  bool IsRefOverridden() const { return sources_.ref != NULL; }

  
  
  const URLComponentSource<CHAR>& sources() const { return sources_; }
  const url_parse::Parsed& components() const { return components_; }

 private:
  
  
  const CHAR* Placeholder() {
    static const CHAR empty_string = 0;
    return &empty_string;
  }

  
  
  
  
  
  
  
  
  
  
  URLComponentSource<CHAR> sources_;
  url_parse::Parsed components_;
};

URL_EXPORT bool ReplaceStandardURL(const char* base,
                                   const url_parse::Parsed& base_parsed,
                                   const Replacements<char>& replacements,
                                   CharsetConverter* query_converter,
                                   CanonOutput* output,
                                   url_parse::Parsed* new_parsed);
URL_EXPORT bool ReplaceStandardURL(
    const char* base,
    const url_parse::Parsed& base_parsed,
    const Replacements<base::char16>& replacements,
    CharsetConverter* query_converter,
    CanonOutput* output,
    url_parse::Parsed* new_parsed);

URL_EXPORT bool ReplaceFileSystemURL(const char* base,
                                     const url_parse::Parsed& base_parsed,
                                     const Replacements<char>& replacements,
                                     CharsetConverter* query_converter,
                                     CanonOutput* output,
                                     url_parse::Parsed* new_parsed);
URL_EXPORT bool ReplaceFileSystemURL(
    const char* base,
    const url_parse::Parsed& base_parsed,
    const Replacements<base::char16>& replacements,
    CharsetConverter* query_converter,
    CanonOutput* output,
    url_parse::Parsed* new_parsed);

URL_EXPORT bool ReplaceFileURL(const char* base,
                               const url_parse::Parsed& base_parsed,
                               const Replacements<char>& replacements,
                               CharsetConverter* query_converter,
                               CanonOutput* output,
                               url_parse::Parsed* new_parsed);
URL_EXPORT bool ReplaceFileURL(const char* base,
                               const url_parse::Parsed& base_parsed,
                               const Replacements<base::char16>& replacements,
                               CharsetConverter* query_converter,
                               CanonOutput* output,
                               url_parse::Parsed* new_parsed);

URL_EXPORT bool ReplacePathURL(const char* base,
                               const url_parse::Parsed& base_parsed,
                               const Replacements<char>& replacements,
                               CanonOutput* output,
                               url_parse::Parsed* new_parsed);
URL_EXPORT bool ReplacePathURL(const char* base,
                               const url_parse::Parsed& base_parsed,
                               const Replacements<base::char16>& replacements,
                               CanonOutput* output,
                               url_parse::Parsed* new_parsed);

URL_EXPORT bool ReplaceMailtoURL(const char* base,
                                 const url_parse::Parsed& base_parsed,
                                 const Replacements<char>& replacements,
                                 CanonOutput* output,
                                 url_parse::Parsed* new_parsed);
URL_EXPORT bool ReplaceMailtoURL(const char* base,
                                 const url_parse::Parsed& base_parsed,
                                 const Replacements<base::char16>& replacements,
                                 CanonOutput* output,
                                 url_parse::Parsed* new_parsed);


URL_EXPORT bool IsRelativeURL(const char* base,
                              const url_parse::Parsed& base_parsed,
                              const char* fragment,
                              int fragment_len,
                              bool is_base_hierarchical,
                              bool* is_relative,
                              url_parse::Component* relative_component);
URL_EXPORT bool IsRelativeURL(const char* base,
                              const url_parse::Parsed& base_parsed,
                              const base::char16* fragment,
                              int fragment_len,
                              bool is_base_hierarchical,
                              bool* is_relative,
                              url_parse::Component* relative_component);

URL_EXPORT bool ResolveRelativeURL(
    const char* base_url,
    const url_parse::Parsed& base_parsed,
    bool base_is_file,
    const char* relative_url,
    const url_parse::Component& relative_component,
    CharsetConverter* query_converter,
    CanonOutput* output,
    url_parse::Parsed* out_parsed);
URL_EXPORT bool ResolveRelativeURL(
    const char* base_url,
    const url_parse::Parsed& base_parsed,
    bool base_is_file,
    const base::char16* relative_url,
    const url_parse::Component& relative_component,
    CharsetConverter* query_converter,
    CanonOutput* output,
    url_parse::Parsed* out_parsed);

}  

#endif  

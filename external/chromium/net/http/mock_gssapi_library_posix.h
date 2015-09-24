// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_MOCK_GSSAPI_LIBRARY_POSIX_H_
#define NET_HTTP_MOCK_GSSAPI_LIBRARY_POSIX_H_
#pragma once

#include <list>
#include <string>

#include "base/gtest_prod_util.h"
#include "net/http/http_auth_gssapi_posix.h"
#include "net/third_party/gssapi/gssapi.h"

namespace net {

namespace test {

class GssContextMockImpl {
 public:
  GssContextMockImpl();
  GssContextMockImpl(const GssContextMockImpl& other);
  GssContextMockImpl(const char* src_name,
                     const char* targ_name,
                     OM_uint32 lifetime_rec,
                     const gss_OID_desc& mech_type,
                     OM_uint32 ctx_flags,
                     int locally_initiated,
                     int open);
  ~GssContextMockImpl();

  void Assign(const GssContextMockImpl& other);

  std::string src_name;
  std::string targ_name;
  OM_uint32 lifetime_rec;
  gss_OID_desc mech_type;
  OM_uint32 ctx_flags;
  int locally_initiated;
  int open;
};

class MockGSSAPILibrary : public GSSAPILibrary {
 public:
  
  struct SecurityContextQuery {
    SecurityContextQuery();
    SecurityContextQuery(const std::string& expected_package,
                         OM_uint32 response_code,
                         OM_uint32 minor_response_code,
                         const test::GssContextMockImpl& context_info,
                         const char* expected_input_token,
                         const char* output_token);
    ~SecurityContextQuery();

    std::string expected_package;
    OM_uint32 response_code;
    OM_uint32 minor_response_code;
    test::GssContextMockImpl context_info;
    gss_buffer_desc expected_input_token;
    gss_buffer_desc output_token;
  };

  MockGSSAPILibrary();
  virtual ~MockGSSAPILibrary();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ExpectSecurityContext(const std::string& expected_package,
                             OM_uint32 response_code,
                             OM_uint32 minor_response_code,
                             const test::GssContextMockImpl& context_info,
                             const gss_buffer_desc& expected_input_token,
                             const gss_buffer_desc& output_token);

  

  
  
  
  virtual bool Init();

  
  virtual OM_uint32 import_name(
      OM_uint32* minor_status,
      const gss_buffer_t input_name_buffer,
      const gss_OID input_name_type,
      gss_name_t* output_name);
  virtual OM_uint32 release_name(
      OM_uint32* minor_status,
      gss_name_t* input_name);
  virtual OM_uint32 release_buffer(
      OM_uint32* minor_status,
      gss_buffer_t buffer);
  virtual OM_uint32 display_name(
      OM_uint32* minor_status,
      const gss_name_t input_name,
      gss_buffer_t output_name_buffer,
      gss_OID* output_name_type);
  virtual OM_uint32 display_status(
      OM_uint32* minor_status,
      OM_uint32 status_value,
      int status_type,
      const gss_OID mech_type,
      OM_uint32* message_contex,
      gss_buffer_t status_string);
  virtual OM_uint32 init_sec_context(
      OM_uint32* minor_status,
      const gss_cred_id_t initiator_cred_handle,
      gss_ctx_id_t* context_handle,
      const gss_name_t target_name,
      const gss_OID mech_type,
      OM_uint32 req_flags,
      OM_uint32 time_req,
      const gss_channel_bindings_t input_chan_bindings,
      const gss_buffer_t input_token,
      gss_OID* actual_mech_type,
      gss_buffer_t output_token,
      OM_uint32* ret_flags,
      OM_uint32* time_rec);
  virtual OM_uint32 wrap_size_limit(
      OM_uint32* minor_status,
      const gss_ctx_id_t context_handle,
      int conf_req_flag,
      gss_qop_t qop_req,
      OM_uint32 req_output_size,
      OM_uint32* max_input_size);
  virtual OM_uint32 delete_sec_context(
      OM_uint32* minor_status,
      gss_ctx_id_t* context_handle,
      gss_buffer_t output_token);
  virtual OM_uint32 inquire_context(
      OM_uint32* minor_status,
      const gss_ctx_id_t context_handle,
      gss_name_t* src_name,
      gss_name_t* targ_name,
      OM_uint32* lifetime_rec,
      gss_OID* mech_type,
      OM_uint32* ctx_flags,
      int* locally_initiated,
      int* open);

 private:
  FRIEND_TEST_ALL_PREFIXES(HttpAuthGSSAPIPOSIXTest, GSSAPICycle);

  
  
  
  std::list<SecurityContextQuery> expected_security_queries_;
};

}  

}  

#endif  


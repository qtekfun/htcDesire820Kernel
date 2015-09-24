// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_DOCUMENT_STATE_H_
#define CONTENT_PUBLIC_RENDERER_DOCUMENT_STATE_H_

#include <string>

#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/supports_user_data.h"
#include "base/time/time.h"
#include "content/common/content_export.h"
#include "net/http/http_response_info.h"
#include "third_party/WebKit/public/web/WebDataSource.h"

namespace content {

class NavigationState;

class CONTENT_EXPORT DocumentState
    : NON_EXPORTED_BASE(public blink::WebDataSource::ExtraData),
      public base::SupportsUserData {
 public:
  
  
  enum LoadType {
    UNDEFINED_LOAD,            
    RELOAD,                    
    HISTORY_LOAD,              
    NORMAL_LOAD,               
    LINK_LOAD,                 
    LINK_LOAD_NORMAL,          
    LINK_LOAD_RELOAD,          
    LINK_LOAD_CACHE_STALE_OK,  
    LINK_LOAD_CACHE_ONLY,      
    kLoadTypeMax               
  };

  DocumentState();
  virtual ~DocumentState();

  static DocumentState* FromDataSource(blink::WebDataSource* ds) {
    return static_cast<DocumentState*>(ds->extraData());
  }

  
  const base::Time& request_time() const {
    return request_time_;
  }
  void set_request_time(const base::Time& value) {
    DCHECK(start_load_time_.is_null());
    request_time_ = value;
  }

  
  const base::Time& start_load_time() const {
    return start_load_time_;
  }
  void set_start_load_time(const base::Time& value) {
    
    
    DCHECK(finish_document_load_time_.is_null());
    start_load_time_ = value;
  }

  
  const base::Time& commit_load_time() const {
    return commit_load_time_;
  }
  void set_commit_load_time(const base::Time& value) {
    commit_load_time_ = value;
  }

  
  const base::Time& finish_document_load_time() const {
    return finish_document_load_time_;
  }
  void set_finish_document_load_time(const base::Time& value) {
    
    
    DCHECK(!value.is_null());
    
    
    
    
    finish_document_load_time_ = value;
  }

  
  const base::Time& finish_load_time() const { return finish_load_time_; }
  void set_finish_load_time(const base::Time& value) {
    DCHECK(!value.is_null());
    DCHECK(finish_load_time_.is_null());
    
    
    finish_load_time_ = value;
  }

  
  const base::Time& first_paint_time() const { return first_paint_time_; }
  void set_first_paint_time(const base::Time& value) {
    first_paint_time_ = value;
  }

  
  const base::Time& first_paint_after_load_time() const {
    return first_paint_after_load_time_;
  }
  void set_first_paint_after_load_time(const base::Time& value) {
    first_paint_after_load_time_ = value;
  }

  
  bool load_histograms_recorded() const { return load_histograms_recorded_; }
  void set_load_histograms_recorded(bool value) {
    load_histograms_recorded_ = value;
  }

  bool web_timing_histograms_recorded() const {
    return web_timing_histograms_recorded_;
  }
  void set_web_timing_histograms_recorded(bool value) {
    web_timing_histograms_recorded_ = value;
  }

  
  bool was_fetched_via_spdy() const { return was_fetched_via_spdy_; }
  void set_was_fetched_via_spdy(bool value) { was_fetched_via_spdy_ = value; }

  bool was_npn_negotiated() const { return was_npn_negotiated_; }
  void set_was_npn_negotiated(bool value) { was_npn_negotiated_ = value; }

  const std::string& npn_negotiated_protocol() const {
    return npn_negotiated_protocol_;
  }
  void set_npn_negotiated_protocol(const std::string& value) {
    npn_negotiated_protocol_ = value;
  }

  bool was_alternate_protocol_available() const {
    return was_alternate_protocol_available_;
  }
  void set_was_alternate_protocol_available(bool value) {
    was_alternate_protocol_available_ = value;
  }

  net::HttpResponseInfo::ConnectionInfo connection_info() const {
    return connection_info_;
  }
  void set_connection_info(
      net::HttpResponseInfo::ConnectionInfo connection_info) {
    connection_info_ = connection_info;
  }

  bool was_fetched_via_proxy() const { return was_fetched_via_proxy_; }
  void set_was_fetched_via_proxy(bool value) {
    was_fetched_via_proxy_ = value;
  }

  void set_was_prefetcher(bool value) { was_prefetcher_ = value; }
  bool was_prefetcher() const { return was_prefetcher_; }

  void set_was_referred_by_prefetcher(bool value) {
    was_referred_by_prefetcher_ = value;
  }
  bool was_referred_by_prefetcher() const {
    return was_referred_by_prefetcher_;
  }

  void set_was_after_preconnect_request(bool value) {
    was_after_preconnect_request_ = value;
  }
  bool was_after_preconnect_request() { return was_after_preconnect_request_; }

  
  LoadType load_type() const { return load_type_; }
  void set_load_type(LoadType load_type) { load_type_ = load_type; }

  NavigationState* navigation_state() { return navigation_state_.get(); }
  void set_navigation_state(NavigationState* navigation_state);

  bool can_load_local_resources() const { return can_load_local_resources_; }
  void set_can_load_local_resources(bool can_load) {
    can_load_local_resources_ = can_load;
  }

 private:
  base::Time request_time_;
  base::Time start_load_time_;
  base::Time commit_load_time_;
  base::Time finish_document_load_time_;
  base::Time finish_load_time_;
  base::Time first_paint_time_;
  base::Time first_paint_after_load_time_;
  bool load_histograms_recorded_;
  bool web_timing_histograms_recorded_;
  bool was_fetched_via_spdy_;
  bool was_npn_negotiated_;
  std::string npn_negotiated_protocol_;
  bool was_alternate_protocol_available_;
  net::HttpResponseInfo::ConnectionInfo connection_info_;
  bool was_fetched_via_proxy_;

  
  bool was_prefetcher_;
  bool was_referred_by_prefetcher_;
  bool was_after_preconnect_request_;

  LoadType load_type_;

  scoped_ptr<NavigationState> navigation_state_;

  bool can_load_local_resources_;
};

#endif  

}  

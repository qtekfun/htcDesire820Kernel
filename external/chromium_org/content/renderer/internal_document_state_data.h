// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_INTERNAL_DOCUMENT_STATE_DATA_H_
#define CONTENT_RENDERER_INTERNAL_DOCUMENT_STATE_DATA_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/supports_user_data.h"
#include "third_party/WebKit/public/platform/WebReferrerPolicy.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "url/gurl.h"

namespace blink {
class WebDataSource;
}

namespace content {

class AltErrorPageResourceFetcher;
class DocumentState;

class InternalDocumentStateData : public base::SupportsUserData::Data {
 public:
  InternalDocumentStateData();

  static InternalDocumentStateData* FromDataSource(blink::WebDataSource* ds);
  static InternalDocumentStateData* FromDocumentState(DocumentState* ds);

  
  
  bool did_first_visually_non_empty_layout() const {
    return did_first_visually_non_empty_layout_;
  }
  void set_did_first_visually_non_empty_layout(bool value) {
    did_first_visually_non_empty_layout_ = value;
  }

  
  
  
  bool did_first_visually_non_empty_paint() const {
    return did_first_visually_non_empty_paint_;
  }
  void set_did_first_visually_non_empty_paint(bool value) {
    did_first_visually_non_empty_paint_ = value;
  }

  int http_status_code() const { return http_status_code_; }
  void set_http_status_code(int http_status_code) {
    http_status_code_ = http_status_code;
  }

  const GURL& searchable_form_url() const { return searchable_form_url_; }
  void set_searchable_form_url(const GURL& url) { searchable_form_url_ = url; }
  const std::string& searchable_form_encoding() const {
    return searchable_form_encoding_;
  }
  void set_searchable_form_encoding(const std::string& encoding) {
    searchable_form_encoding_ = encoding;
  }

  
  
  bool use_error_page() const { return use_error_page_; }
  void set_use_error_page(bool use_error_page) {
    use_error_page_ = use_error_page;
  }

  
  bool is_overriding_user_agent() const { return is_overriding_user_agent_; }
  void set_is_overriding_user_agent(bool state) {
    is_overriding_user_agent_ = state;
  }

  
  
  bool must_reset_scroll_and_scale_state() const {
    return must_reset_scroll_and_scale_state_;
  }
  void set_must_reset_scroll_and_scale_state(bool state) {
    must_reset_scroll_and_scale_state_ = state;
  }

  
  
  
  void set_cache_policy_override(
      blink::WebURLRequest::CachePolicy cache_policy) {
    cache_policy_override_ = cache_policy;
    cache_policy_override_set_ = true;
  }
  blink::WebURLRequest::CachePolicy cache_policy_override() const {
    return cache_policy_override_;
  }
  void clear_cache_policy_override() {
    cache_policy_override_set_ = false;
    cache_policy_override_ = blink::WebURLRequest::UseProtocolCachePolicy;
  }
  bool is_cache_policy_override_set() const {
    return cache_policy_override_set_;
  }

  
  
  
  blink::WebReferrerPolicy referrer_policy() const {
    return referrer_policy_;
  }
  void set_referrer_policy(blink::WebReferrerPolicy referrer_policy) {
    referrer_policy_ = referrer_policy;
    referrer_policy_set_ = true;
  }
  void clear_referrer_policy() {
    referrer_policy_ = blink::WebReferrerPolicyDefault;
    referrer_policy_set_ = false;
  }
  bool is_referrer_policy_set() const { return referrer_policy_set_; }

  AltErrorPageResourceFetcher* alt_error_page_fetcher() const {
    return alt_error_page_fetcher_.get();
  }
  void set_alt_error_page_fetcher(AltErrorPageResourceFetcher* f);

 protected:
  virtual ~InternalDocumentStateData();

 private:
  bool did_first_visually_non_empty_layout_;
  bool did_first_visually_non_empty_paint_;
  int http_status_code_;
  GURL searchable_form_url_;
  std::string searchable_form_encoding_;
  bool use_error_page_;
  bool is_overriding_user_agent_;
  bool must_reset_scroll_and_scale_state_;
  bool cache_policy_override_set_;
  blink::WebURLRequest::CachePolicy cache_policy_override_;
  bool referrer_policy_set_;
  blink::WebReferrerPolicy referrer_policy_;
  scoped_ptr<AltErrorPageResourceFetcher> alt_error_page_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(InternalDocumentStateData);
};

}  

#endif  

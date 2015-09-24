// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_WEBREQUEST_WEBREQUEST_CONDITION_ATTRIBUTE_H_
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_WEBREQUEST_WEBREQUEST_CONDITION_ATTRIBUTE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/declarative_webrequest/request_stage.h"
#include "chrome/common/extensions/api/events.h"
#include "webkit/common/resource_type.h"

namespace base {
class Value;
}

namespace net {
class URLRequest;
}

namespace extensions {

class HeaderMatcher;
struct WebRequestData;

class WebRequestConditionAttribute
    : public base::RefCounted<WebRequestConditionAttribute> {
 public:
  enum Type {
    CONDITION_RESOURCE_TYPE,
    CONDITION_CONTENT_TYPE,
    CONDITION_RESPONSE_HEADERS,
    CONDITION_THIRD_PARTY,
    CONDITION_REQUEST_HEADERS,
    CONDITION_STAGES
  };

  WebRequestConditionAttribute();

  
  
  
  
  static scoped_refptr<const WebRequestConditionAttribute> Create(
      const std::string& name,
      const base::Value* value,
      std::string* error);

  
  
  
  virtual int GetStages() const = 0;

  
  virtual bool IsFulfilled(
      const WebRequestData& request_data) const = 0;

  virtual Type GetType() const = 0;
  virtual std::string GetName() const = 0;

  
  
  virtual bool Equals(const WebRequestConditionAttribute* other) const;

 protected:
  friend class base::RefCounted<WebRequestConditionAttribute>;
  virtual ~WebRequestConditionAttribute();

 private:
  DISALLOW_COPY_AND_ASSIGN(WebRequestConditionAttribute);
};

typedef std::vector<scoped_refptr<const WebRequestConditionAttribute> >
    WebRequestConditionAttributes;


class WebRequestConditionAttributeResourceType
    : public WebRequestConditionAttribute {
 public:
  
  static scoped_refptr<const WebRequestConditionAttribute> Create(
      const std::string& instance_type,
      const base::Value* value,
      std::string* error,
      bool* bad_message);

  
  virtual int GetStages() const OVERRIDE;
  virtual bool IsFulfilled(
      const WebRequestData& request_data) const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual bool Equals(const WebRequestConditionAttribute* other) const OVERRIDE;

 private:
  explicit WebRequestConditionAttributeResourceType(
      const std::vector<ResourceType::Type>& types);
  virtual ~WebRequestConditionAttributeResourceType();

  const std::vector<ResourceType::Type> types_;

  DISALLOW_COPY_AND_ASSIGN(WebRequestConditionAttributeResourceType);
};

class WebRequestConditionAttributeContentType
    : public WebRequestConditionAttribute {
 public:
  
  static scoped_refptr<const WebRequestConditionAttribute> Create(
      const std::string& name,
      const base::Value* value,
      std::string* error,
      bool* bad_message);

  
  virtual int GetStages() const OVERRIDE;
  virtual bool IsFulfilled(
      const WebRequestData& request_data) const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual bool Equals(const WebRequestConditionAttribute* other) const OVERRIDE;

 private:
  explicit WebRequestConditionAttributeContentType(
      const std::vector<std::string>& include_content_types,
      bool inclusive);
  virtual ~WebRequestConditionAttributeContentType();

  const std::vector<std::string> content_types_;
  const bool inclusive_;

  DISALLOW_COPY_AND_ASSIGN(WebRequestConditionAttributeContentType);
};

class WebRequestConditionAttributeRequestHeaders
    : public WebRequestConditionAttribute {
 public:
  
  static scoped_refptr<const WebRequestConditionAttribute> Create(
      const std::string& name,
      const base::Value* value,
      std::string* error,
      bool* bad_message);

  
  virtual int GetStages() const OVERRIDE;
  virtual bool IsFulfilled(
      const WebRequestData& request_data) const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual bool Equals(const WebRequestConditionAttribute* other) const OVERRIDE;

 private:
  WebRequestConditionAttributeRequestHeaders(
      scoped_ptr<const HeaderMatcher> header_matcher, bool positive);
  virtual ~WebRequestConditionAttributeRequestHeaders();

  const scoped_ptr<const HeaderMatcher> header_matcher_;
  const bool positive_;

  DISALLOW_COPY_AND_ASSIGN(WebRequestConditionAttributeRequestHeaders);
};

class WebRequestConditionAttributeResponseHeaders
    : public WebRequestConditionAttribute {
 public:
  
  static scoped_refptr<const WebRequestConditionAttribute> Create(
      const std::string& name,
      const base::Value* value,
      std::string* error,
      bool* bad_message);

  
  virtual int GetStages() const OVERRIDE;
  virtual bool IsFulfilled(
      const WebRequestData& request_data) const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual bool Equals(const WebRequestConditionAttribute* other) const OVERRIDE;

 private:
  WebRequestConditionAttributeResponseHeaders(
      scoped_ptr<const HeaderMatcher> header_matcher, bool positive);
  virtual ~WebRequestConditionAttributeResponseHeaders();

  const scoped_ptr<const HeaderMatcher> header_matcher_;
  const bool positive_;

  DISALLOW_COPY_AND_ASSIGN(WebRequestConditionAttributeResponseHeaders);
};

class WebRequestConditionAttributeThirdParty
    : public WebRequestConditionAttribute {
 public:
  
  static scoped_refptr<const WebRequestConditionAttribute> Create(
      const std::string& name,
      const base::Value* value,
      std::string* error,
      bool* bad_message);

  
  virtual int GetStages() const OVERRIDE;
  virtual bool IsFulfilled(
      const WebRequestData& request_data) const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual bool Equals(const WebRequestConditionAttribute* other) const OVERRIDE;

 private:
  explicit WebRequestConditionAttributeThirdParty(bool match_third_party);
  virtual ~WebRequestConditionAttributeThirdParty();

  const bool match_third_party_;

  DISALLOW_COPY_AND_ASSIGN(WebRequestConditionAttributeThirdParty);
};

class WebRequestConditionAttributeStages
    : public WebRequestConditionAttribute {
 public:
  
  static scoped_refptr<const WebRequestConditionAttribute> Create(
      const std::string& name,
      const base::Value* value,
      std::string* error,
      bool* bad_message);

  
  virtual int GetStages() const OVERRIDE;
  virtual bool IsFulfilled(
      const WebRequestData& request_data) const OVERRIDE;
  virtual Type GetType() const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual bool Equals(const WebRequestConditionAttribute* other) const OVERRIDE;

 private:
  explicit WebRequestConditionAttributeStages(int allowed_stages);
  virtual ~WebRequestConditionAttributeStages();

  const int allowed_stages_;  

  DISALLOW_COPY_AND_ASSIGN(WebRequestConditionAttributeStages);
};

}  

#endif  

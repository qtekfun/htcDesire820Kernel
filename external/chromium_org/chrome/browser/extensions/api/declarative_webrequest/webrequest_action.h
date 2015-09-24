// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_WEBREQUEST_WEBREQUEST_ACTION_H_
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_WEBREQUEST_WEBREQUEST_ACTION_H_

#include <list>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/extensions/api/declarative/declarative_rule.h"
#include "chrome/browser/extensions/api/declarative_webrequest/request_stage.h"
#include "chrome/browser/extensions/api/web_request/web_request_api_helpers.h"
#include "chrome/common/extensions/api/events.h"
#include "url/gurl.h"

class WebRequestPermission;

namespace base {
class DictionaryValue;
class Time;
class Value;
}

namespace extension_web_request_api_helpers {
struct EventResponseDelta;
}

namespace extensions {
class Extension;
class InfoMap;
struct WebRequestData;
}

namespace net {
class URLRequest;
}

namespace re2 {
class RE2;
}

namespace extensions {

typedef linked_ptr<extension_web_request_api_helpers::EventResponseDelta>
    LinkedPtrEventResponseDelta;

class WebRequestAction : public base::RefCounted<WebRequestAction> {
 public:
  
  
  
  enum Type {
    ACTION_CANCEL_REQUEST,
    ACTION_REDIRECT_REQUEST,
    ACTION_REDIRECT_TO_TRANSPARENT_IMAGE,
    ACTION_REDIRECT_TO_EMPTY_DOCUMENT,
    ACTION_REDIRECT_BY_REGEX_DOCUMENT,
    ACTION_SET_REQUEST_HEADER,
    ACTION_REMOVE_REQUEST_HEADER,
    ACTION_ADD_RESPONSE_HEADER,
    ACTION_REMOVE_RESPONSE_HEADER,
    ACTION_IGNORE_RULES,
    ACTION_MODIFY_REQUEST_COOKIE,
    ACTION_MODIFY_RESPONSE_COOKIE,
    ACTION_SEND_MESSAGE_TO_EXTENSION,
  };

  
  enum HostPermissionsStrategy {
    STRATEGY_NONE,     
    STRATEGY_DEFAULT,  
                       
    STRATEGY_HOST,     
                       
  };

  
  
  struct ApplyInfo {
    const InfoMap* extension_info_map;
    const WebRequestData& request_data;
    bool crosses_incognito;
    
    std::list<LinkedPtrEventResponseDelta>* deltas;
    std::set<std::string>* ignored_tags;
  };

  int stages() const {
    return stages_;
  }

  Type type() const {
    return type_;
  }

  
  
  virtual bool Equals(const WebRequestAction* other) const;

  
  
  virtual std::string GetName() const = 0;

  int minimum_priority() const {
    return minimum_priority_;
  }

  HostPermissionsStrategy host_permissions_strategy() const {
    return host_permissions_strategy_;
  }

  
  
  
  
  
  
  
  virtual bool HasPermission(const InfoMap* extension_info_map,
                             const std::string& extension_id,
                             const net::URLRequest* request,
                             bool crosses_incognito) const;

  
  
  
  
  
  
  static scoped_refptr<const WebRequestAction> Create(
      const Extension* extension,
      const base::Value& json_action,
      std::string* error,
      bool* bad_message);

  
  
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const = 0;

  
  
  void Apply(const std::string& extension_id,
             base::Time extension_install_time,
             ApplyInfo* apply_info) const;

 protected:
  friend class base::RefCounted<WebRequestAction>;
  virtual ~WebRequestAction();
  WebRequestAction(int stages,
                   Type type,
                   int minimum_priority,
                   HostPermissionsStrategy strategy);

 private:
  
  
  const int stages_;

  const Type type_;

  
  
  const int minimum_priority_;

  
  const HostPermissionsStrategy host_permissions_strategy_;
};

typedef DeclarativeActionSet<WebRequestAction> WebRequestActionSet;


class WebRequestCancelAction : public WebRequestAction {
 public:
  WebRequestCancelAction();

  
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestCancelAction();
  DISALLOW_COPY_AND_ASSIGN(WebRequestCancelAction);
};

class WebRequestRedirectAction : public WebRequestAction {
 public:
  explicit WebRequestRedirectAction(const GURL& redirect_url);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestRedirectAction();

  GURL redirect_url_;  

  DISALLOW_COPY_AND_ASSIGN(WebRequestRedirectAction);
};

class WebRequestRedirectToTransparentImageAction : public WebRequestAction {
 public:
  WebRequestRedirectToTransparentImageAction();

  
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestRedirectToTransparentImageAction();
  DISALLOW_COPY_AND_ASSIGN(WebRequestRedirectToTransparentImageAction);
};


class WebRequestRedirectToEmptyDocumentAction : public WebRequestAction {
 public:
  WebRequestRedirectToEmptyDocumentAction();

  
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestRedirectToEmptyDocumentAction();
  DISALLOW_COPY_AND_ASSIGN(WebRequestRedirectToEmptyDocumentAction);
};

class WebRequestRedirectByRegExAction : public WebRequestAction {
 public:
  
  
  explicit WebRequestRedirectByRegExAction(scoped_ptr<re2::RE2> from_pattern,
                                           const std::string& to_pattern);

  
  
  static std::string PerlToRe2Style(const std::string& perl);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestRedirectByRegExAction();

  scoped_ptr<re2::RE2> from_pattern_;
  std::string to_pattern_;

  DISALLOW_COPY_AND_ASSIGN(WebRequestRedirectByRegExAction);
};

class WebRequestSetRequestHeaderAction : public WebRequestAction {
 public:
  WebRequestSetRequestHeaderAction(const std::string& name,
                                   const std::string& value);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestSetRequestHeaderAction();

  std::string name_;
  std::string value_;
  DISALLOW_COPY_AND_ASSIGN(WebRequestSetRequestHeaderAction);
};

class WebRequestRemoveRequestHeaderAction : public WebRequestAction {
 public:
  explicit WebRequestRemoveRequestHeaderAction(const std::string& name);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestRemoveRequestHeaderAction();

  std::string name_;
  DISALLOW_COPY_AND_ASSIGN(WebRequestRemoveRequestHeaderAction);
};

class WebRequestAddResponseHeaderAction : public WebRequestAction {
 public:
  WebRequestAddResponseHeaderAction(const std::string& name,
                                    const std::string& value);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestAddResponseHeaderAction();

  std::string name_;
  std::string value_;
  DISALLOW_COPY_AND_ASSIGN(WebRequestAddResponseHeaderAction);
};

class WebRequestRemoveResponseHeaderAction : public WebRequestAction {
 public:
  explicit WebRequestRemoveResponseHeaderAction(const std::string& name,
                                                const std::string& value,
                                                bool has_value);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestRemoveResponseHeaderAction();

  std::string name_;
  std::string value_;
  bool has_value_;
  DISALLOW_COPY_AND_ASSIGN(WebRequestRemoveResponseHeaderAction);
};

class WebRequestIgnoreRulesAction : public WebRequestAction {
 public:
  explicit WebRequestIgnoreRulesAction(int minimum_priority,
                                       const std::string& ignore_tag);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;
  const std::string& ignore_tag() const { return ignore_tag_; }

 private:
  virtual ~WebRequestIgnoreRulesAction();

  
  
  std::string ignore_tag_;
  DISALLOW_COPY_AND_ASSIGN(WebRequestIgnoreRulesAction);
};

class WebRequestRequestCookieAction : public WebRequestAction {
 public:
  typedef extension_web_request_api_helpers::RequestCookieModification
      RequestCookieModification;

  explicit WebRequestRequestCookieAction(
      linked_ptr<RequestCookieModification> request_cookie_modification);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestRequestCookieAction();

  linked_ptr<RequestCookieModification> request_cookie_modification_;
  DISALLOW_COPY_AND_ASSIGN(WebRequestRequestCookieAction);
};

class WebRequestResponseCookieAction : public WebRequestAction {
 public:
  typedef extension_web_request_api_helpers::ResponseCookieModification
      ResponseCookieModification;

  explicit WebRequestResponseCookieAction(
      linked_ptr<ResponseCookieModification> response_cookie_modification);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestResponseCookieAction();

  linked_ptr<ResponseCookieModification> response_cookie_modification_;
  DISALLOW_COPY_AND_ASSIGN(WebRequestResponseCookieAction);
};

class WebRequestSendMessageToExtensionAction : public WebRequestAction {
 public:
  explicit WebRequestSendMessageToExtensionAction(const std::string& message);

  
  virtual bool Equals(const WebRequestAction* other) const OVERRIDE;
  virtual std::string GetName() const OVERRIDE;
  virtual LinkedPtrEventResponseDelta CreateDelta(
      const WebRequestData& request_data,
      const std::string& extension_id,
      const base::Time& extension_install_time) const OVERRIDE;

 private:
  virtual ~WebRequestSendMessageToExtensionAction();

  std::string message_;
  DISALLOW_COPY_AND_ASSIGN(WebRequestSendMessageToExtensionAction);
};

}  

#endif  

// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_SITE_ISOLATION_POLICY_H_
#define CONTENT_CHILD_SITE_ISOLATION_POLICY_H_

#include <map>
#include <utility>

#include "base/gtest_prod_util.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/platform/WebURLRequest.h"
#include "third_party/WebKit/public/platform/WebURLResponse.h"
#include "third_party/WebKit/public/web/WebFrame.h"
#include "webkit/common/resource_response_info.h"
#include "webkit/common/resource_type.h"

namespace content {


class CONTENT_EXPORT SiteIsolationPolicy {
 public:
  
  static void SetPolicyEnabled(bool enabled);

  
  
  
  
  static void OnReceivedResponse(int request_id,
                                 const GURL& frame_origin,
                                 const GURL& response_url,
                                 ResourceType::Type resource_type,
                                 int origin_pid,
                                 const webkit_glue::ResourceResponseInfo& info);

  
  
  
  
  
  static bool ShouldBlockResponse(int request_id,
                                  const char* payload,
                                  int length,
                                  std::string* alternative_data);

  
  static void OnRequestComplete(int request_id);

  struct ResponseMetaData {

    enum CanonicalMimeType {
      HTML = 0,
      XML = 1,
      JSON = 2,
      Plain = 3,
      Others = 4,
      MaxCanonicalMimeType,
    };

    ResponseMetaData();

    std::string frame_origin;
    GURL response_url;
    ResourceType::Type resource_type;
    CanonicalMimeType canonical_mime_type;
    int http_status_code;
    bool no_sniff;
  };

  typedef std::map<int, ResponseMetaData> RequestIdToMetaDataMap;
  typedef std::map<int, bool> RequestIdToResultMap;

private:
  FRIEND_TEST_ALL_PREFIXES(SiteIsolationPolicyTest, IsBlockableScheme);
  FRIEND_TEST_ALL_PREFIXES(SiteIsolationPolicyTest, IsSameSite);
  FRIEND_TEST_ALL_PREFIXES(SiteIsolationPolicyTest, IsValidCorsHeaderSet);
  FRIEND_TEST_ALL_PREFIXES(SiteIsolationPolicyTest, SniffForHTML);
  FRIEND_TEST_ALL_PREFIXES(SiteIsolationPolicyTest, SniffForXML);
  FRIEND_TEST_ALL_PREFIXES(SiteIsolationPolicyTest, SniffForJSON);
  FRIEND_TEST_ALL_PREFIXES(SiteIsolationPolicyTest, SniffForJS);

  
  
  
  static ResponseMetaData::CanonicalMimeType GetCanonicalMimeType(
      const std::string& mime_type);

  
  
  static bool IsBlockableScheme(const GURL& frame_origin);

  
  static bool IsSameSite(const GURL& frame_origin, const GURL& response_url);

  
  
  
  
  
  
  
  
  static bool IsValidCorsHeaderSet(const GURL& frame_origin,
                                   const GURL& website_origin,
                                   const std::string& access_control_origin);

  static bool SniffForHTML(base::StringPiece data);
  static bool SniffForXML(base::StringPiece data);
  static bool SniffForJSON(base::StringPiece data);

  
  
  static bool SniffForJS(base::StringPiece data);

  
  SiteIsolationPolicy() {}
  ~SiteIsolationPolicy() {}

  DISALLOW_COPY_AND_ASSIGN(SiteIsolationPolicy);
};

}  

#endif  

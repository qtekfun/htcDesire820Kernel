// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_NPAPI_PLUGIN_STREAM_URL_H_
#define CONTENT_CHILD_NPAPI_PLUGIN_STREAM_URL_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "content/child/npapi/plugin_stream.h"
#include "content/child/npapi/webplugin_resource_client.h"
#include "url/gurl.h"

namespace content {
class PluginInstance;
class PluginURLFetcher;

class PluginStreamUrl : public PluginStream,
                        public WebPluginResourceClient {
 public:
  
  
  
  
  PluginStreamUrl(unsigned long resource_id,
                  const GURL &url,
                  PluginInstance *instance,
                  bool notify_needed,
                  void *notify_data);

  void SetPluginURLFetcher(PluginURLFetcher* fetcher);

  void URLRedirectResponse(bool allow);

  
  
  
  virtual bool Close(NPReason reason) OVERRIDE;
  virtual WebPluginResourceClient* AsResourceClient() OVERRIDE;
  virtual void CancelRequest() OVERRIDE;

  
  virtual void WillSendRequest(const GURL& url, int http_status_code) OVERRIDE;
  virtual void DidReceiveResponse(const std::string& mime_type,
                                  const std::string& headers,
                                  uint32 expected_length,
                                  uint32 last_modified,
                                  bool request_is_seekable) OVERRIDE;
  virtual void DidReceiveData(const char* buffer,
                              int length,
                              int data_offset) OVERRIDE;
  virtual void DidFinishLoading(unsigned long resource_id) OVERRIDE;
  virtual void DidFail(unsigned long resource_id) OVERRIDE;
  virtual bool IsMultiByteResponseExpected() OVERRIDE;
  virtual int ResourceId() OVERRIDE;
  virtual void AddRangeRequestResourceId(unsigned long resource_id) OVERRIDE;

 protected:
  virtual ~PluginStreamUrl();

 private:
  void SetDeferLoading(bool value);

  
  
  void UpdateUrl(const char* url);

  GURL url_;
  unsigned long id_;
  
  
  std::vector<unsigned long> range_requests_;

  
  
  
  std::string pending_redirect_url_;

  scoped_ptr<PluginURLFetcher> plugin_url_fetcher_;

  DISALLOW_COPY_AND_ASSIGN(PluginStreamUrl);
};

}  

#endif 

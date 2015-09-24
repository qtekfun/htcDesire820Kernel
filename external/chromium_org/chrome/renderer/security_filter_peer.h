// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SECURITY_FILTER_PEER_H_
#define CHROME_RENDERER_SECURITY_FILTER_PEER_H_

#include "webkit/child/resource_loader_bridge.h"

class SecurityFilterPeer : public webkit_glue::ResourceLoaderBridge::Peer {
 public:
  virtual ~SecurityFilterPeer();

  static SecurityFilterPeer* CreateSecurityFilterPeerForDeniedRequest(
      ResourceType::Type resource_type,
      webkit_glue::ResourceLoaderBridge::Peer* peer,
      int os_error);

  static SecurityFilterPeer* CreateSecurityFilterPeerForFrame(
      webkit_glue::ResourceLoaderBridge::Peer* peer,
      int os_error);

  
  virtual void OnUploadProgress(uint64 position, uint64 size) OVERRIDE;
  virtual bool OnReceivedRedirect(
      const GURL& new_url,
      const webkit_glue::ResourceResponseInfo& info,
      bool* has_new_first_party_for_cookies,
      GURL* new_first_party_for_cookies) OVERRIDE;
  virtual void OnReceivedResponse(
      const webkit_glue::ResourceResponseInfo& info) OVERRIDE;
  virtual void OnDownloadedData(int len, int encoded_data_length) OVERRIDE {}
  virtual void OnReceivedData(const char* data,
                              int data_length,
                              int encoded_data_length) OVERRIDE;
  virtual void OnCompletedRequest(
      int error_code,
      bool was_ignored_by_handler,
      const std::string& security_info,
      const base::TimeTicks& completion_time) OVERRIDE;

 protected:
  SecurityFilterPeer(webkit_glue::ResourceLoaderBridge* resource_loader_bridge,
                     webkit_glue::ResourceLoaderBridge::Peer* peer);

  webkit_glue::ResourceLoaderBridge::Peer* original_peer_;
  webkit_glue::ResourceLoaderBridge* resource_loader_bridge_;

 private:
  DISALLOW_COPY_AND_ASSIGN(SecurityFilterPeer);
};

class BufferedPeer : public SecurityFilterPeer {
 public:
  BufferedPeer(webkit_glue::ResourceLoaderBridge* resource_loader_bridge,
               webkit_glue::ResourceLoaderBridge::Peer* peer,
               const std::string& mime_type);
  virtual ~BufferedPeer();

  
  virtual void OnReceivedResponse(
      const webkit_glue::ResourceResponseInfo& info) OVERRIDE;
  virtual void OnReceivedData(const char* data,
                              int data_length,
                              int encoded_data_length) OVERRIDE;
  virtual void OnCompletedRequest(
      int error_code,
      bool was_ignored_by_handler,
      const std::string& security_info,
      const base::TimeTicks& completion_time) OVERRIDE;

 protected:
  
  
  
  
  virtual bool DataReady() = 0;

  webkit_glue::ResourceResponseInfo response_info_;
  std::string data_;

 private:
  std::string mime_type_;

  DISALLOW_COPY_AND_ASSIGN(BufferedPeer);
};

class ReplaceContentPeer : public SecurityFilterPeer {
 public:
  ReplaceContentPeer(webkit_glue::ResourceLoaderBridge* resource_loader_bridge,
                     webkit_glue::ResourceLoaderBridge::Peer* peer,
                     const std::string& mime_type,
                     const std::string& data);
  virtual ~ReplaceContentPeer();

  
  virtual void OnReceivedResponse(
      const webkit_glue::ResourceResponseInfo& info) OVERRIDE;
  virtual void OnReceivedData(const char* data,
                              int data_length,
                              int encoded_data_length) OVERRIDE;
  virtual void OnCompletedRequest(
      int error_code,
      bool was_ignored_by_handler,
      const std::string& security_info,
      const base::TimeTicks& completion_time) OVERRIDE;

 private:
  webkit_glue::ResourceResponseInfo response_info_;
  std::string mime_type_;
  std::string data_;

  DISALLOW_COPY_AND_ASSIGN(ReplaceContentPeer);
};

#endif  

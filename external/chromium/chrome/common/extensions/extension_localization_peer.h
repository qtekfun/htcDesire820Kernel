// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_EXTENSION_LOCALIZATION_PEER_H_
#define CHROME_COMMON_EXTENSIONS_EXTENSION_LOCALIZATION_PEER_H_
#pragma once

#include <string>

#include "ipc/ipc_message.h"
#include "webkit/glue/resource_loader_bridge.h"

class ExtensionLocalizationPeer
    : public webkit_glue::ResourceLoaderBridge::Peer {
 public:
  virtual ~ExtensionLocalizationPeer();

  static ExtensionLocalizationPeer* CreateExtensionLocalizationPeer(
      webkit_glue::ResourceLoaderBridge::Peer* peer,
      IPC::Message::Sender* message_sender,
      const std::string& mime_type,
      const GURL& request_url);

  
  virtual void OnUploadProgress(uint64 position, uint64 size);
  virtual bool OnReceivedRedirect(
      const GURL& new_url,
      const webkit_glue::ResourceResponseInfo& info,
      bool* has_new_first_party_for_cookies,
      GURL* new_first_party_for_cookies);
  virtual void OnReceivedResponse(
      const webkit_glue::ResourceResponseInfo& info);
  virtual void OnDownloadedData(int len) {}
  virtual void OnReceivedData(const char* data,
                              int data_length,
                              int encoded_data_length);
  virtual void OnCompletedRequest(const net::URLRequestStatus& status,
                                  const std::string& security_info,
                                  const base::Time& completion_time);

 private:
  friend class ExtensionLocalizationPeerTest;

  
  ExtensionLocalizationPeer(
      webkit_glue::ResourceLoaderBridge::Peer* peer,
      IPC::Message::Sender* message_sender,
      const GURL& request_url);

  
  
  void ReplaceMessages();

  
  webkit_glue::ResourceLoaderBridge::Peer* original_peer_;

  
  webkit_glue::ResourceResponseInfo response_info_;

  
  
  IPC::Message::Sender* message_sender_;

  
  std::string data_;

  
  GURL request_url_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionLocalizationPeer);
};

#endif  

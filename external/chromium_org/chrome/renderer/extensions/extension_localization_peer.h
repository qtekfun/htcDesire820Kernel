// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_EXTENSION_LOCALIZATION_PEER_H_
#define CHROME_RENDERER_EXTENSIONS_EXTENSION_LOCALIZATION_PEER_H_

#include <string>

#include "ipc/ipc_sender.h"
#include "webkit/child/resource_loader_bridge.h"

class ExtensionLocalizationPeer
    : public webkit_glue::ResourceLoaderBridge::Peer {
 public:
  virtual ~ExtensionLocalizationPeer();

  static ExtensionLocalizationPeer* CreateExtensionLocalizationPeer(
      webkit_glue::ResourceLoaderBridge::Peer* peer,
      IPC::Sender* message_sender,
      const std::string& mime_type,
      const GURL& request_url);

  
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

 private:
  friend class ExtensionLocalizationPeerTest;

  
  ExtensionLocalizationPeer(
      webkit_glue::ResourceLoaderBridge::Peer* peer,
      IPC::Sender* message_sender,
      const GURL& request_url);

  
  
  void ReplaceMessages();

  
  webkit_glue::ResourceLoaderBridge::Peer* original_peer_;

  
  webkit_glue::ResourceResponseInfo response_info_;

  
  
  IPC::Sender* message_sender_;

  
  std::string data_;

  
  GURL request_url_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionLocalizationPeer);
};

#endif  

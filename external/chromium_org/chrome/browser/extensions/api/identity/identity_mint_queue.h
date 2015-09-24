// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IDENTITY_IDENTITY_MINT_QUEUE_H_
#define CHROME_BROWSER_EXTENSIONS_API_IDENTITY_IDENTITY_MINT_QUEUE_H_

#include <list>
#include <map>
#include <set>
#include <string>

#include "chrome/browser/extensions/api/identity/extension_token_key.h"

namespace extensions {

class IdentityMintRequestQueue {
 public:
  enum MintType {
    MINT_TYPE_NONINTERACTIVE,
    MINT_TYPE_INTERACTIVE
  };

  IdentityMintRequestQueue();
  virtual ~IdentityMintRequestQueue();

  class Request {
   public:
    virtual ~Request() {}
    virtual void StartMintToken(IdentityMintRequestQueue::MintType type) = 0;
  };

  
  void RequestStart(IdentityMintRequestQueue::MintType type,
                    const ExtensionTokenKey& key,
                    IdentityMintRequestQueue::Request* request);
  
  void RequestComplete(IdentityMintRequestQueue::MintType type,
                       const ExtensionTokenKey& key,
                       IdentityMintRequestQueue::Request* request);
  bool empty(IdentityMintRequestQueue::MintType type,
             const ExtensionTokenKey& key);

 private:
  typedef std::list<IdentityMintRequestQueue::Request*> RequestQueue;
  typedef std::map<const ExtensionTokenKey, RequestQueue> RequestQueueMap;

  RequestQueueMap& GetRequestQueueMap(IdentityMintRequestQueue::MintType type);

  RequestQueueMap interactive_request_queue_map_;
  RequestQueueMap noninteractive_request_queue_map_;
};

}  

#endif  

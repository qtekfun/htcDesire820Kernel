// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GEOLOCATION_FAKE_ACCESS_TOKEN_STORE_H_
#define CONTENT_BROWSER_GEOLOCATION_FAKE_ACCESS_TOKEN_STORE_H_

#include "base/message_loop/message_loop_proxy.h"
#include "content/public/browser/access_token_store.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace content {

class FakeAccessTokenStore : public AccessTokenStore {
 public:
  FakeAccessTokenStore();

  void NotifyDelegateTokensLoaded();

  
  MOCK_METHOD1(LoadAccessTokens,
               void(const LoadAccessTokensCallbackType& callback));
  MOCK_METHOD2(SaveAccessToken,
               void(const GURL& server_url,
                    const base::string16& access_token));

  void DefaultLoadAccessTokens(const LoadAccessTokensCallbackType& callback);

  void DefaultSaveAccessToken(const GURL& server_url,
                              const base::string16& access_token);

  AccessTokenSet access_token_set_;
  LoadAccessTokensCallbackType callback_;

 protected:
  
  virtual ~FakeAccessTokenStore();

 private:
  
  
  
  base::MessageLoopProxy* originating_message_loop_;

  DISALLOW_COPY_AND_ASSIGN(FakeAccessTokenStore);
};

}  

#endif  

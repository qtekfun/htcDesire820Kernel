// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_SEARCH_PROVIDER_INSTALL_STATE_MESSAGE_FILTER_H_
#define CHROME_BROWSER_SEARCH_ENGINES_SEARCH_PROVIDER_INSTALL_STATE_MESSAGE_FILTER_H_

#include "chrome/browser/search_engines/search_provider_install_data.h"
#include "chrome/common/search_provider.h"
#include "content/browser/browser_message_filter.h"

class GURL;
class Profile;

class SearchProviderInstallStateMessageFilter : public BrowserMessageFilter {
 public:
  
  SearchProviderInstallStateMessageFilter(int render_process_id,
                                          Profile* profile);
  ~SearchProviderInstallStateMessageFilter();

  
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok);

 private:
  
   search_provider::InstallState GetSearchProviderInstallState(
      const GURL& page_location,
      const GURL& requested_host);

  
  void OnMsgGetSearchProviderInstallState(const GURL& page_location,
                                          const GURL& requested_host,
                                          IPC::Message* reply_msg);

  
  void ReplyWithProviderInstallState(const GURL& page_location,
                                     const GURL& requested_host,
                                     IPC::Message* reply_msg);

  
  ScopedRunnableMethodFactory<SearchProviderInstallStateMessageFilter>
      reply_with_provider_install_state_factory_;

  
  SearchProviderInstallData provider_data_;

  
  
  const bool is_off_the_record_;

  DISALLOW_COPY_AND_ASSIGN(SearchProviderInstallStateMessageFilter);
};

#endif  
